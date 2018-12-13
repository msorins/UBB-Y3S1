#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <string>
#include <QApplication>
#include "UI/mainwindow.h"
using namespace std;







int main(int argc, char* argv[]) {
    while(true) {
        try {
            QApplication a(argc, argv);
            MainWindow w;
            w.show();
            a.exec();


        } catch(exception ex) {
            std::cerr<<ex.what();
        }
    }


//    RSA rsa;
//    for(int i = 1; i<= 1000; i++) {
//        long long encrypted = rsa.encrypt(i);
//        long long decrypted = rsa.decrypt(encrypted);
//
//        if(decrypted != i) {
//            cout << "bou \n";
//        }
//    }

//    while(true) {
//        RSAText rsaText;
//        string msg = "ana are mere";
//        string encrypted = rsaText.encrypt(msg);
//        string decrypted = rsaText.decrypt(encrypted);
//
//        if(decrypted != msg) {
//            cout << "esti bou\n";
//        }
//    }
    return 0;
}