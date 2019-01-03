//
// Created by Sorin Sebastian Mircea on 12/12/2018.
//

#include "RSA.h"

#ifndef CRYPTO_LAB_3_RSA_RSATEXT_H
#define CRYPTO_LAB_3_RSA_RSATEXT_H

#endif //CRYPTO_LAB_3_RSA_RSATEXT_H


#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <string>

using namespace std;

class RSAText {
public:
    RSA rsa;

    string encrypt(string str) {
        str = this->appendIfNeeded(str);
        string encryptedText;

        for(int i = 1; i < str.size(); i += 2) {
            int nrToEncrypt = this->lettersToNr( str.substr(i - 1, 2) );
            if(nrToEncrypt == -1) {
                return "INVALID CHARACTERS";
            }
            int nrEncrypted = this->rsa.encrypt(nrToEncrypt);

            encryptedText += this->padByEncryptionLength( this->nrToLetters(nrEncrypted) );
        }

        return encryptedText;
    }

    string decrypt(string str) {
        string decryptedText;

        for(int i = this->rsa.getLengthEncrypted() - 1; i < str.size(); i += this->rsa.getLengthEncrypted()) {
            string substr = str.substr(i - this->rsa.getLengthEncrypted() + 1, this->rsa.getLengthEncrypted());

            int nrToDecrypt = this->lettersToNr( substr );
            int nrDecrypted = this->rsa.decrypt( nrToDecrypt );

            decryptedText += this->appendIfNeeded( this->nrToLetters(nrDecrypted) );
        }

        return decryptedText;
    }

    int lettersToNr(string str) {
        int power = str.size() - 1;
        int nr = 0;

        for(int i = 0; i < str.size(); i++) {
            if(str[i] == ' ') {
            } else {
                if(str[i] < 'a' || str[i] > 'z' ) {
                    return -1;
                }

                nr += pow(27, power)* (str[i] - 'a' + 1);
            }

            power -= 1;
        }

        return nr;
    }

    string nrToLetters(int nr) {
        string txt;

        while(nr) {
            int lastDigit = nr % 27;

            if(lastDigit == 0) {
                txt = ' ' + txt;
            } else {
                char adding = (lastDigit + 'a' - 1);
                txt = adding + txt;
            }

            nr /= 27;
        }

        return txt;
    }

private:
    string padByEncryptionLength(string str) {
        while(str.size() < this->rsa.getLengthEncrypted()) {
            str = " " + str;
        }

        return str;
    }


    string appendIfNeeded(string str) {
        if(str.size() % 2 == 1) {
            str = " " + str;
        }

        return str;
    }
};