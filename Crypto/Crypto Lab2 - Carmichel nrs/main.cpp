#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

long long gcd(long long a, long long b) {
    long long c;
    while(b) {
        c = a % b;
        a = b;
        b = c;
    }

    return a;
}

long long moduloPow(long long base, long long power, long long modulo) {
    long long last = base % modulo;
    long long crt = base % modulo;
    long long res = 1;

    while(power) {
        int crtBit = power & 1;

        if(crtBit) {
            res = ( res * crt ) % modulo;
        }

        crt = (last * last) % modulo;
        last = crt;
        power >>= 1;
    }

    return res;
}

bool isPrime(long long nr) {
    if(nr <= 1 || nr % 2 == 0 || nr % 3 == 0) return false;

    int sqroot = sqrt(nr);
    for(int i = 5; i <= sqroot; ++i) {
        if(nr % i == 0)
            return false;
    }

    return true;
}

vector<long long> getCarmichael(long long bound) {
    vector<long long> carmichaelNrs;

    for(int n = 10; n <= bound; ++n) {
        if(isPrime(n)) {
            continue;
        }

        bool carmichaelNr = true;
        for(int b = 1; b < n; ++b) {
            if(gcd(n, b) == 1) {
                if(moduloPow(b, n - 1, n) != 1) {
                    carmichaelNr = false;
                    break;
                }
            }
        }

        if(carmichaelNr == true) {
            carmichaelNrs.push_back(n);
        }
    }

    return carmichaelNrs;
}

int main() {
    auto res = getCarmichael(100000);
    for(auto nr: res) {
        cout << nr << " ";
    }

    cout << "\n";
    cout << res.size() << "\n";
    return 0;
}