//
//  ItemsRepository.swift
//  ExamWithServer
//
//  Created by Sorin Sebastian Mircea on 03/01/2019.
//  Copyright © 2019 Sorin Sebastian Mircea. All rights reserved.
//
// https://stackoverflow.com/questions/42369333/create-generic-realm-repository-in-swift


import Foundation
import RxSwift
import RxCocoa
import Realm
import RealmSwift
import Alamofire
import TTGSnackbar
import os

let IP = "http://169.254.6.239:3000"

class RealmRepository: RepositoryProtocol {
    public static let shared = RealmRepository()
    
    public let entitiesObservable: Observable<[RV]>
    public let prospectsObservable: Observable<[RV]>
    public var entities: [RV] {
        get {
            return self.getAll()
        }
    }
    private var entitiesBehaviourSubject: BehaviorSubject<[RV]> = BehaviorSubject(value: [])
    private var prospectsBehaviourSubject: BehaviorSubject<[RV]> = BehaviorSubject(value: [])
    private let disposeBag = DisposeBag()
    private let realm = try! Realm()
    
    private init() {
        entitiesObservable = entitiesBehaviourSubject.asObservable()
        prospectsObservable = prospectsBehaviourSubject.asObserver()
        syncFromServer()
        syncToServer()
        
        // Trigger update
        self.entitiesBehaviourSubject.onNext(self.getAll())
    }
    
    func getAll() -> [RV] {
        return realm.objects(RealmRV.self).compactMap { $0.entity }
    }
    
    func get(ID: Int) -> RV? {
        for entity in self.entities {

            if entity.Id == ID {
                return entity
            }
        }
        
        return nil
    }
    
    func getAt(ID: Int) -> Int {
        var at = 0;
        for entity in self.entities {
            if entity.Id == ID {
                return at
            }
            
            at += 1
        }
        
        return at
    }
    
    func get(at: Int) -> RV? {
        if(at < 0 || at > self.entities.count) {
            return nil
        }
        
        return self.entities[at]
    }
    
    func add(item: RV) -> Bool{
        // An object with this key already exists
        if self.get(ID: item.Id) != nil {
            return false
        }
        
        do {
            try realm.write {
                // Edit Local
                realm.add(RealmRV(item), update: false)
                
                // Edit Server
                var unsync = UnsyncedRealmRV(item, "add")
                realm.add(unsync, update: true)
            }
        } catch let error{
            print("Error on add: ", error)
        }
      
        // Trigger update
        self.entitiesBehaviourSubject.onNext(self.getAll())
        return true
    }
    
    func update(newItem: RV) -> Bool {
        // Object with this id must exist
        if self.get(ID: newItem.Id) == nil {
            return false
        }
        
        do {
            try realm.write {
                // Edit Local
                realm.add(RealmRV(newItem), update: true)
                
                 // Edit on server
                var unsync = UnsyncedRealmRV(newItem, "update")
                realm.add(unsync, update: true)
            }
          
        } catch let error{
            print("Error on update: ", error)
        }
        
        // Do not rigger update -> wait for update to be synced with server
       //  self.entitiesBehaviourSubject.onNext(self.getAll())
        return true
    }
    
    func update(at: Int, newItem: RV)  -> Bool {
        let entities = self.getAll()
        if at >= 0 && at < entities.count {
            return self.update(newItem: newItem)
        }
        
        return false
    }
    
    func delete(ID: Int, local: Bool) -> Bool {
        let predicate = NSPredicate(format: "Id == %d", ID)
        if let toDelete = realm.objects(RealmRV.self).filter(predicate).first {
            do {
                  try self.realm.write {
                    // Delete on server
                    if local == false {
                         realm.add(UnsyncedRealmRV(toDelete.entity, "delete"), update: true)
                    }
                    
                    // Delete local
                     realm.delete(toDelete)
                    
                    // Trigger update
                    self.entitiesBehaviourSubject.onNext(self.getAll())     
                  }
            } catch {
                return false
            }
        
            return true
        }
        return false
    }
    
    func deleteAllLocalData() {
        do {
            try self.realm.write {
                // First delete all local entities
                self.realm.deleteAll()
                
                // Trigger update
                self.entitiesBehaviourSubject.onNext(self.getAll())
            }
        } catch {
            
        }
    }
    
