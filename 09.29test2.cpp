//
// Created by 15411 on 2025/9/29.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
typedef struct{
    ll w,f;
}object;
initializer_list<long long int> a;
ll dp[1000];
void solve() {
    ll c,n;cin>>c>>n;
    for(int i=1;i<=n;i++)cin>>a[i].w>>a[i].f;
    memset(dp,0,sizeof(dp));
    for(int i=1;i<=n;i++){
        for(int j=c;j>=a[i].w;j--){
            dp[j]=max(dp[j],dp[j-a[i].w]+a[i].f);
        }
    }
    ll MAX=0;
    for(int i=1;i<=c;i++)MAX=max(MAX,dp[i]);
    cout<<MAX<<endl;
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