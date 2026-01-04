//
// Created by 15411 on 2025/10/25.
//
#include<bits/stdc++.h>
#define endl "\n"
using namespace std;
static const int MOD = 1000000007;
static const uint32_t P1 = 998244353;
static const uint32_t P2 = 1004535809;
static const uint32_t P3 = 469762049;
static const uint32_t G  = 3;

static inline uint32_t addmod(uint32_t a, uint32_t b, uint32_t m) {
    uint32_t c = a + b;
    return c >= m ? c - m : c;
}

static inline uint32_t submod(uint32_t a, uint32_t b, uint32_t m) {
    return a >= b ? a - b : a + m - b;
}

static inline uint64_t powmod(uint64_t a, uint64_t e, uint64_t m) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = (r * a) % m;
        a = (a * a) % m;
        e >>= 1;
    }
    return r;
}

static inline int64_t exgcd(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

static inline uint64_t inv_mod(uint64_t a, uint64_t mod) {
    int64_t x, y;
    exgcd((int64_t) a, (int64_t) mod, x, y);
    x %= (int64_t) mod;
    if (x < 0) x += mod;
    return (uint64_t) x;
}

template<uint32_t MODN>
struct NTT {
    void ntt(vector<uint32_t> &a, bool invert) {
        int n = (int) a.size();
        static vector<int> rev;
        static vector<uint32_t> roots{0, 1};
        if ((int) rev.size() != n) {
            int k = __builtin_ctz(n);
            rev.assign(n, 0);
            for (int i = 0; i < n; i++) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
        }
        if ((int) roots.size() < n) {
            int k = __builtin_ctz(roots.size());
            roots.resize(n);
            while ((1 << k) < n) {
                uint64_t z = powmod(G, (MODN - 1) >> (k + 1), MODN);
                for (int i = 1 << (k - 1); i < (1 << k); ++ i) {
                    roots[2 * i] = roots[i];
                    roots[2 * i + 1] = (uint64_t) roots[i] * z % MODN;
                }
                ++k;
            }
        }
        for (int i = 0; i < n; i++) if (i < rev[i]) swap(a[i], a[rev[i]]);
        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; ++ j) {
                    uint32_t u = a[i + j];
                    uint32_t v = (uint64_t) a[i + j + len] * roots[len + j] % MODN;
                    a[i + j] = addmod(u, v, MODN);
                    a[i + j + len] = submod(u, v, MODN);
                }
            }
        }
        if (invert) {
            reverse(a.begin() + 1, a.end());
            uint32_t inv_n = powmod(n, MODN - 2, MODN);
            for (uint32_t &x: a) x = (uint64_t) x * inv_n % MODN;
        }
    }
    vector<uint32_t> conv(const vector<uint32_t>& A, const vector<uint32_t>& B){
        if(A.empty()||B.empty()) return {};
        int need = (int)A.size() + (int)B.size() - 1;
        int n=1; while(n<need) n<<=1;
        vector<uint32_t> fa(A.begin(),A.end()), fb(B.begin(),B.end());
        fa.resize(n); fb.resize(n);
        ntt(fa,false); ntt(fb,false);
        for(int i=0;i<n;i++) fa[i]=(uint64_t)fa[i]*fb[i]%MODN;
        ntt(fa,true); fa.resize(need);
        return fa;
    }
};
static NTT<P1> ntt1; static NTT<P2> ntt2; static NTT<P3> ntt3;

struct CRT3 {
    uint64_t inv_p1_mod_p2 = inv_mod(P1, P2);
    uint64_t inv_p1p2_mod_p3 = inv_mod((uint64_t) P1 * P2 % P3, P3);
    uint64_t p1_mod = P1 % MOD;
    uint64_t p1p2_mod = (uint64_t) P1 % MOD * (P2 % MOD) % MOD;
    inline uint32_t combine(uint32_t r1, uint32_t r2, uint32_t r3) {
        uint64_t t1 = ((r2 + (uint64_t) P2 - (r1 % P2)) % P2);
        t1 = t1 * inv_p1_mod_p2 % P2;
        uint64_t x12_mod_p3 = ((r1 + (uint64_t) P1 * t1) % P3);
        uint64_t t2 = ((r3 + (uint64_t) P3 - x12_mod_p3) % P3);
        t2 = t2 * inv_p1p2_mod_p3 % P3;
        uint64_t res = ((r1 % MOD) + p1_mod * t1 + p1p2_mod * t2) % MOD;
        return (uint32_t) res;
    }

