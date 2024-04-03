#include <iostream>
#include <cmath>
#include <chrono>
#include <iomanip>

using namespace std;

int main() {

    long long n;
    int m;
    cin >> n >> m;

    //  DP algorithm

    auto start = chrono::high_resolution_clock::now();

    long long count = 0, piece = n, round = pow(10, to_string(n).size() - 1);

    while (piece >= m && round > 0) {
        while (round % m) ++round;
        while (piece % m) --piece;
        count += (piece - round) / m + 1;
        piece /= 10;
        round /= 10;
    }
    count -= n % m == 0;

    cout << count << endl;

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end-start;
    cout << "Время выполнения: " << fixed << setprecision(6) << diff.count() << " секунд\n";

    //  naive algorithm

    start = chrono::high_resolution_clock::now();

    count = 0;
    for (long long i = 1; i < n; ++i)
        if (i % m == 0 && to_string(i) < to_string(n))
            ++count;

    cout << count << endl;

    end = chrono::high_resolution_clock::now();
    diff = end-start;
    cout << "Время выполнения: " << fixed << setprecision(6) << diff.count() << " секунд\n";

    return 0;

}
