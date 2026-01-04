//
// Created by 15411 on 2025/1/10.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=6e5+7;
const ll INF=(ll)1e18+7;
const ll MOD=998244353;
ll F[N];
initializer_list<long long int> a(N, 0);
vector<ll>b(N,0);
void solve() {
    F[1]=1;F[2]=1;
    for(int i=3;i<N;i++){
        F[i]=F[i-1]+F[i-2];
        F[i]%=MOD;
    }
    ll n;cin>>n;
    ll l=3e5+7,r=3e5+6;
    while(n--){
        ll op,x,y;cin>>op>>x;
        if(op>3)cin>>y;
        switch (op) {
            case 1:{
                for(int i=1;i<=x;i++){
                    a[++r]=F[i];
                    b[r]=i;
                }
                break;
            }
            case 2:{
                l=l-x;
                for(int i=1;i<=x;i++){
                    a[l+i-1]=F[i];
                    b[l+i-1]=i;
                }
                break;
            }
            case 3:{
                while(x--){
                    a[r]=0;
                    b[r]=0;
                    r--;
                }
                break;
            }
            case 4:{
                ll MAX=0;
                for(int i=x;i<=y;i++){
                    MAX=max(MAX,b[l+i-1]);
                }
                cout<<F[MAX]<<endl;
                break;
            }
            case 5:{
                ll SUM=0;
                for(int i=x;i<=y;i++){
                    SUM+=a[l+i-1];
                }
                cout<<SUM<<endl;
                break;
            }
        }
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
/*9
1 3
1 2
3 3
2 6
5 1 6
4 1 6
3 3
5 1 4
5 1 5*/