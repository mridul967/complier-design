#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, a;
    cout << "Enter number of elements: ";
    cin >> n;
    cout << "Enter value of a: ";
    cin >> a;

    if (n <= 0) {
        cout << "Number of elements must be positive.\n";
        return 1;
    }

    vector<int> b(n), originalY(n), optimizedY(n);
    cout << "Enter " << n << " elements of array b: ";
    for (int i = 0; i < n; ++i) cin >> b[i];

    int originalMultiplications = 0;
    for (int i = 0; i < n; ++i) {
        int x = 10 * a;
        ++originalMultiplications;
        originalY[i] = x + b[i];
    }

    int x = 10 * a; // Loop-invariant expression moved outside the loop.
    int optimizedMultiplications = 1;
    for (int i = 0; i < n; ++i) {
        optimizedY[i] = x + b[i];
    }

    cout << "\nOriginal loop:\n";
    cout << "for (i = 0; i < n; i++) {\n";
    cout << "    x = 10 * a;\n";
    cout << "    y[i] = x + b[i];\n";
    cout << "}\n";

    cout << "\nOptimized loop after LICM:\n";
    cout << "x = 10 * a;  // moved outside the loop\n";
    cout << "for (i = 0; i < n; i++) {\n";
    cout << "    y[i] = x + b[i];\n";
    cout << "}\n";

    cout << "\nOriginal y:  ";
    for (int value : originalY) cout << value << ' ';
    cout << "\nOptimized y:  ";
    for (int value : optimizedY) cout << value << ' ';

    cout << "\n\nMultiplications before optimization: " << originalMultiplications;
    cout << "\nMultiplications after optimization:  " << optimizedMultiplications;
    if (originalY == optimizedY)
        cout << "\nResult: Both versions produce identical output.\n";
    else
        cout << "\nResult: Output mismatch.\n";

    return 0;
}