    func deleteLocalEntities() {
        do {
            try self.realm.write {
                // First delete all local entities
                let objs = realm.objects(RealmRV.self)
                realm.delete(objs)
                
                // Trigger update
                self.entitiesBehaviourSubject.onNext(self.getAll())
            }
        } catch {
            
        }
    }
    
    func delete(at: Int) -> Bool {
        let entities = self.getAll()
        if at >= 0 && at < entities.count {
            return self.delete(ID: entities[at].Id, local: false)
        }
        
        return false
    }
    
    
    private func syncFromServer() {
        Alamofire
            .request(IP + "/clients",  method: .get, headers: [:])
            .responseJSON { res in
                if(res.error == nil) {
                    do {
                        let entities = try JSONDecoder().decode([RV].self, from: res.data!)
                        // First delete all local entities
                        self.deleteLocalEntities()
                        
                        try self.realm.write {
                            // Replace with the new ones
                            for entity in entities {
                                self.realm.add(RealmRV(entity))
                            }
                            
                            // Trigger update
                            self.entitiesBehaviourSubject.onNext(self.getAll())
                            
                            os_log("Sync with server was successful", log: .default, type: .info)
                        }

                    } catch let err{
                        print("error: ", err)
                        os_log("Sync with server failed", log: .default, type: .info)
                        
                        let snackbar = TTGSnackbar(message: "Sync with server failed", duration: .short)
                        snackbar.show()
                    }
                } else {
                    let snackbar = TTGSnackbar(message: "Sync with server failed", duration: .short)
                    snackbar.show()
                }
            }
    }
    
    
    public func getProspects() {
        Alamofire
            .request(IP + "/prospects",  method: .get, headers: [:])
            .responseJSON { res in
                if(res.error == nil) {
                    do {
                        let entities = try JSONDecoder().decode([RV].self, from: res.data!)
                        self.prospectsBehaviourSubject.onNext(entities)
                        
                        os_log("Prospects request to server was successful", log: .default, type: .info)
                        
                    } catch let err{
                        print("error: ", err)
                        os_log("Prospects with server failed", log: .default, type: .info)
                        
                        let snackbar = TTGSnackbar(message: "Prospects with server failed", duration: .short)
                        snackbar.show()
                    }
                } else {
                    let snackbar = TTGSnackbar(message: "Prospects with server failed", duration: .short)
                    snackbar.show()
                }
        }
    }
    
    private func syncToServer() {
        Observable<Int>
            .interval(5.0, scheduler: MainScheduler.instance)
            .subscribe({ ev in
                // Loop through all unsynced levels and try to add them
                do {
                    self.realm.objects(UnsyncedRealmRV.self).forEach { (a) in
                        switch a.Operation {
                        case "add":
                            self.syncAddToServer(a: a.entity)
                        case "update":
                            self.syncUpdateToServer(a: a.entity)
                        case "delete":
                            self.syncDeleteToServer(a: a.entity)
                        default:
                            break
                        }
                        
                        self.getProspects()
                    }
                }
                catch {
                    
                }
            })
            .disposed(by: self.disposeBag)
    }
    
