//
// Created by 15411 on 2025/1/10.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
map<string,map<string,bool>>xd;
map<string,ll>cnt;
map<string,bool>vst;
void solve() {
    ll q;cin>>q;
    ll ans4=0;
    while(q--){
        ll op;cin>>op;
        if(op==1||op==3){
            string x,y;cin>>x>>y;
            if(op==1){
                if(!xd[x][y]){
                    xd[x][y]=true;
                    cnt[x]++;
                }
                if(!vst[x]){
                    vst[x]=true;
                    ans4++;
                }
            }
            if(op==3){
                if(!xd[x][y])
                    cout<<0<<endl;
                else
                    cout<<1<<endl;
            }
        }
        if(op==2){
            string x;cin>>x;
            cout<<cnt[x]<<endl;
        }
        if(op==4){
            cout<<ans4<<endl;
        }
    }
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