#include <bits/stdc++.h>
using namespace std;
using ll = long long;
struct TreeLCA {
    int size;
    vector<int> firstOcc;
    vector<int> tour;
    vector<int> tourDepth;
    vector<ll> nodeDist;
    vector<int> logtable;
    vector<vector<int>> sparse;
    vector<vector<pair<int,int>>> adj;
    TreeLCA(int n = 0) {
        if (n > 0) setup(n);
    }
    void setup(int n) {
        size = n;
        adj.assign(size + 1, {});
        firstOcc.assign(size + 1, -1);
        nodeDist.assign(size + 1, 0);
        tour.clear();
        tourDepth.clear();
    }

    void link(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void euler_dfs(int u, int parent, int dep) {
        firstOcc[u] = static_cast<int>(tour.size());
        tour.push_back(u);
        tourDepth.push_back(dep);
        for (auto &pr : adj[u]) {
            int v = pr.first;
            int w = pr.second;
            if (v == parent) continue;
            nodeDist[v] = nodeDist[u] + w;
            euler_dfs(v, u, dep + 1);
            tour.push_back(u);
            tourDepth.push_back(dep);
        }
    }

    void prepare(int root = 1) {
        euler_dfs(root, 0, 0);

        int m = static_cast<int>(tour.size());
        logtable.assign(m + 1, 0);
        for (int i = 2; i <= m; ++i) {
            logtable[i] = logtable[i / 2] + 1;
        }

        int K = logtable[m];
        sparse.assign(K + 1, vector<int>(m));
        iota(sparse[0].begin(), sparse[0].end(), 0);

        auto pick_shallower = [&](int i, int j) {
            return tourDepth[i] < tourDepth[j] ? i : j;
        };

        for (int k = 1; k <= K; ++k) {
            int span = 1 << k;
            int half = span >> 1;
            for (int i = 0; i + span <= m; ++i) {
                int a = sparse[k - 1][i];
                int b = sparse[k - 1][i + half];
                sparse[k][i] = pick_shallower(a, b);
            }
        }
    }

    int getLca(int u, int v) {
        int L = firstOcc[u];
        int R = firstOcc[v];
        if (L > R) swap(L, R);
        int k = logtable[R - L + 1];
        int i = sparse[k][L];
        int j = sparse[k][R - (1 << k) + 1];
        int idx = tourDepth[i] < tourDepth[j] ? i : j;
        return tour[idx];
    }

    ll distBetween(int u, int v) {
        int c = getLca(u, v);
        return nodeDist[u] + nodeDist[v] - 2 * nodeDist[c];
    }
};

static inline int decodeIndex(uint64_t x, uint64_t mod, ll lastAns) {
    uint64_t lastAbs = (lastAns >= 0) ? (uint64_t) lastAns : (uint64_t)(-lastAns);
    uint64_t r = (x ^ lastAbs) % mod;
    return static_cast<int>(r + 1);
}
void solve(){
    int nodes;
    cin>>nodes;
    TreeLCA tree(nodes);
    for (int i = 1; i <= nodes - 1; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        tree.link(u, v, w);
    }
    tree.prepare(1);

    int queries;
    cin >> queries;

    vector<int> seq;
    seq.reserve(queries + 5);

    ll last = 0;

    while (queries--) {
        int type;
        cin >> type;
        if (type == 1) {
            uint64_t x;
            cin >> x;
            int node = decodeIndex(x, static_cast<uint64_t>(nodes), last);
            seq.push_back(node);
        } else if (type == 2) {
            if (!seq.empty()) seq.pop_back();
        } else { // type == 3
            uint64_t Lx, Rx, Xx;
            cin >> Lx >> Rx >> Xx;

            int len = static_cast<int>(seq.size());
            if (len == 0) {
                cout << 0 << '\n';
                last = 0;
                continue;
            }

            int l = decodeIndex(Lx, static_cast<uint64_t>(len), last);
            int r = decodeIndex(Rx, static_cast<uint64_t>(len), last);
            if (l > r) swap(l, r);
            int x = decodeIndex(Xx, static_cast<uint64_t>(nodes), last);

            ll best = LLONG_MIN;
            for (int i = l - 1; i <= r - 1; ++i) {
                ll d = tree.distBetween(x, seq[i]);
                if (d > best) best = d;
            }
            cout << best << '\n';
            last = best;
        }
    }
}
signed main(){
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    ios::sync_with_stdio(false);
//    ll _=1;cin>>_;while(_--)
        solve();
    return 0;
}