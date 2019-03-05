#include <map>
#include <iostream>
#include <string>
using namespace std;

int main() {
    map<string, int> casino;
    string nom, accio;
    while (cin >> nom >> accio) {
        auto it = casino.find(nom);
        if (it == casino.end()) cout << nom << " is not in the casino" << endl;
        
    }
}
