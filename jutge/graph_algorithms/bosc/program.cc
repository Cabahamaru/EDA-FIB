#include <iostream>
#include <vector>
using namespace std;

using VE = vector<int>;
using ME = vector<VE>;
using VB = vector<bool>;

int n, m;
ME G;
VB vist;

void prof(int x, int& y, int& a) {
    if (vist[x]) return;
    vist[x] = true;
    ++y;
    for (int i = 0; i < (int)G[x].size(); ++i) {
        prof(G[x][i],y,a);
        ++a;
    }
}

int solucio() {
    int res = 0;
        vist = VB(n,false);
        for (int x = 0; x < n; ++x) {
            if (not vist[x]) {
                ++res;
                int v = 0, a = 0;
                prof(x,v,a);
                if (a != 2*(v-1)) return -1;
            }
        }
        return res;
}

int main() {
    while (cin >> n >> m) {
        G = ME(n);
        for (int r = 0; r < m; ++r) {
            int x, y;
            cin >> x >> y;
            G[x].push_back(y);
            G[y].push_back(x);
        }
        int res = solucio();
        if (res == -1) cout << "no" << endl;
        else cout << res << endl;
        }
    }

//g++ -Wall -O2 -std=c++11 -o program.exe program.cc 
