#include <iostream>
#include <vector>

using namespace std;

constexpr size_t NumFingers = 5;

int main(int argc, char *argv[]) {
    int n;
    cin >> n;

    vector<size_t> a;
    for (size_t i = 0; i < n; ++i) {
        cin >> a[i];
    }

    bool convenient = true;
    vector<size_t> fingering {n};
    fingering[0] = 1;
    for (size_t i = 1; i < n; ++i) {
        if (fingering[i] < 1 || fingering[i] > NumFingers) {
            convenient = false;
            break;
        }
        if (a[i - 1] < a[i]) {
            fingering[i] = fingering[i - 1] + 1;
        } else if (a[i - 1] > a[i]) {
            fingering[i] = fingering[i - 1] - 1;
        } else { // a[i - 1] == a[i]
            
        }
    }

    if (convenient) {
        for (size_t i = 0; i < n; ++i) {
            cout << fingering[i] << " ";
        }
        cout << endl;
    } else {
        cout << -1 << endl;
    }

    return 0;
}
