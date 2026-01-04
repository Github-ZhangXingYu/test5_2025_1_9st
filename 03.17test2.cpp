//
// Created by 15411 on 2025/3/17.
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
    ll n,k;cin>>n>>k;
    ll sumk=0,sumk1=0;
    priority_queue<ll,vector<ll>,greater<>>pq_k,pq_k1;
    for(int i=1;i<=n;i++){
        cin>>a[i];
        if(i<=k-1){
            pq_k.push(a[i]);
            sumk+=a[i];
        }
        if(i<=k){
            pq_k1.push(a[i]);
            sumk1+=a[i];
        }
    }
    ll ans=sumk+a[k];
    if(k!=1){

        pq_k.push(a[k]);
        sumk+=a[k];
        sumk-=pq_k.top();
        pq_k.pop();//前k个里的k-1个

        for(int r=k+1;r<=n;r++){
            if(r<=n-1)
                ans=max(ans,max(sumk1+a[r],sumk+a[r]+a[n]));
            else
                ans=max(ans,sumk1+a[r]);
            pq_k.push(a[r]);
            sumk+=a[r];
            sumk-=pq_k.top();
            pq_k.pop();//k-1个

            pq_k1.push(a[r]);
            sumk1+=a[r];
            sumk1-=pq_k1.top();
            pq_k1.pop();
        }
    }
    else{
        ll MAX=0;
        for(int i=2;i<=n-1;i++)MAX=max(MAX,a[i]);
        ans=MAX+max(a[1],a[n]);
        ans=max(ans,a[1]+a[n]);
    }
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