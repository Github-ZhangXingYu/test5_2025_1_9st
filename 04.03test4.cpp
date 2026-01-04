//
// Created by 15411 on 2025/4/4.
//
#include <bits/stdc++.h>
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
typedef unsigned long long ull;

const ull BASE = 131;
struct SegTree {
    int n;
    vector<pair<int,int>> seg;
    SegTree() {}
    SegTree(const vector<pair<int,int>> &arr) {
        n = arr.size();
        seg.resize(n * 4);
        build(arr, 1, 0, n - 1);
    }
    void build(const vector<pair<int,int>> &arr, int idx, int l, int r) {
        if(l == r) {
            seg[idx] = {arr[l].second, arr[l].first};
            return;
        }
        int mid = (l + r) >> 1;
        build(arr, idx * 2, l, mid);
        build(arr, idx * 2 + 1, mid + 1, r);
        seg[idx] = merge(seg[idx * 2], seg[idx * 2 + 1]);
    }
    pair<int,int> merge(const pair<int,int> &a, const pair<int,int> &b) {
        if(a.first > b.first) return a;
        else if(a.first < b.first) return b;
        else return {a.first, min(a.second, b.second)};
    }
    pair<int,int> query(int idx, int l, int r, int ql, int qr) {
        if(ql > r || qr < l) return {-1, 0};
        if(ql <= l && r <= qr) return seg[idx];
        int mid = (l + r) >> 1;
        auto left = query(idx * 2, l, mid, ql, qr);
        auto right = query(idx * 2 + 1, mid + 1, r, ql, qr);
        return merge(left, right);
    }
    pair<int,int> query(int ql, int qr) {
        return query(1, 0, n - 1, ql, qr);
    }
};

unordered_map<ull, vector<pair<int,int>>> globalMap;
unordered_map<ull, SegTree> segMap;

void solve() {
    string s;
    cin >> s;
    int n = s.size();
    vector<ull> sHash(n + 1, 0), power(n + 1, 1);
    for (int i = 0; i < n; ++ i){
        sHash[i + 1] = sHash[i] * BASE + (s[i] - 'a' + 1);
        power[i + 1] = power[i] * BASE;
    }
    int m;
    cin >> m;
    vector<string> media(m);
    for (int i = 0; i < m; ++ i){
        cin >> media[i];
    }

    for (int i = 0; i < m; ++ i) {
        string &t = media[i];
        int len = t.size();
        vector<ull> prefix(len + 1, 0), powT(len + 1, 1);
        for (int j = 0; j < len; ++ j){
            prefix[j + 1] = prefix[j] * BASE + (t[j] - 'a' + 1);
            powT[j + 1] = powT[j] * BASE;
        }
        unordered_map<ull, int> cnt;
        cnt.reserve(len * (len + 1) / 2);
        for (int k = 0; k < len; ++ k){
            for (int x = k; x < len; ++ x){
                ull h = prefix[x + 1] - prefix[k] * powT[x - k + 1];
                ++ cnt[h];
            }
        }
        for (auto &p : cnt) {
            globalMap[p.first].push_back({i + 1, p.second});
        }
    }
    for (auto &entry : globalMap) {
        auto &vec = entry.second;
        sort(vec.begin(), vec.end(), [](const pair<int,int> &a, const pair<int,int> &b) {
            return a.first < b.first;
        });
        if (vec.size() > 1) {
            segMap[entry.first] = SegTree(vec);
        }
    }

    int q;
    cin >> q;
    while(q --) {
        int L, R, pl, pr;
        cin >> L >> R >> pl >> pr;
        ull queryHash = sHash[pr] - sHash[pl - 1] * power[pr - pl + 1];
        if(globalMap.find(queryHash) == globalMap.end()){
            cout << L << ' ' << 0 << '\n';
        } else {
            auto &vec = globalMap[queryHash];
            int lo = 0, hi = vec.size();
            while(lo < hi){
                int mid = (lo + hi) >> 1;
                if(vec[mid].first < L) lo = mid + 1;
                else hi = mid;
            }
            int start = lo;
            lo = 0, hi = vec.size();
            while(lo < hi){
                int mid = (lo + hi) >> 1;
                if(vec[mid].first <= R) lo = mid + 1;
                else hi = mid;
            }
            int end = lo - 1;
            if(start > end){
                cout << L << ' ' << 0 << '\n';
            } else {
                pair<int,int> res;
                if(vec.size() > 1 && segMap.find(queryHash) != segMap.end()) {
                    res = segMap[queryHash].query(start, end);
                } else {
                    res = {vec[start].second, vec[start].first};
                    for (int j = start + 1; j <= end; ++ j){
                        pair<int,int> cur = {vec[j].second, vec[j].first};
                        if(cur.first > res.first || (cur.first == res.first && cur.second < res.second))
                            res = cur;
                    }
                }
                cout << res.second << ' ' << res.first << '\n';
            }
        }
    }
}
int32_t main() {
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    int T = 1;
//    cin >> T;
    while(T --) {
        solve();
    }
    return 0;
}