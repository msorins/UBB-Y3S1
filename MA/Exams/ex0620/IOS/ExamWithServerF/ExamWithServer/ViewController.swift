//
//  ViewController.swift
//  ExamWithServer
//
//  Created by Sorin Sebastian Mircea on 03/01/2019.
//  Copyright © 2019 Sorin Sebastian Mircea. All rights reserved.
//

import UIKit

class ViewController: UIPageViewController, UIPageViewControllerDataSource{

    lazy var viewControllerList:[UIViewController] = {
        let sb = UIStoryboard(name: "Main", bundle: nil)
        
        let ownerScene = sb.instantiateViewController(withIdentifier: "OwnerVC")
        let emplyeeScene = sb.instantiateViewController(withIdentifier: "EmployeeVC")
        
        return [ownerScene, emplyeeScene]
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.dataSource = self
        if let firstViewController = viewControllerList.first {
            self.setViewControllers([firstViewController], direction: .forward, animated: true, completion: nil)
        }
    }
    
    func pageViewController(_ pageViewController: UIPageViewController, viewControllerBefore viewController: UIViewController) -> UIViewController? {
        guard let vcIndex = viewControllerList.index(of: viewController) else { return nil }
        
        let previousIndex = vcIndex - 1
        
        guard previousIndex >= 0 else { return nil}
        guard viewControllerList.count > previousIndex else { return nil}
        
        return viewControllerList[previousIndex]
    }
    
    func pageViewController(_ pageViewController: UIPageViewController, viewControllerAfter viewController: UIViewController) -> UIViewController? {
        guard let vcIndex = viewControllerList.index(of: viewController) else { return nil }
        
        let nextIndex = vcIndex + 1
        
        guard viewControllerList.count != nextIndex else { return nil}
        guard viewControllerList.count > nextIndex else { return nil }
        
        return viewControllerList[nextIndex]
    }

}


