#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXLOG = 20;

int N, M;
struct Edge{int v; int w;};
vector<vector<Edge>> g;
vector<int> up[MAXLOG];
vector<int> tin, tout, depth;
vector<ll> d;
int timer = 0;

void dfs(int root=1){
    tin.assign(N+1,0); tout.assign(N+1,0); depth.assign(N+1,0);
    d.assign(N+1,0);
    for(int k=0;k<MAXLOG;k++) up[k].assign(N+1,0);
    vector<int> st; st.reserve(N);
    st.push_back(root);
    up[0][root]=root;
    vector<int> it(N+1,0);
    while(!st.empty()){
        int u=st.back();
        if(!tin[u]){
            tin[u]=++timer;
            for(auto &e: g[u]){
                int v=e.v;
                if(v==up[0][u]) continue;
                up[0][v]=u;
                depth[v]=depth[u]+1;
                d[v]=d[u]+e.w;
                st.push_back(v);
            }
        }else{
            st.pop_back();
            tout[u]=++timer;
        }
    }
    for(int k=1;k<MAXLOG;k++){
        for(int v=1;v<=N;v++) up[k][v]=up[k-1][up[k-1][v]];
    }
}

bool is_anc(int a,int b){ return tin[a]<=tin[b] && tout[b]<=tout[a]; }
int lca(int a,int b){
    if(is_anc(a,b)) return a;
    if(is_anc(b,a)) return b;
    for(int k=MAXLOG-1;k>=0;k--){
        int w=up[k][a];
        if(!is_anc(w,b)) a=w;
    }
    return up[0][a];
}
ll dist(int a,int b){ int c=lca(a,b); return d[a]+d[b]-2*d[c]; }

struct Node{
    int a,b;
    bool emp;
    Node():a(-1),b(-1),emp(true){}
    Node(int x):a(x),b(x),emp(false){}
};

Node mergeNode(const Node &L,const Node &R){
    if(L.emp) return R;
    if(R.emp) return L;
    vector<int> cand = {L.a,L.b,R.a,R.b};
    ll best = LLONG_MIN;
    pair<int,int> bestp = {L.a,L.b};
    int sz = cand.size();
    for(int i=0;i<sz;i++) for(int j=i+1;j<sz;j++){
            ll val = dist(cand[i], cand[j]);
            if(val>best){ best=val; bestp={cand[i],cand[j]}; }
        }
    Node res;
    res.a = bestp.first; res.b = bestp.second; res.emp = false;
    return res;
}

struct SegTree {
    int n;
    vector<Node> t;
    SegTree(int _n=0){ init(_n); }
    void init(int _n){
        n=1; while(n<_n) n<<=1;
        t.assign(2*n, Node());
    }
    void setpos(int pos,int val){ // pos:1-based
        int i = pos-1 + n;
        if(val<=0) t[i]=Node();
        else t[i]=Node(val);
        for(i>>=1;i;i>>=1) t[i]=mergeNode(t[i<<1], t[i<<1|1]);
    }
    Node query(int l,int r){ // 1-based inclusive
        l = l-1 + n; r = r-1 + n;
        Node Lres, Rres;
        while(l<=r){
            if(l&1) Lres = mergeNode(Lres, t[l++]);
            if(!(r&1)) Rres = mergeNode(t[r--], Rres);
            l>>=1; r>>=1;
        }
        return mergeNode(Lres, Rres);
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if(!(cin>>N)) return 0;
    g.assign(N+1, {});
    for(int i=0;i<N-1;i++){
        int u,v,w; cin>>u>>v>>w;
        g[u].push_back({v,w});
        g[v].push_back({u,w});
    }
    dfs(1);
    cin>>M;
    SegTree seg(M);
    int cur_sz = 0;
    ll last_ans = 0;
    for(int qi=0; qi<M; qi++){
        int type; cin>>type;
        if(type==1){
            long long xin; cin>>xin;
            int x = (int)(( (xin ^ ll(llabs(last_ans))) % N) + 1);
            ++cur_sz;
            seg.setpos(cur_sz, x);
        }else if(type==2){
            if(cur_sz>0){
                seg.setpos(cur_sz, 0);
                --cur_sz;
            }
        }else if(type==3){
            long long lin, rin, xin; cin>>lin>>rin>>xin;
            int l = int(( (lin ^ ll(llabs(last_ans))) % (cur_sz==0?1:cur_sz) ) + 1);
            int r = int(( (rin ^ ll(llabs(last_ans))) % (cur_sz==0?1:cur_sz) ) + 1);
            if(l>r) swap(l,r);
            int x = (int)(( (xin ^ ll(llabs(last_ans))) % N) + 1);
            Node res = seg.query(l,r);
            ll ans = 0;
            if(res.emp) ans = 0;
            else ans = max(dist(res.a, x), dist(res.b, x));
            cout<<ans<<"\n";
            last_ans = ans;
        }
    }
    return 0;
}
