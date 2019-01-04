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

let IP = "169.254.37.138:4023"

class RealmRepository: RepositoryProtocol {
    public static let shared = RealmRepository()
    
    public let entitiesObservable: Observable<[RV]>
    public var entities: [RV] {
        get {
            return self.getAll()
        }
    }
    private var entitiesBehaviourSubject: BehaviorSubject<[RV]> = BehaviorSubject(value: [])
    private let disposeBag = DisposeBag()
    private let realm = try! Realm()
    
    private init() {
        entitiesObservable = entitiesBehaviourSubject.asObservable()
        syncFromServer()
        syncToServer()
    }
    
    func getAll() -> [RV] {
        return realm.objects(RealmRV.self).compactMap { $0.entity }
    }
    
    func getAllX() -> [UnsynscedRV] {
        return realm.objects(UnsyncedRealmRV.self).compactMap { $0.entity }
    }
    
    func get(ID: Int) -> RV? {
        for entity in self.entities {

            if entity.ID == ID {
                return entity
            }
        }
        
        return nil
    }
    
    func getAt(ID: Int) -> Int {
        var at = 0;
        for entity in self.entities {
            if entity.ID == ID {
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
        if self.get(ID: item.ID) != nil {
            return false
        }
        
        // Edit Local
        do {
            try realm.write {
                realm.add(RealmRV(item), update: false)
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
        if self.get(ID: newItem.ID) == nil {
            return false
        }
        
        do {
            try realm.write {
                // Edit Local
                realm.add(RealmRV(newItem), update: true)
                
                 // Edit on server
                realm.add(UnsyncedRealmRV(newItem, "update"), update: true)
                
            }
          
        } catch let error{
            print("Error on update: ", error)
        }
        
        // Trigger update
        self.entitiesBehaviourSubject.onNext(self.getAll())
        return true
    }
    
    func update(at: Int, newItem: RV)  -> Bool {
        let entities = self.getAll()
        if at >= 0 && at < entities.count {
            return self.update(newItem: newItem)
        }
        
        return false
    }
    
    func delete(ID: Int) -> Bool {
        let predicate = NSPredicate(format: "ID == %d", ID)
        if let toDelete = realm.objects(RealmRV.self).filter(predicate).first {
            do {
                  try self.realm.write {
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
            return self.delete(ID: entities[at].ID)
        }
        
        return false
    }
    
    func triggerUpdate(at: Int) {
        // Edit on server
        try! self.realm.write {
            //            let ual = UnsyncedLevel()
            //            ual.name = self.entities[at].Name
            //            ual.level = self.entities[at]
            //            ual.operation = "update"
            //            realm.add(ual, update: true)
        }
    }
    
    private func syncFromServer() {
        Alamofire
            .request("http://" + IP + "/rvs",  method: .get, headers: [:])
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
                        }

                    } catch let err{
                        print("error: ", err)
                        
                    }
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
//                        case "add":
//                            self.syncAddToServer(a: a)
                        case "update":
                            self.syncUpdateToServer(a: a.entity)
//                        case "delete":
//                            self.syncDeleteToServer(a: a)
                        default:
                            break
                        }
                    }
                }
                catch {
                    
                }
            })
            .disposed(by: self.disposeBag)
    }
    
    private func syncAddToServer(a: RV) {
//        let levelThreadSafeRef = ThreadSafeReference(to: a)
//
//        let jsonEncoder = JSONEncoder()
//        var json: String = ""
//        do {
//            let jsonData = try jsonEncoder.encode(a.level)
//            json = String(data: jsonData, encoding: String.Encoding.utf8)!
//        } catch {
//            print("Enconding object to json failed")
//            return
//        }
        
        // Send the post request
//        Alamofire.request("https://domino.serveo.net/levels", method: .post, parameters: [:], encoding: json, headers: [:])
//            .responseJSON { res in
//                if(res.error == nil) {
//                    do {
//                        let almoRealm = try Realm()
//                        let lvl = almoRealm.resolve(levelThreadSafeRef)
//                        try! almoRealm.write {
//                            // Delete the level from unsynced added level
//                            print(lvl!.name, " deleted from unsynced added data queue")
//                            almoRealm.delete(lvl!)
//                        }
//                    } catch {
//
//                    }
//                }
//        }
    }
    
    
    private func syncDeleteToServer(a: RV) {
//        let levelThreadSafeRef = ThreadSafeReference(to: a)
//
//        Alamofire
//            .request("https://domino.serveo.net/levels/" + a.name,  method: .delete, headers: [:])
//            .responseJSON { res in
//                if(res.error == nil) {
//                    do {
//                        let almoRealm = try Realm()
//                        let lvl = almoRealm.resolve(levelThreadSafeRef)
//                        try! almoRealm.write {
//                            // Delete the level from unsynced added level
//                            print(lvl!.name, " deleted from unsynced deleted data queue")
//                            almoRealm.delete(lvl!)
//                        }
//                    } catch {
//
//                    }
//                }
//        }
    }
    
    private func syncUpdateToServer(a: UnsynscedRV) {
        // Send the post request
        var request = URLRequest(url: URL(string: "http://" + IP + "/change")!)
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
                        print("Update for object with id: ", a.ID,  "successfully synced to server")
    
                        let almoRealm = try Realm()
                        let predicate = NSPredicate(format: "ID == %d", a.ID)
                        if let toDelete = almoRealm.objects(UnsyncedRealmRV.self).filter(predicate).first {
                            try almoRealm.write {
                                almoRealm.delete(toDelete)
                            }
                        }
                    } else {
                        print("Update for object with id: ", a.ID,  "failed the syncing to server: ")
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
