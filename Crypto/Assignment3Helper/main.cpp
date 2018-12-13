#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int iterations = 13;

int main() {
    long long n = 7819;

    vector<long long> a(iterations, 0);
    vector<long long> b(iterations, 0);
    vector<long long> bmod(iterations, 0);
    vector<float> x(iterations, 0);

    b[0] = static_cast<long long>(floor(sqrt(n)));
    a[0] = b[0];
    x[0] = static_cast<float>(sqrt(n) - a[0]);
    bmod[0] = static_cast<long long>((b[0] * b[0]) % n);

    for(int i = 1; i < iterations; ++i)
    {
        a[i] = static_cast<long long>(floor(1.0 / x[i-1]));
        x[i] = static_cast<float>(1.0 / x[i - 1] - a[i]);
    }

    b[1] = (a[1] * b[0] + 1) % n;

    for(int i = 2; i < iterations; ++i)
    {
        b[i] = (a[i] * b[i-1] + b[i-2]) % n;
    }

    for(int i = 1; i < iterations; ++i)
    {
        bmod[i] = static_cast<int>((b[i] * b[i]) % n);
    }






    for(int i = 0; i < iterations; ++i)
    {
        cout << a[i] << ' ';
    }
    cout << '\n';

    for(int i = 0; i < iterations; ++i)
    {

        cout << b[i] << ' ';
    }
    cout << '\n';
    for(int i = 0; i < iterations; ++i)
    {
        if(bmod[i] > n/2)
        {
            bmod[i] = -n + bmod[i];
        }
        cout << bmod[i] << ' ';
    }
    cout << '\n';


    return 0;
}