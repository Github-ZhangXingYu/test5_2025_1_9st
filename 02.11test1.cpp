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
bool vst[N];
map<ll,ll>m[N];
vector<ll>G[N];
void solve() {
    ll n;cin>>n;
    auto clear=[&n](){
        for(int i=1;i<=n;i++){
            vst[i]=false;
            G[i].clear();
            m[i].clear();
        }
    };
    clear();
    for(int i=1;i<=n;i++){
        cin>>a[i];
        m[i][a[i]]++;
    }
    for(int i=1,u,v;i<=n-1;i++){
        cin>>u>>v;
        G[u].push_back(v);
        G[v].push_back(u);
        m[u][a[v]]++;
        m[v][a[u]]++;
    }
    string s;
    for(int u=1;u<=n;u++){
        for(auto v:G[u]){
            if(m[v][a[u]]>=2){
                vst[a[u]]=true;
                break;
            }
        }
    }
    for(int i=1;i<=n;i++){
        if(vst[i])s+='1';
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