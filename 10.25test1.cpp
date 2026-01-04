//
// Created by 15411 on 2025/10/25.
//没有完成
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
const ll MOD=1e9+7;
ll f(ll k,ll m){
    if(m<=0)return 1;
    ll res=0;
    for(int j=1;j<=min(k,m);j++){
        res+=(k-j+1)*f(j,m-j);
        res%=MOD;
    }
    return res;
}
ll a(ll n){
    ll res=0;
    for(int k=1;k<=n;k++){
        res+=f(k,n-k);
        res%=MOD;
    }
    return res;
}
void solve() {
    ll n;cin>>n;
    cout<<a(n)<<endl;
}

signed main(){
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    ios::sync_with_stdio(false);//cin.tie(0);cout.tie(0);
    ll _=1;cin>>_;while(_--)
        solve();
    return 0;
}