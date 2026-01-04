//
// Created by 15411 on 2025/1/17.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e2+7;
const ll INF=(ll)1e18+7;
ll n,m,Ex,Ey,k;
ll d[][2]={{0,1},{1,0},{0,-1},{-1,0}};
bool IsOk(ll x,ll y){
    if(x<1||x>n||y<1||y>m)return false;
    return true;
}
ll BFS(auto &G){
    queue<pair<ll,pair<ll,ll>>>q;
    q.push({1,{1,0}});
    while(!q.empty()){
        pair<ll,pair<ll,ll>>now=q.front();
        q.pop();
        G[now.first][now.second.first]=1;
        if(now.first==Ex&&now.second.first==Ey)
            return now.second.second;
        pair<ll,pair<ll,ll>>next=now;
        next.second.second++;
        for(auto & i : d){
            next.first=now.first+i[0];
            next.second.first=now.second.first+i[1];
            if(!IsOk(next.first,next.second.first))continue;
            if(G[next.first][next.second.first])continue;
            q.push(next);
            G[next.first][next.second.first]=1;
        }
    }
    return -1;
}
void solve() {
    cin>>n>>m>>Ex>>Ey>>k;
    vector<vector<ll>>G(n+4,vector<ll>(m+4,0));
    while(k--){
        ll x,y;cin>>x>>y;
        G[x][y]=-1;
        G[x][y+1]=-1;G[x+1][y]=-1;
        G[x][y-1]=-1;G[x-1][y]=-1;
    }
    ll ans=BFS(G);
    if(ans==-1)
        cout<<"Oops"<<endl;
    else
        cout<<ans<<endl;
}

signed main(){
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