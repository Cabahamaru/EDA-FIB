#include <iostream>
using namespace std;

int exp(int n, int k, int m) {
    if (k == 0) return 1;
    int y = exp(n,k/2,m);
    y = (y*y)%m;
    if (k%2 == 1) y = (y*n)%m;
    return y;
}

int main() {
    int n, k, m;
    while (cin >> n >> k >> m) {
        cout << exp(n,k,m) << endl;
    }
}
