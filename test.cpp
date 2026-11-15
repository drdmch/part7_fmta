#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    for (int i = 0; i < 3; i++)
        cout << i << endl;

    auto sum = [&]() {
        int s = 0;
        for (int x : v)
            s += x;
        return s;
    };

    auto print = [](int n) {
        int i = 0;
        while (i < n) {
            cout << i << " ";
            i++;
        }
        do {
            n--;
        } while (n > 0);
    };

    auto outer = [&]() {
        for (int i = 0; i < 2; i++) {
            auto inner = [](int k) {
                for (int j = 0; j < k; j++)
                    cout << j;
            };
            inner(i);
        }
    };

    auto empty = []() { return 0; };

    cout << sum() << endl;
    print(3);
    outer();
    empty();
    return 0;
}
