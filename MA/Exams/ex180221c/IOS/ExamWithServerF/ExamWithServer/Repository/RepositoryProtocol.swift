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
    func add(item: T) -> Bool
    
    func update(newItem: T) -> Bool
    func update(at: Int, newItem: T)    -> Bool
    
    func delete(ID: Int) -> Bool
    func delete(at: Int) -> Bool
    func deleteAllLocalData()
}
