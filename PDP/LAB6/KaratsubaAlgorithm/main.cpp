#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <thread>

using namespace std;

const int MAX_THREADS = 10;
int T = 0;

void mult(const vector <int> &a, const vector <int> &b, vector <int> &sol) {
  if(a.size() == 1 && b.size() == 1) {
    sol[0] = a[0] * b[0];
    return;
  }
  int half = a.size() / 2 + a.size() % 2;

  vector <int> a_lo(a.begin(), a.begin() + half);
  vector <int> a_hi(a.begin() + half, a.end());
  vector <int> b_lo(b.begin(), b.begin() + half);
  vector <int> b_hi(b.begin() + half, b.end());
  vector <int> lo(a_lo.size() + b_lo.size() - 1);
  vector <int> hi(a_hi.size() + b_hi.size() - 1);
  vector <thread> th;
  T += 2;

  if (T < MAX_THREADS) {
    th.push_back(thread([&a_lo, &b_lo, &lo](){mult(a_lo, b_lo, lo);}));
    th.push_back(thread([&a_hi, &b_hi, &hi](){mult(a_hi, b_hi, hi);}));
  } else {
    mult(a_lo, b_lo, lo);
    mult(a_hi, b_hi, hi);
  }

  vector <int> aLow(a_lo);
  vector <int> bLow(b_lo);
  for(int i = 0; i < a_hi.size(); ++ i) {
    aLow[i] += a_hi[i];
    bLow[i] += b_hi[i];
  }

  vector <int> mid(aLow.size() + bLow.size() - 1);
  T += 1;
  if (T < MAX_THREADS) {
    th.push_back(thread([&aLow, &bLow, &mid](){mult(aLow, bLow, mid);}));
  } else {
    mult(aLow, bLow, mid);
  }

  // Wait for threads to finish
  T -= th.size();
  for(int i = 0; i < th.size(); ++ i) {
    th[i].join();
  }

  // Copy
  for(int i = 0; i < lo.size(); ++ i) {
    sol[i] += lo[i];
  }
  for(int i = 0; i < mid.size(); ++ i) {
    sol[i + half] += mid[i] - lo[i] - hi[i];
  }
  for(int i = 0; i < hi.size(); ++ i) {
    sol[i + 2 * half] += hi[i];
  }
}

int main(int argc, char* argv[]) {
  clock_t t;
  t = clock();

  vector<int> v1;
  vector<int> v2;

  int n = 10;
  for(int i = 1; i <= n; i++) {
    v1.push_back(i % 100);
    v2.push_back( (n - i) % 100);
  }

    for(auto i: v1) {
        cout << i;
    }
    cout << "\n";

    for(auto i: v2) {
        cout << i;
    }
    cout << "\n";


    vector <int> sol(2 * v1.size() - 1);
  mult(v1, v2, sol);
  for(auto elem: sol) {
      cout << elem << "";
  } cout << "\n";
  t = clock() - t;
  cout << static_cast<float> (t) / CLOCKS_PER_SEC << " seconds\n";
}