    private func syncAddToServer(a: UnsynscedRV) {
        // Send the post request
        var request = URLRequest(url: URL(string: IP + "/new")!)
        request.httpMethod = HTTPMethod.post.rawValue
        request.setValue("application/json; charset=UTF-8", forHTTPHeaderField: "Content-Type")
        
        let jsonEncoder = JSONEncoder()
        do {
            let jsonData = try jsonEncoder.encode(a.Entity)
            request.httpBody = jsonData
        } catch {
            print("Enconding object to json failed")
            return
        }
        
        Alamofire.request(request)
            .responseJSON { res in
                do {
                    if res.response?.statusCode == 200 {
                        print("Update for object with id: ", a.Id,  "successfully synced to server")
                        os_log("Update for object synced with success", log: .default, type: .info)
                        
                        // Trigger update
                        self.entitiesBehaviourSubject.onNext(self.getAll())
                        
                        let almoRealm = try Realm()
                        let predicate = NSPredicate(format: "Id == %d", a.Id)
                        if let toDelete = almoRealm.objects(UnsyncedRealmRV.self).filter(predicate).first {
                            try almoRealm.write {
                                almoRealm.delete(toDelete)
                            }
                        }
                    } else {
                        print("ADD for object with id: ", a.Id,  "failed the syncing to server: ")
                        os_log("ADD for object failed while syncing", log: .default, type: .info)
                        
                        let snackbar = TTGSnackbar(message: "Sync add with server failed", duration: .short)
                        snackbar.show()
                    }
                } catch {
                    
                }
        }
    }
    
    
    private func syncDeleteToServer(a: UnsynscedRV) {
        Alamofire
            .request(IP + "/client/" + String(a.Id),  method: .delete, headers: [:])
            .responseJSON { res in
                if( res.response?.statusCode == 200 ) {
                    do {
                        let almoRealm = try Realm()
                        try almoRealm.write {
                            // Delete the level from unsynced added level
                            let predicate = NSPredicate(format: "Id == %d", a.Id)
                            if let toDelete = almoRealm.objects(UnsyncedRealmRV.self).filter(predicate).first {
                                 almoRealm.delete(toDelete)
                            }
                        }
                        
                        print("Delete for object with id: ", a.Id,  "successfuly synced with server")
                        os_log("Delete for object successfuly synced with server", log: .default, type: .info)
                    } catch {

                    }
                } else {
                    print("Delete for object with id: ", a.Id,  "failed the syncing to server: ")
                    os_log("Delete for object failed while syncing", log: .default, type: .info)
                    
                    let snackbar = TTGSnackbar(message: "Delete sync with server failed", duration: .short)
                    snackbar.show()
                }
        }
    }
    
    private func syncUpdateToServer(a: UnsynscedRV) {
        // Send the post request
        var request = URLRequest(url: URL(string: IP + "/update/" + String(a.Id))!)
        request.httpMethod = HTTPMethod.post.rawValue
        request.setValue("application/json; charset=UTF-8", forHTTPHeaderField: "Content-Type")
        
        let jsonEncoder = JSONEncoder()
        do {
            let jsonData = try jsonEncoder.encode(a.Entity)
            request.httpBody = jsonData
        } catch {
            print("Enconding object to json failed")
            return
        }
    
        Alamofire.request(request)
            .responseJSON { res in
                do {
                    if res.response?.statusCode == 200 {
                        print("Update for object with id: ", a.Id,  "successfully synced to server")
                        os_log("Update for object synced with success", log: .default, type: .info)
                        
                        // Trigger update
                        self.entitiesBehaviourSubject.onNext(self.getAll())
                        
                        let almoRealm = try Realm()
                        let predicate = NSPredicate(format: "Id == %d", a.Id)
                        if let toDelete = almoRealm.objects(UnsyncedRealmRV.self).filter(predicate).first {
                            try almoRealm.write {
                                almoRealm.delete(toDelete)
                            }
                        }
                    } else {
                        print("Update for object with id: ", a.Id,  "failed the syncing to server: ")
                        os_log("Update for object failed while syncing", log: .default, type: .info)
                        
                        let snackbar = TTGSnackbar(message: "Sync with server failed", duration: .short)
                        snackbar.show()
                    }
                } catch {
    
                }
        }
    }
    
    public func addEmployees(rvID: Int, rides: Int) {
        // Send the post request
        var request = URLRequest(url: URL(string: IP + "/employees/" + String(rvID) + "/" + String(rides))!)
        request.httpMethod = HTTPMethod.post.rawValue
        request.setValue("application/json; charset=UTF-8", forHTTPHeaderField: "Content-Type")
        
        Alamofire.request(request)
            .responseJSON { res in
                do {
                    if res.response?.statusCode == 200 {
                        print("Add employees for object with id: ", rvID,  "successfully synced to server")
                        os_log("Add employees for object was successful", log: .default, type: .info)
                        self.syncFromServer()
                    } else {
                        print("Update for object with id: ", rvID,  "failed the syncing to server: ")
                        os_log("Add employees for object failed", log: .default, type: .info)
                        
                        let snackbar = TTGSnackbar(message: "Change employees failed", duration: .short)
                        snackbar.show()
                    }
                } catch {
                    
                }
        }
    }
}

extension String: ParameterEncoding {
    
    public func encode(_ urlRequest: URLRequestConvertible, with parameters: Parameters?) throws -> URLRequest {
        var request = try urlRequest.asURLRequest()
        request.httpBody = data(using: .utf8, allowLossyConversion: false)
        return request
    }
    
}
