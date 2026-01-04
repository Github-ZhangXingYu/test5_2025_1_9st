//
// Created by 15411 on 2025/4/13.
//加了初始化，方便多组造数据
//修复了dfs2反复查找到同一个点问题，AC
//修复了二分处的查询问题，TE8
//
//修改了线段树查询，与提前预处理S字符串加速查询，TE7,理论复杂度最接近题解版本
//加入二分link链跳转，使得一次查询复杂度最多 log(|5e4*2|)+log(|s|);
#include<bits/stdc++.h>
#define endl "\n"
using namespace std;
const int N=5e5+100;
const int M=1e5+100;//限制到1e3,建树时会卡到4*M*M   .#情况
char s[N];
string T[M];
namespace SAM{
    int sz=-1,last=0,root=0;//sz分配空间，last记录上一次末尾节点，t0是树起点
    struct node{
        int son[27];//a-z #
        int link,len;//后缀链接,最长串长度
        int end_ti;//记录当前状态来自的文本编号，即终点节点
        int max_ti,max_cnt;//最多出现的文本编号，最多出现次数
        int tl,tr;//存当前endpos等价点的左儿子和右儿子
        bool to,vst;//用于提前预处理，记录当前点有没有link_to，即是link链的终点，vst标记当前点是否已经计算过
        int link_id,id;//记录当前点的link链所属vector,与位置，用于二分
        int vstP;
    }t[M*32];//5e4*16（线段树16层，每层都覆盖1-m个字符串）*2（每个节点最多复制一次）=1600000=M*32<2e6,

    void newNode(int length){
        t[++sz].len=length;
        t[sz].link=-1;//后缀链接树还未知
        memset(t[sz].son,0,sizeof(t[sz].son));
        t[sz].end_ti=-1;
        t[sz].max_ti=-1;t[sz].max_cnt=-1;

        t[sz].tl=-1;t[sz].tr=-1;

        t[sz].to=false;t[sz].vst=false;
        t[sz].link_id=0;t[sz].id=0;

        t[sz].vstP=-1;
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
            dfs(0,l);
        }
        return root;
    }

    void dfs2(int p,int lc,int rc,int rootP){
        if(t[p].vstP==rootP)return;
        t[p].vstP=rootP;
        t[p].tl=lc;t[p].tr=rc;
        if(lc==-1&&rc==-1)return;
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
            if(nlc==-1&&nrc==-1)continue;
            dfs2(np,nlc,nrc,rootP);
        }
    }

    vector<vector<int>>link;
    void init_tow_link(int now_root){//预处理tree[1].root所有点的二分link点
        for(int i=sz;i>=now_root;i--)if(t[i].link>=0)t[t[i].link].to=true;//记录link链的终点

        for(int i=sz;i>=now_root;i--){
            if(t[i].vst||t[i].to)continue;

            vector<int>v;//记录当前二分链上的点
            int now=i;
            while(now!=-1&&!t[now].vst){
                v.push_back(now);
                now=t[now].link;
            }
            reverse(v.begin(),v.end());//1<-2<-5<-7<-10

            link.push_back(v);
            int link_id=(int)link.size()-1;
            int size=(int)v.size();
            for(int j=size-1;j>=0;j--){
                now=v[j];
                t[now].vst=true;
                t[now].link_id=link_id;
                t[now].id=j;
            }

        }
    }
    int query(int sam_p,int len){//查询spl-spr在tree[1].root构建的SAM里对应的endpos等价点
        int now=sam_p;
        int link_id=t[now].link_id;
        while(t[link[link_id][0]].link!=-1&&t[t[link[link_id][0]].link].len>=len){
            now=t[link[link_id][0]].link;
            link_id=t[now].link_id;
        }
        int l=0,r=t[now].id;
        while(l<r){
            int mid=(l+r)>>1;
            int now_len=t[link[link_id][mid]].len;
            if(now_len<len)
                l=mid+1;
            else
                r=mid;
        }
        now=link[link_id][l];
        return now;
    }

    int ans[N];
    int endpos[N];
    void query(char* s,int now_root){//最长公共子串
        int now=now_root,n=(int)strlen(s);
        for(int i=0;i<n;i++){
            if(t[now].son[s[i]-'a']){
                now=t[now].son[s[i]-'a'];
                ans[i+1]=ans[i]+1;
            }
            else{
                while(now!=-1&&!t[now].son[s[i]-'a'])
                    now=t[now].link;
                if(now==-1){
                    now=now_root;
                    ans[i+1]=0;
                }
                else{
                    ans[i+1]=t[now].len+1;
                    now=t[now].son[s[i]-'a'];
                }
            }
            endpos[i+1]=now;
        }
    }

    void init(){
        sz=-1,last=0,root=0;
        for(auto & i : G)i.clear();
        for(auto &i:ans)i=0;
        for(auto &i:endpos)i=0;
        link.clear();
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
        SAM::dfs2(tree[p].root,tree[lc].root,tree[rc].root,tree[p].root);
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
    pair<int,int> query(int p,int L,int R,int pl,int pr,int sam_p){
        if(L<=tree[p].l&&tree[p].r<=R){
            pair<int,int>res(L,0);
            if(sam_p!=-1){
                res.first=SAM::t[sam_p].max_ti;
                res.second=SAM::t[sam_p].max_cnt;
            }
            return res;
        }
        int mid=(tree[p].l+tree[p].r)>>1;
        pair<int,int> res1(L,0);
        pair<int,int> res2(L,0);
        int sam_pl=-1,sam_pr=-1;
        if(sam_p!=-1){
            sam_pl=SAM::t[sam_p].tl;
            sam_pr=SAM::t[sam_p].tr;
        }
        if(L<=mid&&sam_pl!=-1)res1= query(lc,L,R,pl,pr,sam_pl);
        if(R>mid&&sam_pr!=-1)res2= query(rc,L,R,pl,pr,sam_pr);
        if(res1.second>=res2.second)
            return res1;
        else
            return res2;
    }
}
int main(){
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);std::cout.tie(nullptr);
    cin>>s;
    int m;cin>>m;
    for(int i=1;i<=m;i++){
        cin>>T[i];
    }
    SAM::init();
    xds::build(1,1,m);
    SAM::query(s,xds::tree[1].root);//预处理 s[i]的最长公共后缀 长度和endpos等价点
    SAM::init_tow_link(xds::tree[1].root);//预处理好endpos的link二分查询
    int q;cin>>q;
    int l,r,pl,pr;
    while(q--){
        cin>>l>>r>>pl>>pr;
        if(pr-pl+1>SAM::ans[pr])//s（1-pr）与T1#T2#...#Tm的最长公共子串
            cout<<l<<" "<<0<<endl;
        else{
            int sam_p=SAM::query(SAM::endpos[pr],pr-pl+1);
            pair<int,int> ans=xds::query(1,l,r,pl,pr,sam_p);
            if(ans.second<=0){
                ans.first=l;
                ans.second=0;
            }
            cout<<ans.first<<" "<<ans.second<<endl;
        }
    }
    return 0;
}
/*diniuwpaur
10
hceuaaqnoc
qehyuoaqoz
enceqhpdoq
slahystosa
aqoiuuzmby
rebqteakys
ssyqsqkecn
qagmhhxqve
graajqomgk
aoavuyqkaa
10
9 9 3 5
10 10 3 5
9 9 7 9
5 10 3 3
3 7 1 3
6 10 9 9
5 5 10 10
5 7 10 10
7 7 9 10
5 5 7 8
*/