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
namespace Tree1{
#define lc p<<1
#define rc (p<<1|1)
    ll F[N];
    struct node{
        ll l,r,sum,max;
    }t[N<<2];
    void push_up(ll p){
        t[p].sum=t[lc].sum+t[rc].sum;
        t[p].max=max(t[lc].max,t[rc].max);
    }
    void build(ll p,ll l,ll r){
        t[p].l=l;t[p].r=r;
        if(l==r){
            t[p].sum=F[l];
            t[p].max=l;
            return;
        }
        ll mid=(l+r)>>1;
        build(lc,l,mid);
        build(rc,mid+1,r);
        push_up(p);
    }
    ll SUM,MAX;
    void query(ll p,ll l,ll r){
        if(l<=t[p].l&&t[p].r<=r){
            SUM+=t[p].sum;
            MAX=max(MAX,t[p].max);
            return;
        }
        ll mid=(t[p].l+t[p].r)>>1;
        if(l<=mid)
            query(lc,l,r);
        if(r>mid)
            query(rc,l,r);
    }
    void Query(ll l,ll r){
        SUM=0;MAX=0;
        query(1,l,r);
    }
    void Init(){
        F[1]=1;F[2]=1;
        for(int i=3;i<N;i++){
            F[i]=F[i-1]+F[i-2];
            F[i]%=MOD;
        }
        build(1,1,N-1);
    }
}
namespace Tree2{
#define lc p<<1
#define rc (p<<1|1)
    struct node{
        ll l,r;
        ll x,sum,max;
        bool flag;
    }t[N<<2];
    void push_up(ll p){
        t[p].x=t[lc].x+t[rc].x;
        t[p].sum=t[lc].sum+t[rc].sum;
        t[p].max=max(t[lc].max,t[rc].max);
    }
    void addtag(ll p){
        t[p].flag=true;
        t[p].x=0;t[p].sum=0;t[p].max=0;
    }
    void push_down(ll p){
        if(t[p].flag){
            addtag(lc);
            addtag(rc);
            t[p].flag=false;
        }
    }
    void build(ll p,ll l,ll r){
        t[p].l=l;t[p].r=r;
        t[p].x=0;t[p].sum=0;t[p].max=0;
        if(l==r)
            return;
        ll mid=(l+r)>>1;
        build(lc,l,mid);
        build(rc,mid+1,r);
        push_up(p);
    }
    void update(ll p,ll l,ll r,ll x){
        if(l<=t[p].l&&t[p].r<=r){
            if(x){
                t[p].x=x;
                Tree1::Query(1,x);
                t[p].sum=Tree1::SUM;
                t[p].max=Tree1::MAX;
            }
            else{
                addtag(p);
            }
            return;
        }
        push_down(p);
        ll mid=(t[p].l+t[p].r)>>1;
        if(l<=mid)
            update(lc,l,r,x);
        if(r>mid)
            update(rc,l,r,x);
        push_up(p);
    }
    ll X,SUM,MAX;
    void query(ll p,ll l,ll r){
        if(l<=t[p].l&&t[p].r<=r){
            X+=t[p].x;
            SUM+=t[p].sum;
            MAX=max(MAX,t[p].max);
            return;
        }
        ll mid=(t[p].l+t[p].r)>>1;
        if(l<=mid)
            query(lc,l,r);
        if(r>mid)
            query(rc,l,r);
    }
    void Query(ll l,ll r){
        X=0;SUM=0;MAX=0;
        query(1,l,r);
    }
    void Init(){
        build(1,1,N-1);
    }
}
initializer_list<long long int> a(N, 0);
ll l=3e5+7,r=3e5+6;
ll SUM,MAX;
ll tow_find(ll x){
    ll L=l,R=r;
    while(L<R){
        ll mid=(L+R)>>1;
        Tree2::Query(l,mid);
        ll cnt=Tree2::X;
        if(cnt<x)
            L=mid+1;
        else
            R=mid;
    }
    return L;
}
void Query(ll x,ll y){
    SUM=0;MAX=0;
    ll xx= tow_find(x);
    ll yy= tow_find(y);
    if(xx+1<=yy-1){
        Tree2::Query(xx+1,yy-1);
        SUM+=Tree2::SUM;
        MAX=max(MAX,Tree2::MAX);
    }
    Tree2::Query(l,xx-1);
    ll cnt_x=Tree2::X;
    Tree2::Query(l,yy-1);
    ll cnt_y=Tree2::X;
    if(xx==yy){
        ll L=x-cnt_x;
        Tree1::Query(L,y-cnt_y);
        SUM+=Tree1::SUM;
        MAX=max(MAX,Tree1::MAX);
        return;
    }
    if(cnt_x<x){
        ll L=x-cnt_x;
        Tree1::Query(L,a[xx]);
        SUM+=Tree1::SUM;
        MAX=max(MAX,Tree1::MAX);
    }
    if(cnt_y<y){
        Tree1::Query(1,y-cnt_y);
        SUM+=Tree1::SUM;
        MAX=max(MAX,Tree1::MAX);
    }
}
void solve() {
    Tree1::Init();
    Tree2::Init();
    ll n;cin>>n;
    while(n--){
        ll op,x,y;cin>>op>>x;
        if(op>3)cin>>y;
        switch (op) {
            case 1:{
                a[++r]=x;
                Tree2::update(1,r,r,x);
                break;
            }
            case 2:{
                a[--l]=x;
                Tree2::update(1,l,l,x);
                break;
            }
            case 3:{
                ll L,R=r;
                while(x){
                    if(a[r]<=x){
                        x-=a[r];
                        a[r]=0;
                        r--;
                    }
                    else{
                        a[r]-=x;
                        x=0;
                    }
                }
                L=r;
                if(L+1<=R)
                    Tree2::update(1,L+1,R,0);
                Tree2::update(1,L,L,a[L]);
                break;
            }
            case 4:{
                Query(x,y);
                cout<<Tree1::F[MAX]<<endl;
                break;
            }
            case 5:{
                Query(x,y);
                cout<<SUM%MOD<<endl;
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
    solve();
    return 0;
}