//
// Created by 15411 on 2025/3/22.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=3e5+7;
const ll INF=(ll)1e18+7;
ll a[N];
bitset<N>vst;
void ask(ll i,ll j){
    cout<<"? "<<i<<" "<<j<<endl;
}
void solve(){
    ll n,x,y;cin>>n;
    for(int i=1;i<=n;i++)vst[i]=false;
    for(int i=1;i<=n;i++){
        cin>>a[i];
        vst[a[i]]=true;
        if(a[i]==1)x=i;
        if(a[i]==n)y=i;
    }
    ll k=0;
    for(int i=1;i<=n;i++){
        if(!vst[i])k=i;
    }
    if(k){//有不存在的k
        ask(k,a[1]);
        ll ans;
        cin>>ans;
        ask(k,a[1]);
        cin>>ans;
        if(ans==0)
            cout<<"! A"<<endl;
        else
            cout<<"! B"<<endl;
    }
    else{
        ll ans1,ans2;
        ask(x,y);//? 1 n
        cin>>ans1;
        ask(y,x);//? n 1
        cin>>ans2;
        if((ans1!=ans2)||(ans1<n-1)||(ans2<n-1))
            cout<<"! A"<<endl;
        else
            cout<<"! B"<<endl;
    }
}

signed main(){
    ios::sync_with_stdio(false);//cin.tie(0);cout.tie(0);
    ll _;cin>>_;while(_--)
        solve();
    return 0;
}