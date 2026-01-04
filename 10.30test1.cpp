//
// Created by 15411 on 2025/10/30.
//
#include<stdio.h>
void solve() {
    int n,m;scanf("%d %d",&n,&m);
    if(n>m) {
        int t=n;n=m;
        m=t;
    }
    if(32<=n&&m<=126){
        for(int i=n;i<=m;i++){
            char c=(char)i;
            printf("%c",c);
        }
        printf("\n");
    }
    else{
        printf("I do not think so!\n");
    }
}

signed main(){
    int _=1;scanf("%d",&_);while(_--)
        solve();
    return 0;
}