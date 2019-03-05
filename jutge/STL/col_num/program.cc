#include <queue>
#include <iostream>
using namespace std;

int main() {
    priority_queue<int> pq;
    char s;
    int x;
    while (cin >> s) {
        if (s == 'S') { //afegeix
            cin >> x;
            pq.push(x);
        }
        else if (s == 'A') { //consulta
            if (not pq.empty()) cout << pq.top() << endl;
            else cout << "error!" << endl;
        }
        else if (s == 'R') { //esborra
            if (not pq.empty()) pq.pop();
            else cout << "error!" << endl;
        }
        else if (s == 'I') { //incrementa
            if (not pq.empty()) {
                cin >> x;
                int aux = pq.top();
                pq.pop();
                aux += x;
                pq.push(aux);
            }
            else cout << "error!" << endl;
        }
        else if (s == 'D') { //decrementa
            if (not pq.empty()) {
                cin >> x;
                int aux = pq.top();
                pq.pop();
                aux -= x;
                pq.push(aux);
            }
            else cout << "error!" << endl;
        }
    }
}
