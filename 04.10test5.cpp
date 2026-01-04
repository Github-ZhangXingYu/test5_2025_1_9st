//分情况查询，卡到TE8
// Created by 15411 on 2025/4/9.
//修改了线段树查询，与提前预处理S字符串加速查询，TE7,理论复杂度最接近题解版本
//加入二分link链跳转，使得一次查询复杂度最多 log(|5e4*2|)+log(|s|);
#include<bits/stdc++.h>
#pragma GCC optimize(3)
#pragma GCC target("avx")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-fwhole-program")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-skip-blocks")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("-funsafe-loop-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")
#pragma GCC optimize(2)
#define endl "\n"
using namespace std;

class fastIO {
private:
    char ibuf[50007], *p1 = ibuf, *p2 = ibuf, obuf[50007], *p3 = obuf, sta[50];
    bool file_end = false;
    char get() {
        return p1 == p2 && (p2 = (p1 = ibuf) + fread(ibuf, 1, 50007, stdin), p1 == p2) ? (file_end = true), char(EOF): *p1++;
    }
    void put(const char x) {
        p3 - obuf < 50007 ? *p3 ++ = x : (fwrite(obuf, p3 - obuf, 1, stdout), p3 = obuf, *p3++ = x);
    }
public:
    explicit operator bool() { return !file_end; }
    size_t flush() {
        size_t f = fwrite(obuf, p3 - obuf, 1, stdout);
        p3 = obuf;
        *p3 = 0;
        return f;
    }
    fastIO &operator>>(char &t) {
        for (t = get(); !isgraph(t); t = get());
        return *this;
    }
    template<typename any>
    typename enable_if<is_same<any, char>::value, any>::type tpval() {
        char t;
        for (t = get(); !isgraph(t); t = get());
        return t;
    }
    fastIO &operator>>(char *t) {
        char c;
        for (c = get(); !isgraph(c); c = get());
        for (; isgraph(c); c = get())*t = c, t++;
        *t = 0;
        return *this;
    }
    fastIO &operator>>(string &t) {
        t.clear();
        char c;
        for (c = get(); !isgraph(c); c = get());
        for (; isgraph(c); c = get())t += c;
        return *this;
    }
    template<typename any>
    typename enable_if<is_same<any, string>::value, any>::type tpval() {
        string t;
        char c;
        for (c = get(); !isgraph(c); c = get());
        for (; isgraph(c); c = get())t += c;
        return t;
    }
    template<typename any>
    typename enable_if<
            (is_signed<any>::value && is_integral<any>::value && !is_same<any, char>::value) ||
            is_same<any, __int128_t>::value, fastIO>::type &operator>>(any &t) {
        t = 0;
        bool y = 0;
        char c = get();
        for (; !isdigit(c); c = get())if (c == 45)y = true;
        for (; isdigit(c); c = get())t = t * 10 + c - 48;
        if (y == 1)t = -t;
        return *this;
    }
    template<typename any>
    typename enable_if<
            (is_signed<any>::value && is_integral<any>::value && !is_same<any, char>::value) ||
            is_same<any, __int128_t>::value, any>::type tpval() {
        any t = 0;
        bool y = 0;
        char c = get();
        for (; !isdigit(c); c = get())if (c == 45)y = true;
        for (; isdigit(c); c = get())t = t * 10 + c - 48;
        if (y == 1)t = -t;
        return t;
    }
    template<typename any>
    typename enable_if<
            (is_unsigned<any>::value && is_integral<any>::value && !is_same<any, char>::value) ||
            is_same<any, __uint128_t>::value, fastIO>::type &operator>>(any &t) {
        t = 0;
        char c = get();
        for (; !isdigit(c); c = get());
        for (; isdigit(c); c = get())t = t * 10 + c - 48;
        return *this;
    }
    template<typename any>
    typename enable_if<
            (is_unsigned<any>::value && is_integral<any>::value && !is_same<any, char>::value) ||
            is_same<any, __uint128_t>::value, any>::type tpval() {
        any t = 0;
        char c = get();
        for (; !isdigit(c); c = get());
        for (; isdigit(c); c = get())t = t * 10 + c - 48;
        return t;
    }
    template<typename any1, typename any2>
    fastIO &operator>>(pair<any1, any2> &t) { return *this >> t.first >> t.second; }
    template<typename any1, typename any2>
    pair<any1, any2> tpval() { return pair<any1, any2>(tpval<any1>(), tpval<any2>()); }
    template<typename any>
    fastIO &read(any &t) { return *this >> t; }
    fastIO &read(char *t) {
        char c;
        for (c = get(); !isgraph(c); c = get());
        for (; isgraph(c); c = get())*t = c, t++;
        *t = 0;
        return *this;
    }
    template<typename any, typename...args>
    fastIO &read(any &t1, args &...t2) { return (*this >> t1).read(t2...); }
    fastIO &operator<<(const char t) {
        put(t);
        return *this;
    }
    fastIO &operator<<(const char *t) {
        for (; *t; t++)put(*t);
        return *this;
    }
    fastIO &operator<<(const string &t) {
        for (const char it: t)put(it);
        return *this;
    }
    template<typename any>
    typename enable_if<
            (is_signed<any>::value && is_integral<any>::value && !is_same<any, char>::value) ||
            is_same<any, __int128_t>::value, fastIO>::type &operator<<(any t) {
        if (!t) {
            put(48);
            return *this;
        }
        int len = 0;
        if (t < 0)t = -t, put(45);
        while (t)sta[len++] = char(t % 10 + 48), t /= 10;
        while (len--)put(sta[len]);
        return *this;
    }
    template<typename any>
    typename enable_if<
            (is_unsigned<any>::value && is_integral<any>::value && !is_same<any, char>::value) ||
            is_same<any, __uint128_t>::value, fastIO>::type &operator<<(any t) {
        if (!t) {
            put(48);
            return *this;
        }
        int len = 0;
        while (t)sta[len++] = char(t % 10 + 48), t /= 10;
        while (len--)put(sta[len]);
        return *this;
    }
    template<typename any1, typename any2>
    fastIO &operator<<(const pair<any1, any2> &t) { return *this << t.first << ' ' << t.second; }
    template<typename any>
    fastIO &write(const any &t) { return *this << t; }
    template<typename any, typename...args>
    fastIO &write(const any &t1, const args &...t2) { return (*this << t1).write(t2...); }

