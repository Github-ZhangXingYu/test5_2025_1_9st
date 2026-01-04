//
// Created by 15411 on 2025/4/6.
//
#include<bits/stdc++.h>
#define endl "\n"
using namespace std;
const int N=5e5+7;
const int M=5e4+7;//限制到1e3,建树时会卡到4*M*M
char s[N];
string T[M];
namespace SAM{
    int sz=-1,last=0,root=0;//sz分配空间，last记录上一次末尾节点，t0是树起点
    struct node{
        int son[27];//a-z #
        int link,len;//后缀链接,最长串长度
        int end_ti;//记录当前状态来自的文本编号，即终点节点
        int max_ti,max_cnt;//最多出现的文本编号，最多出现次数
    }t[M*32];//5e4*16（线段树16层，每层都覆盖1-m个字符串）*2（每个节点最多复制一次）=1600000=M*32<2e6,
    void newNode(int length){
        t[++sz].len=length;
        t[sz].link=-1;//后缀链接树还未知
        memset(t[sz].son,0,sizeof(t[sz].son));
        t[sz].end_ti=-1;
        t[sz].max_ti=-1;t[sz].max_cnt=-1;
    }
    void insert(int c,int ti){
        newNode(t[last].len+1);
        t[sz].end_ti=ti;
        int p=last,cur=sz;     //p：上一个节点位置，cur：新节点位置
        while(p!=-1&&!t[p].son[c])
            t[p].son[c]=cur,p=t[p].link;
        if(p==-1)
            t[cur].link=root;//起始节点
        else{
            int q=t[p].son[c];
            if(t[q].len==t[p].len+1)    //判断是否合法，即当前点长度是前一个点的最短长度(t[p].len+1)
                t[cur].link=q;
            else{
                newNode(t[p].len+1);
                int nq=sz;     //复制点q
                memcpy(t[nq].son,t[q].son,sizeof(t[q].son));
                t[nq].link=t[q].link;
                t[cur].link=t[q].link=nq;
                while(p!=-1&&t[p].son[c]==q)
                    t[p].son[c]=nq,p=t[p].link;
            }
        }
        last=cur;
    }
    vector<int>G[M<<1];
    void dfs(int now,int ti_now){
        //只统计状态now在文本Ti的出现次数
        t[now+root].max_ti=ti_now;
        if(t[now+root].end_ti==ti_now)
            t[now+root].max_cnt=1;
        else
            t[now+root].max_cnt=0;
        for(auto next:G[now]){
            dfs(next,ti_now);
            t[now+root].max_cnt+=t[next+root].max_cnt;
        }
    }
    int build(int l,int r){//把Tl-Tr (Tl#...#Tr#) 建后缀自动机，并返回起始点
        newNode(0);
        last=sz;
        root=sz;
        for(int i=l;i<=r;i++){
            //插入当前文本ti
            for(int id=0;id<T[i].length();id++){
                insert(T[i][id]-'a',i);
            }
            //插入分隔符 #
            insert(26,-1);
        }

        //利用后缀链接树link更新每个点的max_ti,max_cnt
        int len=r-l+1;
        if(len==1){
            //建立母树
            for(int i=root;i<=sz;i++){
                G[i-root].clear();
                t[i].max_ti=-1;t[i].max_cnt=-1;
            }
            for(int i=root+1;i<=sz;i++){
                G[t[i].link-root].push_back(i-root);
            }
            for(int i=l;i<=r;i++){
                dfs(0,i);
            }
        }
        return root;
    }

