#include <iostream>
#include <vector>
using namespace std;

int i_first_occurrence(double x, const vector<double>& v, int esq, int dre) {
    if (esq > dre) return -1;
    int m = (esq+dre)/2;
    if (x < v[m]) return i_first_occurrence(x,v,esq,m-1);
    if (x > v[m]) return i_first_occurrence(x,v,m+1,dre);
    if (m != 0) {
        if (v[m-1] == v[m]) return i_first_occurrence(x,v,esq,m-1);
        else return m;
    }
    else return m;
}

int first_occurrence(double x, const vector<double>& v) {
    int esq = 0;
    int dre = v.size() - 1;
    int pos = i_first_occurrence(x,v,esq,dre);
    return pos;
}

