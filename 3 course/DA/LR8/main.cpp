#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <iomanip>

using namespace std;

double area(int a, int b, int c) {
    double p = (a + b + c) / 2.0;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

void naive(vector<int> lengths) {
    double max_area = 0;
    int a, b, c;
    for (int i = 0; i < lengths.size(); ++i) {
        for (int j = i + 1; j < lengths.size(); ++j) {
            for (int k = j + 1; k < lengths.size(); ++k) {
                if (lengths[i] + lengths[j] > lengths[k] &&
                    lengths[i] + lengths[k] > lengths[j] &&
                    lengths[j] + lengths[k] > lengths[i]) {
                    double current_area = area(lengths[i], lengths[j], lengths[k]);
                    if (current_area > max_area) {
                        max_area = current_area;
                        a = lengths[i];
                        b = lengths[j];
                        c = lengths[k];
                    }
                }
            }
        }
    }
    if (max_area == 0) {
        cout << 0 << endl;
    } else {
        vector<int> sides = {a, b, c};
        sort(sides.begin(), sides.end());
        printf("%.3f\n", max_area);
        cout << sides[0] << " " << sides[1] << " " << sides[2] << endl;
    }
}

int main() {

    int n;
    cin >> n;

    vector<int> lengths(n);
    for (int i = 0; i < n; ++i)
        cin >> lengths[i];
    
    auto start = chrono::high_resolution_clock::now();

    naive(lengths);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    cout << "Время выполнения: " << fixed << setprecision(6) << diff.count() << " секунд\n";

    start = chrono::high_resolution_clock::now();

    sort(lengths.begin(), lengths.end());

    int a, b, c, max_a, max_b, max_c;
    double p, s, max_s = 0;
    for (int i = n - 1; i >= 2; --i) {
        if (lengths[i] < lengths[i - 1] + lengths[i - 2]) {
            a = lengths[i];
            b = lengths[i - 1];
            c = lengths[i - 2];
            p = (a + b + c) / 2.0;
            s = sqrt(p * (p - a) * (p - b) * (p - c));
            if (s > max_s) {
                max_s = s;
                max_a = a;
                max_b = b;
                max_c = c;
            }
        }
    }

    if (!max_s) {
        cout << 0 << endl;
        return 0;
    }

    printf("%.3f\n", max_s);
    cout << max_c << ' ' << max_b << ' ' << max_a << endl;

    end = chrono::high_resolution_clock::now();
    diff = end - start;
    cout << "Время выполнения: " << fixed << setprecision(6) << diff.count() << " секунд\n";

    return 0;

}