    void dfs2(int p,int lc,int rc){
        if(lc==-1){
            t[p].max_ti=t[rc].max_ti;
            t[p].max_cnt=t[rc].max_cnt;
        }
        else if(rc==-1){
            t[p].max_ti=t[lc].max_ti;
            t[p].max_cnt=t[lc].max_cnt;
        }
        else{
            if(t[lc].max_cnt>=t[rc].max_cnt){
                t[p].max_ti=t[lc].max_ti;
                t[p].max_cnt=t[lc].max_cnt;
            }
            else{
                t[p].max_ti=t[rc].max_ti;
                t[p].max_cnt=t[rc].max_cnt;
            }
        }
        for(int i=0,np,nlc,nrc;i<26;i++){
            if(!t[p].son[i])continue;
            np=t[p].son[i];
            if(lc!=-1&&t[lc].son[i])nlc=t[lc].son[i];
            else nlc=-1;
            if(rc!=-1&&t[rc].son[i])nrc=t[rc].son[i];
            else nrc=-1;
            dfs2(np,nlc,nrc);
        }
    }
    pair<int,int> query(int l,int r,int pl,int pr,int t0){//最多出现在ti 在ti中出现次数
        int now=t0,next=0;
        pair<int,int>res(l,0);
        for(int i=pl-1;i<=pr-1;i++){
            next=t[now].son[s[i]-'a'];
            if(!next)
                return res;//当前自动机 没有子串s
            else
                now=next;
        }
        res.first=t[now].max_ti;
        res.second=t[now].max_cnt;
        return res;
    }
    int ans[N];
    int query(char* s,int now_root){//最长公共子串
        int now=now_root,n=(int)strlen(s);
        int ret=0;
        for(int i=0;i<n;i++){
            if(t[now].son[s[i]-'a']){
                now=t[now].son[s[i]-'a'];
                ans[i+1]=ans[i]+1;
            }
            else{
                while(now!=-1&&!t[now].son[s[i]-'a'])
                    now=t[now].link;
                if(now==-1){
                    now=0;
                    ans[i+1]=0;
                }
                else{
                    ans[i+1]=t[now].len+1;
                    now=t[now].son[s[i]-'a'];
                }
            }
            ret=max(ret,ans[i+1]);
//            cout<<i<<" "<<ans[i+1]<<endl;
        }
        return ret;
    }

    void show(){//用于调试，输出SAM
        for(int i=0;i<=sz;i++){
            cout<<"now: "<<i<<" link: "<<t[i].link<<"  "<<" to: ";
            for(int j=0;j<26;j++){
                if(t[i].son[j])
                    cout<<char('a'+j)<<t[i].son[j]<<"  ";
            }
            cout<<endl;
        }
    }
}
namespace xds{
#define lc p<<1
#define rc ((p<<1)|1)
    struct node{
        int l,r;
        int root;//存放当前点对应的root
    }tree[M<<2];
    void push_up(int p){
        tree[p].root=SAM::build(tree[p].l,tree[p].r);
        SAM::dfs2(tree[p].root,tree[lc].root,tree[rc].root);
    }
    void build(int p,int l,int r){
        tree[p].l=l;tree[p].r=r;
        if(l==r){
            tree[p].root=SAM::build(tree[p].l,tree[p].r);
            return;
        }
        int mid=(l+r)>>1;
        build(lc,l,mid);
        build(rc,mid+1,r);
        push_up(p);
    }
    pair<int,int> query(int p,int L,int R,int pl,int pr){
        if(L<=tree[p].l&&tree[p].r<=R){
            return SAM::query(L,R,pl,pr,tree[p].root);
        }
        int mid=(tree[p].l+tree[p].r)>>1;
        pair<int,int> res(L,0);
        pair<int,int> res1(L,0);
        pair<int,int> res2(L,0);
        if(L<=mid)res1= query(lc,L,R,pl,pr);
        if(R>mid)res2= query(rc,L,R,pl,pr);
        if(res1.second>=res2.second)
            res=res1;
        else
            res=res2;
        return res;
    }
}
void solve() {
    cin>>s;
    int m;cin>>m;
    for(int i=1;i<=m;i++){
        cin>>T[i];
    }
    xds::build(1,1,m);
    int max_len=SAM::query(s,xds::tree[1].root);//最长公共子串长度
    int q;cin>>q;
    int l,r,pl,pr;
    while(q--){
        cin>>l>>r>>pl>>pr;
//        int root=SAM::build(l,r);
//        pair<int,int> ans=SAM::query(pl,pr,root);
//        cout<<ans.first<<" "<<ans.second<<endl;
//        SAM::show();cout<<endl;
        if(pr-pl+1>SAM::ans[pr])//s（1-pr）与T1#T2#...#Tm的最长公共子串
            cout<<l<<" "<<0<<endl;
        else{
            pair<int,int> ans=xds::query(1,l,r,pl,pr);
            cout<<ans.first<<" "<<ans.second<<endl;
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