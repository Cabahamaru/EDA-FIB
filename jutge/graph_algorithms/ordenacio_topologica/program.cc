#include <iostream>
#include <vector>
#include <stack>
using namespace std;

using VE = vector<int>;
using ME = vector<VE>;

int main() {
    int n, m;
    while (cin >> n >> m) {
        bool primer = true;
        ME G(n);
        VE grau(n,0);
        for (int r = 0; r < m; ++r) {
            int x, y;
            cin >> x >> y;
            G[x].push_back(y);
            ++grau[y];
        }
        stack<int> pila;
        for (int x = 0; x < n; ++x) {
            if (grau[x] == 0) pila.push(x);
            //
            while (not pila.empty()) {
                int x = pila.top(); pila.pop();
                if (primer) {
                    cout << x;
                    primer = false;
                }
                else cout << ' ' << x;
                for (int i = 0; i < (int)G[x].size(); ++i) {
                    int y = G[x][i];
                    if (--grau[y] == 0) pila.push(y);
                }
            }
        }
        cout << endl;
    }
}
//g++ -Wall -O2 -std=c++11 -o program.exe program.cc
