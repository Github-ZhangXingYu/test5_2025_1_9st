//
// Created by 15411 on 2025/1/21.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
vector<ll>G[N];
ll cnt[N];
bool vst[N];
void solve() {
    ll n;cin>>n;
    for(int i=0;i<=n+2;i++){
        G[i].clear();
        cnt[i]=0;
        vst[i]=false;
    }
    for(int i=1;i<=n-1;i++){
        ll u,v;
        cin>>u>>v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    ll id=1;
    for(int i=1;i<=n;i++){
        cnt[i]=(ll)G[i].size();
        if(cnt[i]>cnt[id])id=i;
    }
    ll ans=1;
    ans=ans+cnt[id]-1;
    cnt[id]=0;
    for(auto v:G[id]){
        cnt[v]--;
    }

    id=1;
    for(int i=1;i<=n;i++){
        if(cnt[i]>cnt[id])id=i;
    }
    ans=ans+cnt[id]-1;

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