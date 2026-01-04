//
// Created by 15411 on 2025/1/20.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+100;
const ll INF=(ll)1e18+7;
int n,q,x;
char s[N];
stack<pair<ll,ll>>from;

void solve() {
    cin>>n>>q;
    while(q--){
        while(!from.empty())from.pop();
        cin>>x>>s;
        ll now=n/2+1,L=n/2,sum=0;
        while(now!=x){
            from.emplace(sum,now);
            if(x>now)
                sum+=L+1;
            L/=2;
            now=sum+L+1;
        }

        for(int i=0;s[i];i++){
            if(s[i]=='U'){
                if(from.empty())continue;
                sum=from.top().first;
                now=from.top().second;
                from.pop();
                L=L*2+1;
            }
            else{
                if(L==0)continue;
                from.emplace(sum,now);
                if(s[i]=='R')
                    sum+=L+1;
                L/=2;
                now=sum+L+1;
            }
        }
        cout<<now<<endl;
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