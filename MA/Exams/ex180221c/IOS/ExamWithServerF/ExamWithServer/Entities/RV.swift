//
//  RVEntity.swift
//  ExamWithServer
//
//  Created by Sorin Sebastian Mircea on 03/01/2019.
//  Copyright © 2019 Sorin Sebastian Mircea. All rights reserved.
//

import Foundation
import RealmSwift

struct RV: Codable {
    var ID: Int
    var Name: String
    var Model: String
    var Status: String
    var Seats: Int
    var Rides: Int
    
    enum CodingKeys: String, CodingKey {
        case ID = "id"
        case Name = "name"
        case Model = "model"
        case Status = "status"
        case Seats = "seats"
        case Rides = "rides"
    }
}

class RealmRV: Object {
    @objc dynamic var ID: Int = 0
    @objc dynamic var Name: String = ""
    @objc dynamic var Model: String = ""
    @objc dynamic var Status: String = ""
    @objc dynamic var Seats: Int = 0
    @objc dynamic var Rides: Int = 0
    
    convenience required init(_ entity: RV) {
        self.init()
        self.ID = entity.ID
        self.Name = entity.Name
        self.Model = entity.Model
        self.Status = entity.Status
        self.Rides = entity.Rides
    }

    var entity: RV {
        return RV(ID: self.ID, Name: self.Name, Model: self.Model, Status: self.Status, Seats: self.Seats, Rides: self.Rides)
    }
    
    override static func primaryKey() -> String? {
        return "ID"
    }
}

struct UnsynscedRV: Codable {
    var ID: Int
    var Operation: String
    var Entity: RV?
}

class UnsyncedRealmRV: Object {
    @objc dynamic var ID: Int = 0
    @objc dynamic var Operation: String = ""
    
    @objc dynamic var Name: String = ""
    @objc dynamic var Model: String = ""
    @objc dynamic var Status: String = ""
    @objc dynamic var Seats: Int = 0
    @objc dynamic var Rides: Int = 0
    
    convenience required  init(_ entity: RV, _ operation: String) {
        self.init()
        self.ID = entity.ID
        self.Name = entity.Name
        self.Model = entity.Model
        self.Status = entity.Status
        self.Rides = entity.Rides
        self.Operation = operation
    }
    
    override static func primaryKey() -> String? {
        return "ID"
    }
    
    var entity: UnsynscedRV {
        let x = UnsynscedRV(ID: self.ID, Operation: self.Operation, Entity: RV(ID: self.ID, Name: self.Name, Model: self.Model, Status: self.Status, Seats: self.Seats, Rides: self.Rides) )
        return x
    }
}
