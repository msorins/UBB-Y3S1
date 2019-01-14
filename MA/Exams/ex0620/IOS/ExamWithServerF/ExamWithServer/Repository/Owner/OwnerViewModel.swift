//
//  OwnerViewModel.swift
//  ExamWithServer
//
//  Created by Sorin Sebastian Mircea on 04/01/2019.
//  Copyright © 2019 Sorin Sebastian Mircea. All rights reserved.
//

import Foundation
import RxSwift

final class OwnerViewModel {
    // Public members
    let input: Input
    struct Input {
        let listOfEntities: AnyObserver<[RV]>
    }
    
    let output: Output
    struct Output {
        let listOfCellInfo: Observable<[CellInfo]>
    }
    
    struct CellInfo {
        let rv: RV
    }
    
    // Private members
    private let listOfEntitiesSubject   = ReplaySubject<[RV]>.create(bufferSize: 1)
    private let outputSubject           = ReplaySubject<[CellInfo]>.create(bufferSize: 1)
    private let disposeBag              = DisposeBag()
    
    init() {
        self.input = Input(listOfEntities: self.listOfEntitiesSubject.asObserver())
        
        let out = self.listOfEntitiesSubject.map { (rvs) -> [CellInfo] in
            var cells: [CellInfo] = []
            for rv in rvs {
                cells.append(CellInfo(rv: rv))
            }
            
            return cells
        }

        self.output = Output(listOfCellInfo: out)
    }
    
    func update(el: RV) {
        RealmRepository.shared.update(newItem: el)
    }
    
    func addRides(rvID: Int, rides: Int) {
        RealmRepository.shared.addEmployees(rvID: rvID, rides: rides)
    }
    
    func deleteLocal(el: RV) {
        RealmRepository.shared.delete(ID: el.Id, local: true)
    }
    
    func deleteServer(el: RV) {
        RealmRepository.shared.delete(ID: el.Id, local: false)
    }
    
}
