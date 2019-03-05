#include <iostream>
#include <vector>
using namespace std;

using VC = vector<char>;
using MC = vector<VC>;

int n,m;
MC M;

bool prof(int x, int y) {
	if(x < 0 or x >= n or y < 0 or y >= m) return false; 
	if(M[x][y] == 'X') return false;
	if(M[x][y] == 't') return true;
	M[x][y] = 'X' ;
	return prof(x+1,y) or prof(x-1,y) or prof(x,y+1) or prof(x,y-1);

}

int main() {
	cin >> n >> m;

	M = MC(n, VC(m));

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) cin >> M[i][j];
	int x, y;
	cin >> x >> y;

	cout << (prof(x-1,y-1) ? "yes" : "no") << endl;
}
