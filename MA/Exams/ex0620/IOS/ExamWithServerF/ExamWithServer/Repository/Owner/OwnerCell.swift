//
//  OwnerCell.swift
//  ExamWithServer
//
//  Created by Sorin Sebastian Mircea on 04/01/2019.
//  Copyright © 2019 Sorin Sebastian Mircea. All rights reserved.
//
import Foundation
import UIKit
import RxSwift
import RxGesture

class OwnerCell : UICollectionViewCell{
    @IBOutlet weak var IDLabel: UILabel!
    @IBOutlet weak var NameTextField: UITextField!
    @IBOutlet weak var TpeTextField: UITextField!
    @IBOutlet weak var StatusTextField: UITextField!
    @IBOutlet weak var EmployeesTextField: UITextField!
    @IBOutlet weak var CEOTextField: UITextField!
    @IBOutlet weak var UpdateButton: UIButton!
    @IBOutlet weak var DeleteButton: UIButton!
    
    @IBOutlet weak var AddRidesTextField: UITextField!
    @IBOutlet weak var AddRidesButton: UIButton!
    
    var disposeBag: DisposeBag? = DisposeBag()
    
    func setProperties(rv: RV) {
        self.disposeBag = nil
        self.disposeBag = DisposeBag()
        
        self.IDLabel.text = String(rv.Id)
        self.NameTextField.text = rv.Name
        self.TpeTextField.text = rv.Tpe
        self.StatusTextField.text = rv.Status
        self.EmployeesTextField.text = String(rv.Employees)
        self.CEOTextField.text = String(rv.CEO)
    }
    
    var rv: RV {
        return RV(Id: Int(self.IDLabel!.text!) ?? 0, Name: self.NameTextField.text!, Tpe: self.TpeTextField.text!, Status: self.StatusTextField.text!, Employees: Int(self.EmployeesTextField!.text!) ?? 0, CEO: self.CEOTextField!.text! ?? "")
    }
}

