//
// Created by 15411 on 2025/3/17.
//
#include<bits/stdc++.h>

#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N = 3e5 + 7;
const ll INF = (ll) 1e18 + 7;
ll suml[N];
initializer_list<long long int> a;

void solve() {
    ll nn, m;
    cin >> nn >> m;
    for (int i = 1; i <= m; i++)cin >> a[i];
    sort(a + 1, a + m + 1);
    suml[m + 1] = 0;
    ll ans = 0;
    if (a[m-1] + a[m] < nn)ans = 0;
    else {
        for (int i = m; i >= 1; i--) {
            ll l = max((ll) 1, nn - a[i]);
            suml[i] = suml[i + 1] + l;
        }

        for (int i = 1,j=m; i <= m - 1; i++) {
            while (j - 1 >= i + 1 && a[i] + a[j - 1] >= nn)j--;//i j两种颜色
            if (j <= i)j = i + 1;
            if(a[i]+a[j]<nn)continue;
            ll cnt = m - j + 1;//j可以选择的颜色数量
            ll l = suml[j];
            ll r = min(a[i], nn - 1);
            r = r * cnt;
            ans = ans + (r - l + cnt);
        }
    }


    ans *= 2;//颜色前后可以互换
    cout << ans << endl;
}

signed main() {
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    ios::sync_with_stdio(false);//cin.tie(0);cout.tie(0);
    ll _ = 1;
    cin >> _;
    while (_--)
        solve();
    return 0;
}