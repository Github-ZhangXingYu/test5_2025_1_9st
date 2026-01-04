#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// 检查给定的 b 数组是否满足条件
bool check(const vector<int>& b, const vector<int>& d) {
    int m = 0;
    for (int x : b) m += x;
    if (m == 0) return false;

    for (int i = 0; i < b.size(); i++) {
        double actual = 100.0 * b[i] / m;
        double rounded = round(actual * 100) / 100;  // 四舍五入到两位小数

        // 转换为整数百分比进行比较
        int actual_percent = round(actual);
        if (actual_percent != d[i]) {
            return false;
        }
    }
    return true;
}

void solve() {
    int n;
    cin >> n;
    vector<int> d(n);
    for (int i = 0; i < n; i++) {
        cin >> d[i];
    }

    // 检查特殊情况：所有百分比都是0
    bool all_zero = true;
    for (int x : d) {
        if (x != 0) {
            all_zero = false;
            break;
        }
    }

    if (all_zero) {
        cout << "No\n";
        return;
    }

    // 尝试不同的总画作数 m
    for (int m = 1; m <= 10000; m++) {
        vector<int> b(n);
        bool valid = true;

        // 计算每个 b_i
        for (int i = 0; i < n; i++) {
            // 根据百分比计算 b_i 的范围
            double min_val = m * (d[i] - 0.5) / 100.0;
            double max_val = m * (d[i] + 0.5) / 100.0;

            // 找到满足条件的整数 b_i
            int candidate = -1;
            for (int possible = ceil(min_val); possible < max_val; possible++) {
                if (possible >= 0) {
                    candidate = possible;
                    break;
                }
            }

            if (candidate == -1) {
                valid = false;
                break;
            }
            b[i] = candidate;
        }

        // 检查总和是否为 m
        int sum_b = 0;
        for (int x : b) sum_b += x;

        if (valid && sum_b == m && check(b, d)) {
            cout << "Yes\n";
            for (int i = 0; i < n; i++) {
                cout << b[i] << (i == n - 1 ? "\n" : " ");
            }
            return;
        }
    }

    // 如果没找到解，尝试直接构造解
    // 方法：让 b_i = d_i * k，其中 k 是一个适当的倍数
    for (int k = 1; k <= 10000; k++) {
        vector<int> b(n);
        bool valid = true;
        int sum_b = 0;

        for (int i = 0; i < n; i++) {
            b[i] = d[i] * k;
            sum_b += b[i];
        }

        if (sum_b > 0 && check(b, d)) {
            cout << "Yes\n";
            for (int i = 0; i < n; i++) {
                cout << b[i] << (i == n - 1 ? "\n" : " ");
            }
            return;
        }
    }

    cout << "No\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        solve();
    }

    return 0;
}