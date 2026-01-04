#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    const int MAXD = 100;
    const int SCALE = 10000; // we use denominator 10000: p = d*100 / 10000, 0.005 = 50/10000
    const int MAXM = 10000; // 搜索 m 的上限（常见可行上限）

    while (T--) {
        int n;
        cin >> n;
        vector<int> d(n);
        vector<int> cnt(MAXD+1, 0);
        for (int i = 0; i < n; ++i) {
            cin >> d[i];
            ++cnt[d[i]];
        }

        bool found = false;
        int found_m = -1;
        // store low/high per d for the found m
        vector<ll> low_for_d(MAXD+1), high_for_d(MAXD+1);

        for (int m = 1; m <= MAXM; ++m) {
            ll sum_low = 0, sum_high = 0;
            bool ok = true;
            for (int val = 0; val <= MAXD; ++val) {
                if (cnt[val] == 0) continue;
                // L numerator (in scale 1/10000): max(0, val*100 - 50)
                int numL = val * 100 - 50;
                if (numL < 0) numL = 0;
                // low = ceil(numL * m / 10000)
                ll low = ( (ll)numL * m + SCALE - 1 ) / SCALE;

                ll high;
                if (val == 100) {
                    // p = 1.0, right endpoint >=1 so x <= 1 allowed -> bi <= m
                    high = m;
                } else {
                    int numR = val * 100 + 50; // < 10000 for val < 100
                    // bi < (numR * m) / 10000  -> bi <= floor((numR*m - 1) / 10000)
                    ll t = (ll)numR * m - 1;
                    if (t < 0) high = -1;
                    else high = t / SCALE;
                }

                if (high < low) { ok = false; break; }

                sum_low += low * (ll)cnt[val];
                sum_high += high * (ll)cnt[val];

                // early pruning: if sum_low already > m or sum_high < 0 or sum_low > 1e18 break
                if (sum_low > (ll)m) { ok = false; break; }
            }
            if (!ok) continue;
            if (sum_low <= (ll)m && (ll)m <= sum_high) {
                // feasible, record low/high per d for this m
                found = true;
                found_m = m;
                for (int val = 0; val <= MAXD; ++val) {
                    if (cnt[val] == 0) { low_for_d[val] = 0; high_for_d[val] = 0; continue; }
                    int numL = val * 100 - 50;
                    if (numL < 0) numL = 0;
                    ll low = ( (ll)numL * m + SCALE - 1 ) / SCALE;
                    ll high;
                    if (val == 100) high = m;
                    else {
                        int numR = val * 100 + 50;
                        ll t = (ll)numR * m - 1;
                        high = (t < 0) ? -1 : t / SCALE;
                    }
                    low_for_d[val] = low;
                    high_for_d[val] = high;
                }
                break;
            }
        }

        if (!found) {
            cout << "No\n";
            continue;
        }

        // 构造具体的 b_i （按原序）
        int m = found_m;
        vector<ll> b(n);
        ll sum_low = 0;
        for (int i = 0; i < n; ++i) {
            b[i] = low_for_d[d[i]];
            sum_low += b[i];
        }
        ll rem = m - sum_low;
        // 逐个从前往后尽量提升到 high
        for (int i = 0; i < n && rem > 0; ++i) {
            ll can = high_for_d[d[i]] - low_for_d[d[i]];
            if (can <= 0) continue;
            ll add = min(can, rem);
            b[i] += add;
            rem -= add;
        }
        // sanity check
        ll total = 0;
        for (int i = 0; i < n; ++i) total += b[i];
        if (total != m) {
            // (理论上不会发生) 保险起见，如果发生则输出 No
            cout << "No\n";
            continue;
        }

        cout << "Yes\n";
        for (int i = 0; i < n; ++i) {
            if (i) cout << ' ';
            cout << b[i];
        }
        cout << '\n';
    }

    return 0;
}
