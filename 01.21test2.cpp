//
// Created by 15411 on 2025/1/21.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
initializer_list<long long int> a;
void solve() {
    ll n,l,r;cin>>n>>l>>r;
    for(int i=1;i<=n;i++)cin>>a[i];
    if(l>1)
        sort(a+1,a+l);
    sort(a+l,a+r+1);
    if(r<n)
        sort(a+r+1,a+n+1);
    ll sum=0;
    for(int i=l;i<=r;i++)sum+=a[i];
    ll st1=1,st2=l,st3=r+1;
    ll suml=0,sumr=0;
    for(int i=1;i<=r-l+1;i++){
        if(st1<=l-1&&a[st1]<a[st2]){
            suml+=a[st1];
            st1++;
        }
        else{
            suml+=a[st2];
            st2++;
        }
    }
    st2=l;
    for(int i=1;i<=r-l+1;i++){
        if(st3<=n&&a[st3]<a[st2]){
            sumr+=a[st3];
            st3++;
        }
        else{
            sumr+=a[st2];
            st2++;
        }
    }
    sum=min(sum,suml);
    sum=min(sum,sumr);
    cout<<sum<<endl;
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