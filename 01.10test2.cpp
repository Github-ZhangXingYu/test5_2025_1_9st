//
// Created by 15411 on 2025/1/10.
//
#include<bits/stdc++.h>

#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N = 2e5 + 7;
const ll INF = (ll) 1e18 + 7;

ll ksm(ll x, ll y, ll p) {
    ll ret = 1;
    while (y) {
        if (y & 1)ret = (ret * x) % p;
        x = (x * x) % p;
        y >>= 1;
    }
    return ret;
}

void solve() {
    string s;
    ll p;
    cin >> s >> p;
    if(s.size()>7){
        cout<<0<<endl;
        return;
    }
    ll n=0;
    for(int i=0;i<s.size();i++){
        n*=10;
        n+=s[i]-'0';
    }
    ll ans=1;
    for(int i=1;i<=n;i++){
        ans*=i;
        ans%=p;
    }
    cout<<ans<<endl;
}

signed main() {
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    ios::sync_with_stdio(false);//cin.tie(0);cout.tie(0);
//    ll _=1;cin>>_;while(_--)
    solve();
    return 0;
}