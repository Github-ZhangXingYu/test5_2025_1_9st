//
// Created by 15411 on 2025/11/15.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;

//返回第一个>=x的位置，即x不存在于a中时，l是第一个>x的位置
int lower_bound(int *a,int n,int x){
    if(n==0)return 0;//特殊情况
    int l=0,r=n-1;
    while(l<r){
        int mid=l+(r-l)/2;
        if(a[mid]>=x)r=mid;
        else l=mid+1;
    }
    if(a[l]<x)l=l+1;//此时l=n-1,,x>a[n-1]，比全部元素都大,应返回n
    return l;
}
void solve() {
    ll a[N];
    ll n,x;cin>>n>>x;
    for(int i=0;i<=n-1;i++)cin>>a[i];
    int id=lower_bound(a,n,x);
    a[n]=INF;
    cout<<id<<" "<<a[id]<<endl;
//    cout<<"  "<<tow_find2(a,n,x)<<endl;
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