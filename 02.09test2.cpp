//
// Created by 15411 on 2025/2/9.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
bool cmp(ll A,ll B){
    return A>B;
}
void solve() {
    ll n,m;cin>>n>>m;
    vector<ll>b;
    ll ans=0;
    for(int i=1;i<=n;i++){
        ll ai,sum1=0,sum2=0,now=0;
        for(int j=1;j<=m;j++){
            cin>>ai;
            now+=ai;
            sum1+=now;
            sum2+=ai;
        }
        ans+=sum1;
        b.push_back(sum2);
    }
    sort(b.begin(),b.end(),cmp);
    for(int i=1;i<=n;i++){
        ll sum=b[i-1]*(n-i)*m;
        ans+=sum;
    }
    cout<<ans<<endl;
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