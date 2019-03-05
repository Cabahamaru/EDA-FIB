#include <iostream>
#include <vector>
using namespace std;

typedef vector<vector<int> > matrix

int main() {
    matrix m(2, vector<int>(2));
    int n, mod;
}

void mult(const matrix& M1, const matrix& M2, matrix& R, int m) {
    p = matrix(2, vector<int>(2));
    for (int i = 0, i < 2; ++i) {
        for (int j = 0; i < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                p[i][j] += M1[i][k]*M2[k][j];
                p[i][j] %= m;
            }
        }
    }
}
