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

//返回第一个>=x的位置，如果不存在返回n
int lower_bound(int *a,int n,int t){
    int l=0,r=n;//检测区间左闭右开[l,r)
    while(l<r) {
        int mid = l + (r - l) / 2;
        if (a[mid] < t)
            l = mid + 1;
        else//a[mid]>=target,
            r = mid;
    }
    return l;
}
//[l,r)是待检查区间,r是“已查候选”答案,过程中r不断向“可能答案”靠近，最终l=r全部检查完，l取出r中最终答案

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