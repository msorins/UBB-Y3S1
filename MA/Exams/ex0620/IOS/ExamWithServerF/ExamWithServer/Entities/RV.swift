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
    var Id: Int
    var Name: String
    var Tpe: String
    var Status: String
    var Employees: Int
    var CEO: String
    
    enum CodingKeys: String, CodingKey {
        case Id = "Id"
        case Name = "Name"
        case Tpe = "Type"
        case Status = "Status"
        case Employees = "Employees"
        case CEO = "CEO"
    }
}

class RealmRV: Object {
    @objc dynamic var Id: Int = 0
    @objc dynamic var Name: String = ""
    @objc dynamic var Tpe: String = ""
    @objc dynamic var Status: String = ""
    @objc dynamic var Employees: Int = 0
    @objc dynamic var CEO: String = ""
    
    convenience required init(_ entity: RV) {
        self.init()
        self.Id = entity.Id
        self.Name = entity.Name
        self.Tpe = entity.Tpe
        self.Status = entity.Status
        self.Employees = entity.Employees
        self.CEO = entity.CEO
    }

    var entity: RV {
        return RV(Id: self.Id, Name: self.Name, Tpe: self.Tpe, Status: self.Status, Employees: self.Employees, CEO: self.CEO)
    }
    
    override static func primaryKey() -> String? {
        return "Id"
    }
}

struct UnsynscedRV: Codable {
    var Id: Int
    var Operation: String
    var Entity: RV?
}

class UnsyncedRealmRV: Object {
    @objc dynamic var Id: Int = 0
    @objc dynamic var Operation: String = ""
 
    @objc dynamic var Name: String = ""
    @objc dynamic var Tpe: String = ""
    @objc dynamic var Status: String = ""
    @objc dynamic var Employees: Int = 0
     @objc dynamic var CEO: String = ""
    
    convenience required  init(_ entity: RV, _ operation: String) {
        self.init()
        self.Id = entity.Id
        self.Name = entity.Name
        self.Tpe = entity.Tpe
        self.Status = entity.Status
        self.Employees = entity.Employees
        self.CEO = entity.CEO
        self.Operation = operation
    }
    
    override static func primaryKey() -> String? {
        return "Id"
    }
    
    var entity: UnsynscedRV {
        let x = UnsynscedRV(Id: self.Id, Operation: self.Operation, Entity: RV(Id: self.Id, Name: self.Name, Tpe: self.Tpe, Status: self.Status, Employees: self.Employees, CEO: self.CEO) )
        return x
    }
}
