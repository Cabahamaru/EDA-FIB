#include <iostream>
#include <vector>
using namespace std;

bool i_resistant_search(double x, const vector<double>& v, int esq, int dre) {
    if (esq > dre) return false;
    int m = (esq+dre)/2;
    double y = v[m];
    if (m != 0 and (y < v[m-1])) y = v[m-1];
    else if (m != (v.size()-1) and (y > v[m+1])) y = v[m+1];

    if (x < y) return i_resistant_search(x,v,esq,m-1);
    if (x > y) return i_resistant_search(x,v,m+1,dre);
    return true;
}

bool resistant_search(double x, const vector<double>& v) {
    int esq = 0;
    int dre = v.size()-1;
    return i_resistant_search(x,v,esq,dre);
}
