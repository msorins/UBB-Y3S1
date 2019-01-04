//
//  RVEntity.swift
//  ExamWithServer
//
//  Created by Sorin Sebastian Mircea on 03/01/2019.
//  Copyright © 2019 Sorin Sebastian Mircea. All rights reserved.
//

import Foundation
import RealmSwift

struct RV: Entity {
    var ID: Int
    var Name: String
    var Model: String
    var Status: String
    var Rides: Int
}

class RealmRVEntity: Object, RealmEntity {
    typealias EntityType = RV
    
    @objc dynamic var ID: Int = 0
    @objc dynamic var Name: String = ""
    @objc dynamic var Model: String = ""
    @objc dynamic var Status: String = ""
    @objc dynamic var Rides: Int = 0
    
    convenience required init(_ entity: EntityType) {
        self.init()
        self.ID = entity.ID
        self.Name = entity.Name
        self.Model = entity.Model
        self.Status = entity.Status
        self.Rides = entity.Rides
    }

    var entity: RV {
        return RV(ID: self.ID, Name: self.Name, Model: self.Model, Status: self.Status, Rides: self.Rides)
    }
}
