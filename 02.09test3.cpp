//
// Created by 15411 on 2025/2/11.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=5e5+7;
const ll INF=(ll)1e18+7;
initializer_list<long long int> a;
vector<ll>B[N];
vector<ll>G[N];
ll dfs(ll now,ll from,ll cnt,ll flag){
    ll ret=(a[now]==flag?1:0);
    if(cnt)
        for(auto next:G[now])
            if(next!=from)ret+=dfs(next,now,cnt-1,flag);
    return ret;
}
void solve() {
    ll n;cin>>n;
    auto clear=[&n](){
        for(int i=1;i<=n;i++){
            G[i].clear();
            B[i].clear();
        }
    };
    clear();
    for(int i=1;i<=n;i++){
        cin>>a[i];
        B[a[i]].push_back(i);
    }
    ll u,v;
    for(int i=1;i<=n-1;i++){
        cin>>u>>v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    string s;
    for(int i=1;i<=n;i++){
        bool op=false;
        for(auto p:B[i]){
            if(dfs(p,p,2,i)>=2){
                op=true;
                break;
            }
        }
        if(op)s+='1';
        else s+='0';
    }
    cout<<s<<endl;
}

signed main(){
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    ll _=1;cin>>_;while(_--)
        solve();
    return 0;
}