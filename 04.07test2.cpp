//
// Created by 15411 on 2025/4/7.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
ll Next[N];
//对于Next数组进行整体后移，把i的Next值放在Next[i+1]，这样方便回退
void getNext(char *p,ll p_len){
    Next[0]=0,Next[1]=0;//初始化
    for(int i=1;i<p_len;i++){        //计算当前i的Next值
        int j=Next[i];//Next[i]是i-1的前后缀匹配长度
        while(j&&p[i]!=p[j])       //匹配失败，则回退j,直至0位
            j=Next[j];
        if(p[i]==p[j])  Next[i+1]=j+1;//匹配成功，i的Next值放入i+1
        else            Next[i+1]=0;//匹配失败，则是0
    }
    //Next[p_len]=0;
}
char s[N];
void solve() {
    cin>>s;
    getNext(s,strlen(s));
    for(int i=0;i<strlen(s);i++){
        cout<<s[i]<<" \n"[i==strlen(s)-1];
    }
    for(int i=0;i<strlen(s);i++){
        cout<<Next[i]<<" \n"[i==strlen(s)-1];
    }
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