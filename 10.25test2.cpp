#include<bits/stdc++.h>
#define endl "\n"
using namespace std;

static const int MODULO = 1000000007;
//const int P1 = 469762049, P2 = 167772161, P3 = 998244353;
const int P1 = 167772161, P2 = 469762049, P3 = 998244353;

static const uint32_t ROOT = 3;

static inline uint32_t add_mod(uint32_t x, uint32_t y, uint32_t m) {
    uint32_t z = x + y;
    return z >= m ? z - m : z;
}

static inline uint32_t sub_mod(uint32_t x, uint32_t y, uint32_t m) {
    return x >= y ? x - y : x + m - y;
}

static inline uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t m) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = (result * base) % m;
        base = (base * base) % m;
        exp >>= 1;
    }
    return result;
}

static inline int64_t extended_gcd(int64_t a, int64_t b, int64_t &p, int64_t &q) {
    if (!b) {
        p = 1;
        q = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t g = extended_gcd(b, a % b, x1, y1);
    p = y1;
    q = x1 - y1 * (a / b);
    return g;
}

static inline uint64_t modular_inverse(uint64_t a, uint64_t mod) {
    int64_t x, y;
    extended_gcd((int64_t)a, (int64_t)mod, x, y);
    x %= (int64_t)mod;
    if (x < 0) x += mod;
    return (uint64_t)x;
}

template<uint32_t MODN>
struct NTTPoly {
    void transform(vector<uint32_t> &vec, bool inv) {
        int n = (int)vec.size();
        static vector<int> rev;
        static vector<uint32_t> roots{0, 1};
        if ((int)rev.size() != n) {
            int k = __builtin_ctz(n);
            rev.assign(n, 0);
            for (int i = 0; i < n; i++) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
        }
        if ((int)roots.size() < n) {
            int k = __builtin_ctz(roots.size());
            roots.resize(n);
            while ((1 << k) < n) {
                uint64_t z = pow_mod(ROOT, (MODN - 1) >> (k + 1), MODN);
                for (int i = 1 << (k - 1); i < (1 << k); ++i) {
                    roots[2 * i] = roots[i];
                    roots[2 * i + 1] = (uint64_t)roots[i] * z % MODN;
                }
                ++k;
            }
        }
        for (int i = 0; i < n; i++) if (i < rev[i]) swap(vec[i], vec[rev[i]]);
        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; ++j) {
                    uint32_t u = vec[i + j];
                    uint32_t v = (uint64_t)vec[i + j + len] * roots[len + j] % MODN;
                    vec[i + j] = add_mod(u, v, MODN);
                    vec[i + j + len] = sub_mod(u, v, MODN);
                }
            }
        }
        if (inv) {
            reverse(vec.begin() + 1, vec.end());
            uint32_t inv_n = pow_mod(n, MODN - 2, MODN);
            for (auto &x : vec) x = (uint64_t)x * inv_n % MODN;
        }
    }

    vector<uint32_t> multiply(const vector<uint32_t> &A, const vector<uint32_t> &B) {
        if (A.empty() || B.empty()) return {};
        int sz = (int)A.size() + (int)B.size() - 1;
        int n = 1; while (n < sz) n <<= 1;
        vector<uint32_t> fA(A.begin(), A.end()), fB(B.begin(), B.end());
        fA.resize(n); fB.resize(n);
        transform(fA, false); transform(fB, false);
        for (int i = 0; i < n; i++) fA[i] = (uint64_t)fA[i] * fB[i] % MODN;
        transform(fA, true); fA.resize(sz);
        return fA;
    }
};

static NTTPoly<P1> nttP1;
static NTTPoly<P2> nttP2;
static NTTPoly<P3> nttP3;

struct CRTTriple {
    uint64_t inv12 = modular_inverse(P1, P2);
    uint64_t inv123 = modular_inverse((uint64_t)P1 * P2 % P3, P3);
    uint64_t p1_mod = P1 % MODULO;
    uint64_t p12_mod = (uint64_t)P1 % MODULO * (P2 % MODULO) % MODULO;

    inline uint32_t merge(uint32_t r1, uint32_t r2, uint32_t r3) {
        uint64_t t1 = (r2 + (uint64_t)P2 - (r1 % P2)) % P2;
        t1 = t1 * inv12 % P2;
        uint64_t x12 = (r1 + (uint64_t)P1 * t1) % P3;
        uint64_t t2 = (r3 + (uint64_t)P3 - x12) % P3;
        t2 = t2 * inv123 % P3;
        uint64_t res = ((r1 % MODULO) + p1_mod * t1 + p12_mod * t2) % MODULO;
        return (uint32_t)res;
    }

