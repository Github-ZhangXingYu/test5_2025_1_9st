//
// Created by 15411 on 2025/7/22.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
typedef struct {
    ll x,y;
}node;
typedef int ccc;
ccc abc;

int* New(int num){//错误，这是栈空间，return时就会被系统回收，里面的数组也就回收了
    int a[num];
    int* it=a;
    for(int i=0;i<num;i++)*(it+i)=i;
    return it;
}
int *New2(int num){//正确，malloc分配的在堆空间，除非主动free，否则不会消失
    int* it=(int*)malloc(num*sizeof(int));
    for(int i=0;i<num;i++)*(it+i)=i;
    return it;
}
void solve(){
    int num;cin>>num;
    int *arr=New(num);
    int *arr2=New2(num);
    for(int i=0;i<num;i++){
        cout<<arr[i]<<" \n"[i==num-1];
    }
    for(int i=0;i<num;i++){
        cout<<arr2[i]<<" \n"[i==num-1];
    }
    free(arr2);
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
