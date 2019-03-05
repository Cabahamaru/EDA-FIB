#include <iostream>
#include <vector>
using namespace std;

using VC = vector<char>;
using MC = vector<VC>;

int n, m;
MC M;

int prof(int x, int y)  {
    if (x < 0  or x >= n or y < 0 or y >= m) return 0; //ens sortim de la matriu
    if (M[x][y] == 'X') return 0; //estem en un mur
    int res = 0;
    if (M[x][y] == 't') ++res; //estem en un tresor
    M[x][y] = 'X'; //posem un mur on estem per no tornar-hi a passar
    return res + prof(x+1,y) + prof(x-1,y) + prof(x,y+1) + prof(x,y-1);
    //recorregut implícit: quan una crida retorna true, ja no fa les seguents (lazy).
}

int main() {
    cin >> n >> m;
    M = MC(n, VC(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> M[i][j];
        }
    }
    int x, y;
    cin >> x >> y;
    int n = prof(x-1,y-1);
    cout << n << endl;
}
 
