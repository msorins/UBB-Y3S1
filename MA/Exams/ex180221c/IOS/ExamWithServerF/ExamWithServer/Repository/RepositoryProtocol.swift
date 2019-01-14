//
//  RepositoryProtocol.swift
//  ExamWithServer
//
//  Created by Sorin Sebastian Mircea on 03/01/2019.
//  Copyright © 2019 Sorin Sebastian Mircea. All rights reserved.
//

import Foundation

protocol RepositoryProtocol {
    associatedtype T
    
    func getAll() -> [T]
    func get(ID: Int) -> T?
    
    func add(item: T, syncServer: Bool) -> Bool
    
    func update(newItem: T, syncServer: Bool) -> Bool
    func update(at: Int, newItem: T, syncServer: Bool)    -> Bool
    
    func delete(ID: Int, syncServer: Bool) -> Bool
    func delete(at: Int, syncServer: Bool) -> Bool
    
    func deleteAllLocalData()
}
