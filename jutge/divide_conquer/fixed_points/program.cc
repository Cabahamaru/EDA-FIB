#include <iostream>
#include <vector>
using namespace std;

int fixedpoints(vector<int> &v, int x, int esq, int dre) {
    if (esq > dre) return -1;
    else {
        int mid = (esq+dre)/2;
        if (v[mid]+x == mid+1 and (mid == 0 or mid != v[mid-1] + x)) return mid+1;
        if (v[mid]+x < mid+1) return fixedpoints(v,x,mid+1,dre);
        else return fixedpoints(v,x,esq,mid-1);
    }
}

void llegir_vector(vector<int> &v) {
    int size = v.size();
    for (int i = 0; i < size; ++i) cin >> v[i];
}

int main() {
    int n;
    int count = 0;
    while (cin >> n) {
        cout << "Sequence #" << ++count << endl;
        vector<int> v(n);
        llegir_vector(v);
        int m;
        cin >> m;
        vector<int> s(m);
        llegir_vector(s);
        for (int i = 0; i < m; ++i) {
            int pos = fixedpoints(v,s[i],0,v.size()-1);
            if (pos == -1) cout << "no fixed point for " << s[i] << endl;
            else cout << "fixed point for " << s[i] << ": " << pos << endl;
        }
        cout << endl;
    }
}
