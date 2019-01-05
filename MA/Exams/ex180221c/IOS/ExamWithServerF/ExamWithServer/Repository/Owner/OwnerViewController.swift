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
    
    @IBOutlet weak var deleteLocalDataButton: UIButton!
    @IBOutlet weak var activityIndicator: UIActivityIndicatorView!
    
    @IBOutlet weak var nameTextField: UITextField!
    @IBOutlet weak var modelTextField: UITextField!
    @IBOutlet weak var statusTextField: UITextField!
    @IBOutlet weak var seatsTextField: UITextField!
    
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
        
        setupDeleteLocalDataButton()
    }
    
    private func passDataToViewModel() {
        RealmRepository.shared.entitiesObservable
            .subscribe({ev in
                self.viewModel.input.listOfEntities.onNext(ev.element!)
                self.activityIndicator.isHidden = true
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
                        self?.activityIndicator.isHidden = false
                        self?.viewModel.update(el: cell.rv)
                }).disposed(by: cell.disposeBag!)
                
                // Set add ride tap
                cell.AddRidesButton
                    .rx
                    .anyGesture(.tap())
                    .when(.recognized)
                    .subscribe({[weak self] ev in
                        self?.activityIndicator.isHidden = false
                        self?.viewModel.addRides(rvID: cell.rv.ID, rides: Int(cell.AddRidesTextField.text ?? "") ?? 0)
                    })
                    .disposed(by: cell.disposeBag!)
               
                
                // Set delete ride tap
                cell.DeleteButton
                    .rx
                    .anyGesture(.tap())
                    .when(.recognized)
                    .subscribe({[weak self] ev in
                        self?.activityIndicator.isHidden = false
                        self?.viewModel.delete(el: cell.rv)
                    })
                    .disposed(by: cell.disposeBag!)
                
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
    
    private func setupDeleteLocalDataButton() {
        self.deleteLocalDataButton
            .rx.anyGesture(.tap())
            .when(.recognized)
            .subscribe({ev in
                print("Deleting all local data")
                RealmRepository.shared.deleteAllLocalData()
            })
    }
    
    @IBAction func addRV(_ sender: Any) {
        var id = RealmRepository.shared.getAll().count * 100
        var newRV = RV(ID: id, Name: self.nameTextField.text!, Model: self.modelTextField.text!, Status: self.statusTextField.text!, Seats: Int(self.seatsTextField.text!) ?? 0, Rides: 0)
        RealmRepository.shared.add(item: newRV)
    }
    
}


extension OwnerViewController: UICollectionViewDelegate, UICollectionViewDelegateFlowLayout, UIGestureRecognizerDelegate {
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
        var sz = CGSize(width: collectionView.frame.width / CGFloat(3) - CGFloat(minimumInteritemSpacing),
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

