//
// Created by 15411 on 2025/11/1.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e6+7;
const ll INF=(ll)1e18+7;
typedef struct{
    ll st,ed;
}node;
node bs[N];
void solve() {
    ll n;cin>>n;
    for(int i=1;i<=n;i++)cin>>bs[i].st>>bs[i].ed;
    auto cmp=[](node A,node B){
        return A.ed<B.ed;
    };
    sort(bs+1,bs+n+1,cmp);
    int lastEnd=0,ans=0;
    for(int i=1;i<=n;i++){
//        cout<<bs[i].st<<" "<<bs[i].ed<<endl;
        if(bs[i].st>=lastEnd){
            ans++;
            lastEnd=bs[i].ed;
        }
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
//    ll _=1;cin>>_;while(_--)
        solve();
    return 0;
}