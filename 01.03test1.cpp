//
// Created by 15411 on 2026/1/3.
//
#include<bits/stdc++.h>
#define ll long long
#define int long long
#define endl '\n'
using namespace std;
const int N=2e5+7;
const ll INF=(ll)1e18+7;
void solve(int _){
    int len=0;cin>>len;
    string s;cin>>s;
    len=(int)s.size();
    string target_s0="2025";
    string target_s1="2026";
    int idx0=0,idx1=0;
    for(int i=0;i<len&&idx1<4;i++){
        if(idx0<4&&s[i]==target_s0[idx0])idx0++;
        if(s[i]==target_s1[idx1])idx1++;
    }
    bool vis_2025= (idx0 >= 4);
    bool vis_2026= (idx1 >= 4);
    if(vis_2025&&!vis_2026)cout<<1<<endl;
    else cout<<0<<endl;
}
void solve(){
    int next[4]={-1,0,0,1};
    string targetString2025="2025",targetString2026="2026";
    int len;cin>>len;
    string s;cin>>s;
    bool visString2025=false,visString2026=false;
    for(int i=0,idx5=0,idx6=0;i<len;i++){
        while(idx5!=-1&&s[i]!=targetString2025[idx5])idx5=next[idx5];
        while(idx6!=-1&&s[i]!=targetString2026[idx6])idx6=next[idx6];
        if(++idx5==4){
            visString2025=true;
            idx5=0;
        }
        if(++idx6==4){
            visString2026=true;
            idx6=0;
        }
    }
    if(visString2025&&!visString2026)cout<<1<<endl;
    else cout<<0<<endl;
}
signed main(){
    ios::sync_with_stdio(false);//cin.tie(0);cout.tie(0);
    ll _=1;cin>>_;while(_--)
        solve(_);
    return 0;
}