    ~fastIO() { fwrite(obuf, p3 - obuf, 1, stdout); }
}FastIO;
#define cin FastIO
#define cout FastIO

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
        t[p].tl=lc;t[p].tr=rc;
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
            dfs2(np,nlc,nrc);
        }
    }

    int query(int l,int r,int pl,int pr,int t0){//查询spl-spr在tree[1].root构建的SAM里对应的endpos等价点
        int now=t0,next=0;
//        pair<int,int>res(l,0);
        for(int i=pl-1;i<=pr-1;i++){
            next=t[now].son[s[i]-'a'];
            if(!next)
                return -1;//当前自动机 没有子串s
            else
                now=next;
        }
//        res.first=t[now].max_ti;
//        res.second=t[now].max_cnt;
        return now;
    }
//    int query(int sam_p,int len){//查询spl-spr在tree[1].root构建的SAM里对应的endpos等价点
//        int now=sam_p;
//        while(now!=-1&&t[t[now].link].len>=len)
//            now=t[now].link;
//        return now;
//    }
    vector<vector<int>>link;
    void init_tow_link(int now_root){//预处理tree[1].root所有点的二分link点
        for(int i=sz;i>=now_root;i--)if(t[i].link>=0)t[t[i].link].to=true;//记录link链的终点

        for(int i=sz;i>=now_root;i--){
            if(t[i].vst||t[i].to)continue;

            vector<int>v;//记录当前二分链上的点
            int now=i;
            while(now!=-1){
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
    pair<int,int> query(int p,int L,int R,int pl,int pr,int sam_p){
        if(L<=tree[p].l&&tree[p].r<=R){
            pair<int,int>res(L,0);
            if(sam_p!=-1){
                res.first=SAM::t[sam_p].max_ti;
                res.second=SAM::t[sam_p].max_cnt;
            }
            return res;
//            return SAM::query(L,R,pl,pr,tree[p].root);
        }
        int mid=(tree[p].l+tree[p].r)>>1;
        pair<int,int> res(L,0);
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
            res=res1;
        else
            res=res2;
        return res;
    }
}
int main(){
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
//    std::ios::sync_with_stdio(false);
//    std::cin.tie(nullptr);std::cout.tie(nullptr);
    cin>>s;
    int m;cin>>m;
    for(int i=1;i<=m;i++){
        cin>>T[i];
    }
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
            int sam_p;
            if(pr-pl+1<=100)
                sam_p=SAM::query(l,r,pl,pr,xds::tree[1].root);
            else
                sam_p=SAM::query(SAM::endpos[pr],pr-pl+1);
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