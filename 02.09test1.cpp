//
// Created by 15411 on 2025/2/9.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e10+7;
initializer_list<long long int> a;
void solve() {
    ll n,m,k;cin>>n>>m>>k;
    string s;
    ll sum=n+m;
    if(abs(n-m)<=k&&max(n,m)>=k){
        bool op=false;
        if(m>n){
            op=true;
            ll t=n;n=m;m=t;
        }
        auto push=[&s](ll num,char c){
            for(int i=1;i<=num;i++){
                s+=c;
            }
        };
        while(n>0){
            push(min(n,k),'0');
            n-=min(n,k);
            if(m>0){
                push(min(m,k),'1');
                m-=min(m,k);
            }
        }
        if(op){
            for(int i=0;i<sum;i++){
                if(s[i]=='0')s[i]='1';
                else s[i]='0';
            }
        }
    }
    else
        s="-1";
    cout<<s<<endl;
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