//
//  SocketMessages.swift
//  ExamWithServer
//
//  Created by Sorin Sebastian Mircea on 04/01/2019.
//  Copyright © 2019 Sorin Sebastian Mircea. All rights reserved.
//

import Foundation
import Starscream
import TTGSnackbar

let WSIP = "ws://demos.kaazing.com/echo"


class SocketMessages {
    public static let shared = SocketMessages()
    
    private init() {
      
    }
    
    public func connect() {
        DispatchQueue.global(qos: .background).async {
            print("Trying to connect socket")
            let socket = WebSocket(url: URL(string: WSIP)!)
            
            //websocketDidConnect
            socket.onConnect = {
                print("websocket is connected")
            }
            //websocketDidDisconnect
            socket.onDisconnect = { (error: Error?) in
                print("websocket is disconnected: \(error?.localizedDescription)")
            }
            //websocketDidReceiveMessage
            socket.onText = { (text: String) in
                print("got some text: \(text)")
                
                let snackbar = TTGSnackbar(message: text, duration: .short)
                snackbar.show()
            }
            //websocketDidReceiveData
            socket.onData = { (data: Data) in
                print("got some data: \(data.count)")
                
                let snackbar = TTGSnackbar(message: String(data: data, encoding: String.Encoding.utf8) as String!, duration: .short)
                snackbar.show()
            }
            //you could do onPong as well.
            
            socket.connect()
            
            while(true) {
                RunLoop.current.run(until: Date())
                usleep(10)
            }
        }
    }
    

}
