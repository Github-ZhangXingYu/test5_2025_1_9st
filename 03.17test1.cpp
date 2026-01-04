//
// Created by 15411 on 2025/3/17.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=3e5+7;
const ll INF=(ll)1e18+7;
ll suml[N],al[N];
initializer_list<long long int> a;

void solve() {
    ll n,m;cin>>n>>m;
    for(int i=1;i<=m;i++)cin>>a[i];
    sort(a+1,a+m+1);
    suml[m+1]=0;
    ll j=m;
    for(int i=m;i>=1;i--){
        al[i]=max((ll)1,n-a[i]);
        suml[i]=suml[i+1]+al[i];
        if(i!=1&&a[1]+a[j]>=n)j=i;
    }
    ll ans=0;
    for(int i=1;i<=m-1;i++){
        while(j>=i+2&&a[i]+a[j-1]>=n)j--;//i j两种颜色
        if(j<=i)j=i+1;
        ll cnt=m-j+1;//j可以选择的颜色数量
        ll l=suml[j];
        ll r=min(a[i],n-1);
        r=r*cnt;
        ans=ans+(r-l+cnt);
    }
    ans*=2;//颜色前后可以互换
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