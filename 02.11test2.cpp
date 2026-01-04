//
// Created by 15411 on 2025/2/11.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
ll cnt[N],cnt_p[N];
initializer_list<long long int> a;

void solve() {
    ll n;cin>>n;
    for(int i=1;i<=n;i++)cnt[i]=0,cnt_p[i]=0;
    for(int i=1;i<=n;i++){
        cin>>a[i];
        cnt[a[i]]++;
    }
    ll ans=0,sum=0,ca=0;
    set<ll>prime;
    for(int i=1,x,cnt_r,p1,p2;i<=n;i++){
        x=a[i];
        cnt_r=0;//因子个数
        p1=0;
        p2=0;
        for(int j=2;j*j<=x;j++){
            if(x%j==0){
                while(x%j==0){
                    cnt_r++;
                    if(p1)p2=j;
                    else p1=j;
                    x/=j;
                }
            }
        }
        if(x!=1){
            cnt_r++;
            if(p1)p2=x;
            else p1=x;
            x=1;
        }
        if(cnt_r==1){
            sum++;
            cnt_p[a[i]]++;
            prime.insert(a[i]);
        }
        if(cnt_r==2){
            ans+=cnt[1]+cnt[p1]+cnt[p2]+cnt[a[i]];
            if(p1==p2)ans-=cnt[p1];
            ca+=cnt[a[i]]-1;
        }
    }
    ll ans2=0;
    for(auto p:prime){
        ans2=ans2+cnt_p[p]*(sum-cnt_p[p]);
    }
    ans=ans+ans2/2-ca/2;
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