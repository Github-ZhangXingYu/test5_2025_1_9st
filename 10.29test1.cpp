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
string longestCommonPrefix(vector<string>& strs) {
    int lenMax=0;
    int len=strs.size();
    string s;
    for(int lenNow=1;lenNow<=strs[1].size();lenNow++){
        bool isOk=true;
        for(int i=0;i<len;i++){
            if(strs[i].length()<lenNow){
                isOk=false;
                break;
            }
            if(strs[i][lenNow-1]!=strs[0][lenNow-1]){
                isOk=false;
                break;
            }
        }
        if(!isOk){
            lenMax=lenNow-1;
            break;
        }
    }
    for(int i=0;i<lenMax;i++){
        s+=strs[0][i];
    }
    return s;
}
void solve() {
    ll n;cin>>n;
    vector<string>strs(N,"");
    for(int i=1;i<=n;i++)
        cin>>strs[i];
    cout<<longestCommonPrefix(strs)<<endl;
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