//
// Created by 15411 on 2025/4/3.
//
#include <bits/stdc++.h>

using namespace std;

const int maxn = 5e4 + 42, logn = 20, sigma = 26;

string s[maxn];

struct suffix_automaton
{
    vector<int> len, link, fpos, lpos, left, right, ans, max, cnt, st;
    vector<vector<int>> to;
    vector<vector<int>> up;
    string s;
    int sz, last;

    void clear()
    {
        up.clear();
        to.clear();
        len.clear();
        link.clear();
        fpos.clear();
        lpos.clear();
        cnt.clear();
        st.clear();
        s.clear();
    }

    void reserve(int n)
    {
        up.reserve(n);
        to.reserve(n);
        len.reserve(n);
        link.reserve(n);
        fpos.reserve(n);
        lpos.reserve(n);
        cnt.reserve(n);
        st.reserve(n);
        s.reserve(n);
        left.reserve(n);
        right.reserve(n);
        ans.reserve(n);
        max.reserve(n);
    }

    int make_state(vector<int> new_to = vector<int>(sigma + 1), int new_link = 0, int new_fpos = 0, int new_len = 0)
    {
        to.push_back(new_to);
        link.push_back(new_link);
        fpos.push_back(new_fpos);
        len.push_back(new_len);
        lpos.push_back(new_fpos);
        left.push_back(0);
        right.push_back(0);
        ans.push_back(0);
        max.push_back(0);
        cnt.push_back(0);
        up.push_back(vector<int>(logn));
        return sz++;
    }

    void add_letter(char c)
    {
        s += c;
        int p = last;
        last = make_state(vector<int>(sigma + 1), 0, len[p] + 1, len[p] + 1);
        st.push_back(last);
        cnt[last] = 1;
        for(; to[p][c] == 0; p = link[p])
            to[p][c] = last;
        if(to[p][c] == last)
            return;
        int q = to[p][c];
        if(len[q] == len[p] + 1)
        {
            link[last] = q;
            return;
        }
        int cl = make_state(to[q], link[q], fpos[q], len[p] + 1);
        link[last] = link[q] = cl;
        for(; to[p][c] == q; p = link[p])
            to[p][c] = cl;
    }

    void build(string s)
    {
        make_state();
        int n = s.size();
        reserve(4 * n);

        for(auto c: s)
            add_letter(c);

        int mx_len[n]; // kill states like "X#Y"
        memset(mx_len, 0, sizeof(mx_len));
        int count = 0;
        for(int i = 0; i < n; i++)
        {
            mx_len[i] = count++;
            if(s[i] == sigma)
                count = 1;
        }
        for(int state = 1; state < sz; state++)
        {
            if(len[link[state]] < mx_len[fpos[state] - 1] && len[state] > mx_len[fpos[state] - 1])
            {
                int cl = make_state(to[state], link[state], fpos[state], mx_len[fpos[state] - 1]);
                link[state] = cl;
            }
        }

        for(int state = 1; state < sz; state++) // sparse table, like in LCA
            up[state][0] = link[state];
        for(int lvl = 1; lvl < logn; lvl++)
            for(int state = 1; state < sz; state++)
                up[state][lvl] = up[up[state][lvl - 1]][lvl - 1];

        vector<int> g[n + 1]; // this actually gives us reversed topsort
        for(int state = 1; state < sz; state++)
            g[len[state]].push_back(state);
        for(int ln = n; ln > 0; ln--)
            for(auto state: g[ln])
            {
                if(link[state])
                    cnt[link[state]] += cnt[state];
                lpos[link[state]] = std::max(lpos[link[state]], lpos[state]);
            }
    }

    int get(int pos, int ln) // kind of binary search on path to the root
    { // pos 1-based
        int state = st[pos - 1];
        for(int i = logn - 1; i >= 0; i--)
            if(len[up[state][i]] >= ln)
                state = up[state][i];
        return state;
    }
} me[4 * maxn];

void build(int v = 1, int l = 1, int r = maxn)
{
    if(r - l == 1)
    {
        s[l] = char(sigma) + s[l];
        me[v].build(s[l]);
        for(int state = 0; state < me[v].sz; state++)
        {
            me[v].ans[state] = l;
            me[v].max[state] = me[v].cnt[state];
        }
        return;
    }
    int m = (l + r) / 2;
    build(2 * v, l, m);
    build(2 * v + 1, m, r);
    me[v].build(me[2 * v].s + me[2 * v + 1].s);
    for(int state = 1; state < me[v].sz; state++)
    {
        if(me[v].fpos[state] <= (int) me[2 * v].s.size()) // we want to know states corresponding to strings from this state in children
            me[v].left[state] = me[2 * v].get(me[v].fpos[state], me[v].len[state]);
        if(me[v].lpos[state] > (int) me[2 * v].s.size())
            me[v].right[state] = me[2 * v + 1].get(me[v].lpos[state] - me[2 * v].s.size(), me[v].len[state]);
        me[v].max[state] = max(me[2 * v].max[me[v].left[state]], me[2 * v + 1].max[me[v].right[state]]);
        if(me[v].max[state] == me[2 * v].max[me[v].left[state]])
            me[v].ans[state] = me[2 * v].ans[me[v].left[state]];
        else
            me[v].ans[state] = me[2 * v + 1].ans[me[v].right[state]];
    }
    me[2 * v].clear(); // we don't need O(n log^2 n) memory
    me[2 * v + 1].clear();
}

pair<int, int> get(int a, int b, int state, int v = 1, int l = 1, int r = maxn)
{
    if(a <= l && r <= b)
        return {me[v].max[state], me[v].ans[state]};
    if(r <= a || b <= l)
        return {0, 0};
    int m = (l + r) / 2;
    auto A = get(a, b, me[v].left[state], 2 * v, l, m);
    auto B = get(a, b, me[v].right[state], 2 * v + 1, m, r);
    if(A.first >= B.first)
        return A;
    else
        return B;
}

int main()
{
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    //ios::sync_with_stdio(false);//cin.tie(0);cout.tie(0);
    string Q;
    cin >> Q;

    int n;
    cin >> n;
    for(int i = 0; i < n; i++)
    {
        cin >> s[i + 1];
        for(auto &it: s[i + 1])
            it -= 'a';
    }
    build(); // yeah, this thing builds!
    int state = 0, ln = 0;
    vector<int> states, lens;
    for(auto c: Q)
    {
        c -= 'a';
        while(state && !me[1].to[state][c])
        {
            state = me[1].link[state];
            ln = me[1].len[state];
        }
        if(me[1].to[state][c])
        {
            state = me[1].to[state][c];
            ln++;
        }
        lens.push_back(ln);
        states.push_back(state);
    }

    int m;
    cin >> m;
    while(m--)
    {
        int l, r, pl, pr;
        cin >> l >> r >> pl >> pr;
        if(pr - pl + 1 > lens[pr - 1])
        {
//            cout<<"i need "<<lens[pr - 1]<<endl;
            cout << l << ' ' << 0 << "\n";
            continue;
        }
        int state = me[1].get(me[1].fpos[states[pr - 1]], pr - pl + 1);
        auto ans = get(l, r + 1, state);
        cout << max(l, ans.second) << ' ' << ans.first << "\n";
    }
    return 0;
}
