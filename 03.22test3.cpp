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
bool have7(ll x){
    while(x){
        if(x%10==7)return true;
        x/=10;
    }
    return false;
}
ll get9(ll len){
    ll ret=0;
    while(len--){
        ret*=10;
        ret+=9;
    }
    return ret;
}
void solve() {
    ll x;cin>>x;
    ll ans=9;
    for(int i=1;i<=11;i++){
        for(int j=0;j<=10;j++){
            for(int k=0;k<=10;k++){
                ll now=x+j*get9(i)+k*get9(i-1);
                if(have7(now)){
                    ans=min(ans,j+k);
                }
            }
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
    ll _=1;cin>>_;while(_--)
        solve();
    return 0;
}