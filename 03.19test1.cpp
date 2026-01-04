//
// Created by 15411 on 2025/3/19.
//
#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define int long long
#define endl "\n"
const int N=2e5+7;
const ll INF=(ll)9187201950435737471;
ll dp[59][59][59];
ll pow2[65];
void init(){
    memset(dp,0x7F,sizeof(dp));
    dp[1][0][0]=0;
    dp[1][1][0]=2;
    dp[1][0][1]=2;
    pow2[0]=1;
    for(int i=1;i<64;i++)pow2[i]=pow2[i-1]*2;
    for(int k=2;k<=58;k++){
        for(int i=0;i<=58;i++){
            for(int j=0;j<=58;j++){
                dp[k][i][j]=dp[k-1][i][j];
                if(i>=k&&dp[k-1][i-k][j]<INF){
                    dp[k][i][j]=min(dp[k][i][j],dp[k-1][i-k][j]+pow2[k]);
                }
                if(j>=k&&dp[k-1][i][j-k]<INF){
                    dp[k][i][j]=min(dp[k][i][j],dp[k-1][i][j-k]+pow2[k]);
                }
            }
        }
    }
}
void solve(){
    ll x,y;cin>>x>>y;
    ll ans=INF;
    for(int i=0;i<=58;i++){
        ll x_c=x/pow2[i];
        for(int j=0;j<=58;j++){
            ll y_c=y/pow2[j];
            if(x_c==y_c){
                ans=min(ans,dp[58][i][j]);
            }
//            if(y_c==0)break;
        }
//        if(x_c==0)break;
    }
//    cout<<(ll)pow((ll)2,(ll)5);
    cout<<ans<<endl;
}
signed main(){
    ios::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    init();
    int _;cin>>_;while(_--)
        solve();
    return 0;
}