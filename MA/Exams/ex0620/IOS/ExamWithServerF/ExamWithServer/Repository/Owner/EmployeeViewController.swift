//
//  EmployeeViewController.swift
//  ExamWithServer
//
//  Created by Sorin Sebastian Mircea on 04/01/2019.
//  Copyright © 2019 Sorin Sebastian Mircea. All rights reserved.
//

import UIKit
import RxSwift
import RxCocoa

class EmployeeViewController: UIViewController {
    @IBOutlet weak var rvsCollectionView: UICollectionView!
    
    @IBOutlet weak var nameTextView: UITextField!
    @IBOutlet weak var typeTextView: UITextField!
    @IBOutlet weak var statusTextView: UITextField!
    @IBOutlet weak var employeesTextView: UITextField!
    @IBOutlet weak var CEOTextView: UITextField!
    
    @IBOutlet weak var idTextView: UITextField!
    private let disposeBag = DisposeBag()
    private let viewModel = OwnerViewModel()
    
    private var minimumLineSpacing:      Int = 5;
    private var minimumInteritemSpacing: Int = 5;
    
    override func viewDidLoad() {
        super.viewDidLoad()

        passDataToViewModel()
        setupCollectionView()
        setupCollectionViewBinding()
    }
    
    private func passDataToViewModel() {
        RealmRepository.shared.getProspects()
        RealmRepository.shared.prospectsObservable
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
                let cell = self!.rvsCollectionView.dequeueReusableCell(withReuseIdentifier: "EmployeeCell", for: IndexPath(row: row, section: 0)) as! OwnerCell
                cell.setProperties(rv: element.rv)
                
                return cell;
                
            }
            .disposed(by: disposeBag)
        
        rvsCollectionView.rx.setDelegate(self)
            .disposed(by: disposeBag)
    }
    
    @IBAction func addNew(_ sender: Any) {
        var id = RealmRepository.shared.getAll().count * 100
        var newRV = RV(Id: id, Name: self.nameTextView.text!, Tpe: self.typeTextView.text!, Status: self.statusTextView.text!, Employees: Int(self.employeesTextView.text!) ?? 0, CEO: self.CEOTextView.text!)
        RealmRepository.shared.add(item: newRV)
    }
    
    @IBAction func deleteById(_ sender: Any) {
        RealmRepository.shared.delete(ID: Int(self.idTextView.text!)!, local: false)
    }
    
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}

extension EmployeeViewController: UICollectionViewDelegate, UICollectionViewDelegateFlowLayout, UIGestureRecognizerDelegate {
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

