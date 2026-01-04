//
// Created by 15411 on 2025/3/22.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
ll a[N];
void solve() {
    ll n;cin>>n;
    ll cnt[(ll)1e3+7]={0};
    for(int i=1,x;i<=n;i++){
        cin>>x;
        cnt[x]++;
    }
    bool op=true;
    for(int i=1;i<=n;i++){
        if(cnt[i]>=2){
            cnt[i+1]+=(cnt[i]-2);
            cnt[i]=2;
        }
        else{
            if(cnt[i]==1){
                op=false;
            }
        }
    }
    if(cnt[n+1]%2)op=false;
    if(op)
        cout<<"Yes"<<endl;
    else
        cout<<"No"<<endl;
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