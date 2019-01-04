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
    @IBOutlet weak var ModelTextField: UITextField!
    @IBOutlet weak var StatusTextField: UITextField!
    @IBOutlet weak var SeatsTextField: UITextField!
    @IBOutlet weak var RidesTextField: UITextField!
    @IBOutlet weak var UpdateButton: UIButton!
    
    var disposeBag: DisposeBag? = DisposeBag()
    
    func setProperties(rv: RV) {
        self.disposeBag = nil
        self.disposeBag = DisposeBag()
        
        self.IDLabel.text = String(rv.ID)
        self.NameTextField.text = rv.Name
        self.ModelTextField.text = rv.Model
        self.StatusTextField.text = rv.Status
        self.SeatsTextField.text = String(rv.Seats)
        self.RidesTextField.text = String(rv.Rides)
    }
    
    var rv: RV {
        return RV(ID: Int(self.IDLabel!.text!) ?? 0, Name: self.NameTextField.text!, Model: self.ModelTextField.text!, Status: self.StatusTextField.text!, Seats: Int(self.SeatsTextField!.text!) ?? 0, Rides: Int(self.RidesTextField!.text!) ?? 0)
    }
}