    vector<int> conv_mod(const int *A, int nA, const int *B, int nB) {
        if (!nA || !nB) return {};
        vector<uint32_t> a1(nA), a2(nA), a3(nA);
        vector<uint32_t> b1(nB), b2(nB), b3(nB);
        for (int i = 0; i < nA; ++i) {
            uint64_t x = ((int64_t)A[i] % MODULO + MODULO) % MODULO;
            a1[i] = x % P1;
            a2[i] = x % P2;
            a3[i] = x % P3;
        }
        for (int i = 0; i < nB; ++i) {
            uint64_t x = ((int64_t)B[i] % MODULO + MODULO) % MODULO;
            b1[i] = x % P1;
            b2[i] = x % P2;
            b3[i] = x % P3;
        }
        auto c1 = nttP1.multiply(a1, b1);
        auto c2 = nttP2.multiply(a2, b2);
        auto c3 = nttP3.multiply(a3, b3);
        int sz = (int)c1.size();
        vector<int> res(sz);
        for (int i = 0; i < sz; ++i) res[i] = merge(c1[i], c2[i], c3[i]);
        return res;
    }
} crt;

static int MAXN;
static vector<int> inv_table, g_table, f_table, acc_table;
static const int SMALL_BLOCK = 256;

void solve_cdq(int left, int right) {
    if (left == right) {
        if (left == 0) return;
        f_table[left] = (int)((long long)acc_table[left] * inv_table[left] % MODULO);
        return;
    }
    if (right - left <= SMALL_BLOCK) {
        int start = (left == 0 ? 1 : left);
        for (int j = start; j <= right; ++j) {
            long long s = acc_table[j];
            for (int i = left; i < j; ++i) s += (long long)f_table[i] * g_table[j - i];
            s %= MODULO;
            acc_table[j] = (int)s;
            f_table[j] = (int)(s * inv_table[j] % MODULO);
        }
        return;
    }
    int mid = (left + right) >> 1;
    solve_cdq(left, mid);
    int n1 = mid - left + 1;
    int n2 = right - left + 1;
    vector<int> A(n1), B(n2);
    for (int i = 0; i < n1; ++i) A[i] = f_table[left + i];
    B[0] = 0;
    for (int i = 1; i < n2; ++i) B[i] = g_table[i];
    auto C = crt.conv_mod(A.data(), n1, B.data(), n2);
    for (int t = mid + 1; t <= right; ++t) {
        int idx = t - left;
        if (idx < (int)C.size()) {
            int val = C[idx];
            int nval = acc_table[t] + val;
            if (nval >= MODULO) nval -= MODULO;
            acc_table[t] = nval;
        }
    }
    solve_cdq(mid + 1, right);
}

void main_solver() {
    cin >> MAXN;
    vector<int> sigma(MAXN + 1, 0);
    for (int d = 1; d <= MAXN; ++d)
        for (int m = d; m <= MAXN; m += d) sigma[m] += d;

    g_table.assign(MAXN + 1, 0);
    for (int k = 1; k <= MAXN; ++k) g_table[k] = (int)((2LL * sigma[k]) % MODULO);

    inv_table.assign(MAXN + 1, 0);
    if (MAXN >= 1) {
        inv_table[1] = 1;
        for (int i = 2; i <= MAXN; ++i)
            inv_table[i] = (int)((long long)(MODULO - MODULO / i) * inv_table[MODULO % i] % MODULO);
    }

    f_table.assign(MAXN + 1, 0);
    f_table[0] = 1;
    acc_table.assign(MAXN + 1, 0);
    solve_cdq(0, MAXN);

    vector<int> H(MAXN + 1, 0);
    for (long long k = 1;; ++k) {
        long long t = k * (k + 1) / 2;
        if (t > MAXN) break;
        H[(int)t] = (k & 1) ? 1 : (MODULO - 1);
    }

    auto ans_poly = crt.conv_mod(H.data(), (int)H.size(), f_table.data(), (int)f_table.size());
    if ((int)ans_poly.size() < MAXN + 1) ans_poly.resize(MAXN + 1, 0);

    for (int n = 1; n <= MAXN; ++n) cout << (ans_poly[n] % MODULO) << '\n';
}

signed main() {
//#ifdef ONLINE_JUDGE
//#else
//    freopen("test.in", "r", stdin);
//    freopen("test.out", "w", stdout);
//#endif
    ios::sync_with_stdio(false);
    main_solver();
    return 0;
}
