#include <iostream>

using namespace std;

int main() {
    int input;
    cin >> input;

    if (input > 2 && input % 2 == 0) cout << "YES" << endl;
    else cout << "NO" << endl;

    return 0;
}
