//
// Created by 15411 on 2025/9/29.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
void solve() {
    int a,b;
    cin>>a>>b;
    int day=0;
    if(b>a){
        if(b-a>25){
            day=(b-a)/25*7;
            b=a+(b-a)%25;
        }
        if(b-a>20)day+=7;
        else if(b-a>15)day+=6;
        else day+=(b-a)/3+((b-a)%3!=0);
    }
    cout<<day<<endl;
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