    vector<int> convolution_mod(const int *A, int n1, const int *B, int n2) {
        if (n1 == 0 || n2 == 0) return {};
        vector<uint32_t> a1(n1), a2(n1), a3(n1);
        vector<uint32_t> b1(n2), b2(n2), b3(n2);
        for (int i = 0; i < n1; ++ i) {
            uint64_t x = ((int64_t) A[i] % MOD + MOD) % MOD;
            a1[i] = x % P1;
            a2[i] = x % P2;
            a3[i] = x % P3;
        }
        for (int i = 0; i < n2; ++ i) {
            uint64_t x = ((int64_t) B[i] % MOD + MOD) % MOD;
            b1[i] = x % P1;
            b2[i] = x % P2;
            b3[i] = x % P3;
        }
        auto c1 = ntt1.conv(a1, b1);
        auto c2 = ntt2.conv(a2, b2);
        auto c3 = ntt3.conv(a3, b3);
        int n = (int) c1.size();
        vector<int> res(n);
        for (int i = 0; i < n; ++ i) {
            res[i] = combine(c1[i], c2[i], c3[i]);
        }
        return res;
    }
} crt3;

static int M;
static vector<int> invs;
static vector<int> g;
static vector<int> f, acc;

static const int BLOCK = 256;

void cdq(int l, int r) {
    if (l == r) {
        if (l == 0) return;
        f[l] = (int) ((long long) acc[l] * invs[l] % MOD);
        return;
    }
    if (r - l <= BLOCK) {
        int start = (l == 0 ? 1 : l);
        for (int j = start; j <= r; ++ j) {
            long long s = acc[j];
            for (int i = l; i < j; ++ i) s += (long long) f[i] * g[j - i];
            s %= MOD;
            acc[j] = (int) s;
            f[j] = (int) (s * invs[j] % MOD);
        }
        return;
    }
    int mid = (l + r) >> 1;
    cdq(l, mid);
    int n1 = mid - l + 1;
    int n2 = r - l + 1;
    vector<int> A(n1), B(n2);
    for (int i = 0; i < n1; ++ i) {
        A[i] = f[l + i];
    }
    B[0] = 0;
    for (int i = 1; i < n2; ++ i) {
        B[i] = g[i];
    }
    auto C = crt3.convolution_mod(A.data(), n1, B.data(), n2);
    for (int t = mid + 1; t <= r; ++ t) {
        int idx = t - l;
        if (idx < (int) C.size()) {
            int v = C[idx];
            int nv = acc[t] + v;
            if (nv >= MOD) nv -= MOD;
            acc[t] = nv;
        }
    }
    cdq(mid + 1, r);
}
void solve() {
    cin >> M;
    vector<int> sigma(M + 1, 0);
    for (int d = 1; d <= M; ++ d) {
        for (int m = d; m <= M; m += d) {
            sigma[m] += d;
        }
    }
    g.assign(M + 1, 0);
    for (int k = 1; k <= M; ++ k) {
        long long x = (2LL * sigma[k]) % MOD;
        g[k] = (int) x;
    }

    invs.assign(M + 1, 0);
    if (M >= 1) {
        invs[1] = 1;
        for (int i = 2; i <= M; ++ i) {
            invs[i] = (int) ((long long) (MOD - MOD / i) * invs[MOD % i] % MOD);
        }
    }

    f.assign(M + 1, 0);
    f[0] = 1;
    acc.assign(M + 1, 0);
    cdq(0, M);

    vector<int> H(M + 1, 0);
    for (long long k = 1;; ++ k) {
        long long t = k * (k + 1) / 2;
        if (t > M) break;
        H[(int) t] = (k & 1) ? 1 : (MOD - 1);
    }
    auto ansPoly = crt3.convolution_mod(H.data(), (int) H.size(), f.data(), (int) f.size());
    if ((int) ansPoly.size() < M + 1) ansPoly.resize(M + 1, 0);

    for (int n = 1; n <= M; ++ n) {
        cout << (ansPoly[n] % MOD) << '\n';
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