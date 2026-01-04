//
// Created by 15411 on 2025/10/29.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl "\n"
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
int R[N];//记录当前左括号对应的右括号位置
ll Sum(int l,int r){//返回字符串s[l]-s[r]的和
    if(l+1==r)return 1;//"()情况"
    if(R[l]==r){//"(....)情况"
        ll sum=2*Sum(l+1,r-1);
        return sum;
    }
    //"(....)(....)情况"
    ll sum1=Sum(l,R[l]);
    ll sum2=Sum(R[l]+1,r);
    return sum1+sum2;
}
void solve() {
    string s;cin>>s;
    stack<int>st;
    int len=(int)s.size();
    for(int i=0;i<len;i++){
        if(s[i]=='(')st.push(i);
        else{
            int L=st.top();
            st.pop();
            R[L]=i;
        }
    }
    cout<<Sum(0,len-1)<<endl;
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