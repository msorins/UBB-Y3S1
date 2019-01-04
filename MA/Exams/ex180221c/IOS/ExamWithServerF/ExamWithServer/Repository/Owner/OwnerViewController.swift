//
//  OwnerViewController.swift
//  ExamWithServer
//
//  Created by Sorin Sebastian Mircea on 04/01/2019.
//  Copyright © 2019 Sorin Sebastian Mircea. All rights reserved.
//

import UIKit
import RxSwift
import RxGesture


class OwnerViewController: UIViewController {
    @IBOutlet weak var rvsCollectionView: UICollectionView!
    
    private let viewModel = OwnerViewModel()
    private let disposeBag = DisposeBag()
    
    private var minimumLineSpacing:      Int = 5;
    private var minimumInteritemSpacing: Int = 5;
    
    override func viewDidLoad() {
        super.viewDidLoad()

        setupCollectionViewTap()
        passDataToViewModel()
        setupCollectionView()
        setupCollectionViewBinding()
    }
    
    private func passDataToViewModel() {
        RealmRepository.shared.entitiesObservable
            .subscribe({ev in
                self.viewModel.input.listOfEntities.onNext(ev.element!)
            })
            .disposed(by: self.disposeBag)
    }
    
    private func setupCollectionView() {
        self.rvsCollectionView.delegate = nil
        self.rvsCollectionView.dataSource = nil
    }
    
    private func setupCollectionViewBinding() {
        viewModel.output.listOfCellInfo
            .bind(to: self.rvsCollectionView.rx.items) { [weak self] view, row, element in
                let cell = self!.rvsCollectionView.dequeueReusableCell(withReuseIdentifier: "OwnerCell", for: IndexPath(row: row, section: 0)) as! OwnerCell
                cell.setProperties(rv: element.rv)
                
                // Set update tap
                cell.UpdateButton
                    .rx
                    .anyGesture(.tap())
                    .when(.recognized)
                    .subscribe(onNext: { [weak self] ev in
                        self?.viewModel.update(el: cell.rv)
                }).disposed(by: cell.disposeBag!)
                return cell;
                
            }
            .disposed(by: disposeBag)
        
        rvsCollectionView.rx.setDelegate(self)
            .disposed(by: disposeBag)
    }
    
    private func setupCollectionViewTap() {
//        self.rvsCollectionView.rx
//            .anyGesture(.tap())
//            .when(.recognized)
//            .subscribe(onNext: { gesture in
//                if let index = self.charactersCollectionView!.indexPathForItem(at: gesture.location(in: self.charactersCollectionView)) {
//                    self.viewModel.selectCharacter(at: index.row)
//                }
//            })
//            .disposed(by: self.disposeBag)
    }
    
}


extension OwnerViewController: UICollectionViewDelegate, UICollectionViewDelegateFlowLayout, UIGestureRecognizerDelegate {
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
        var sz = CGSize(width: collectionView.frame.width / CGFloat(2) - CGFloat(minimumInteritemSpacing),
                        height: collectionView.frame.height / CGFloat(2) - CGFloat(minimumLineSpacing) - 1)
        
        return sz
    }
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, referenceSizeForHeaderInSection section: Int) -> CGSize {
        return CGSize(width: collectionView.frame.width, height: 10)
    }
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, minimumLineSpacingForSectionAt section: Int) -> CGFloat {
        return CGFloat(minimumLineSpacing)
    }
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, minimumInteritemSpacingForSectionAt section: Int) -> CGFloat {
        return CGFloat(minimumInteritemSpacing)
    }
}

