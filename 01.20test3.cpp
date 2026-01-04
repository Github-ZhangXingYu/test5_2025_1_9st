//
// Created by 15411 on 2025/1/20.
//
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<map>
#include<cstring>
#include<map>
#include<utility>
#include<vector>
#include<set>
#include<queue>
#include<cmath>
#include<stack>
#include "iomanip"
#include<unordered_map>
using namespace std;
#define int long long
const int N = 2e5 + 5, mod = 1e9 + 7;
typedef pair<int, int> pll;
int n, m, k,d,u,v,w,x,x2;
string s1;
int a1,a2,a4,a5;
int lowbit(int x){
    return x & -x;
}
void solve(){
    cin >> n >> k;
    while(k --){
        cin >> x;
        cin >> s1;
//        cout << s1 << '\n';
        for(int i = 0;i < s1.size();i ++){
            int now = lowbit(x);
            if(s1[i] == 'U'){
                if(x == (n + 1) / 2) continue;
                x ^= now;
                x |= (now << 1);
            }
            if(s1[i] == 'R'){
                if(x & 1) continue;
                x |= (now) >> 1;
            }
            if(s1[i] == 'L'){
                if(x & 1) continue;
                x ^= now;
                x |= (now >> 1);
            }
//            cout << x << '\n';
        }
        cout << x << '\n';
    }
}
signed main() {
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    int tt = 1;
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
//    cin  >> tt;
    while (tt--) {
        solve();
    }
    return 0;
}