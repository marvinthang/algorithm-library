namespace MODINT {
struct barrett {
    unsigned int _m;
    unsigned long long im;
    explicit barrett(unsigned int m) : _m(m), im((unsigned long long)(-1) / m + 1) {}
    unsigned int umod() const { return _m; };
    unsigned int mul(unsigned int a, unsigned int b) const {
        unsigned long long z = a; z *= b;
        unsigned long long x = (unsigned long long)(((unsigned __int128) z * im) >> 64);
        unsigned long long y = x * _m;
        return (unsigned int)(z - y + (z < y ? _m : 0));
    }
};
template <class T> T invGeneral(T a, T b) {
    a %= b;
    if (!a) return b == 1 ? 0 : -1;
    T x = invGeneral(b, a);
    return x == -1 ? -1 : ((1 - 1LL * b * x) / a + b) % b;
}
template <int m, enable_if_t<1 <= m>* = nullptr>
struct static_modint {
using mint = static_modint;
public:
    static constexpr int mod() { return m; }
    static mint raw(int v) {
        mint x; x.v = v;
        return x;
    }
    static_modint(): v(0) {}
    template <class T> static_modint(T x) {
        int y;
        if (x < 0) {
            if (x < -mod()) y = x % mod();
            else y = x;
            if (y < 0) y += mod();
        } else {
            if (x < mod()) y = x;
            else y = x % mod();
        }
        v = y;
    }
    unsigned int val() const { return v; }
    unsigned int operator () () const { return v; }
    mint & operator ++ () { if (++v == umod()) v = 0; return *this; }
    mint & operator -- () { if (!v) v = umod(); --v; return *this; }
    mint operator ++ (int) { mint old = *this; ++*this; return old; }
    mint operator -- (int) { mint old = *this; --*this; return old; }
    mint operator + () { return *this; }
    mint operator - () { return raw(!v ? 0 : umod() - v); }
    mint & operator += (const mint &rhs) { v += rhs.v; if (v >= umod()) v -= umod(); return *this; }
    mint & operator -= (const mint &rhs) { v -= rhs.v; if (v >= umod()) v += umod(); return *this; }
    mint & operator *= (const mint &rhs) {
        unsigned long long z = v; z *= rhs.v; v = z % umod();
        return *this;
    }
    mint & operator /= (const mint &rhs) { return *this *= rhs.inv(); }
    friend mint operator + (const mint &lhs, const mint &rhs) { return mint(lhs) += rhs; }
    friend mint operator - (const mint &lhs, const mint &rhs) { return mint(lhs) -= rhs; }
    friend mint operator * (const mint &lhs, const mint &rhs) { return mint(lhs) *= rhs; }
    friend mint operator / (const mint &lhs, const mint &rhs) { return mint(lhs) /= rhs; }
    mint pow(long long n) const {
        assert(0 <= n);
        mint res = 1, a = *this;
        for (; n; n >>= 1, a *= a) if (n & 1) res *= a;
        return res;
    }
    mint inv() const {
        int i = invGeneral((int) v, mod());
        assert(~i);
        return i;
    }
    friend bool operator == (const mint& lhs, const mint& rhs) { return lhs.v == rhs.v; }
    friend bool operator != (const mint& lhs, const mint& rhs) { return lhs.v != rhs.v; }
    friend ostream & operator << (ostream &out, const mint &x) { return out << x.v; }
    friend istream & operator >> (istream &in, mint &x) { long long a; in >> a; x = a; return in; }
    explicit operator bool() const { return v; }
    explicit operator int() const { return v; }
private:
    unsigned int v;
    static constexpr unsigned int umod() { return m; }
};
template <int id> struct dynamic_modint {
using mint = dynamic_modint;
public:
    static int mod() { return (int) bt.umod(); }
    static void set_mod(int m) {
        assert(1 <= m);
        bt = barrett(m);
    }
    static mint raw(int v) {
        mint x; x.v = v;
        return x;
    }
    dynamic_modint(): v(0) {}
    template <class T> dynamic_modint(T x) {
        int y;
        if (x < 0) {
            if (x < -mod()) y = x % mod();
            else y = x;
            if (y < 0) y += mod();
        } else {
            if (x < mod()) y = x;
            else y = x % mod();
        }
        v = y;
    }
    unsigned int val() const { return v; }
    unsigned int operator () () const { return v; }
    mint & operator ++ () { if (++v == umod()) v = 0; return *this; }
    mint & operator -- () { if (!v) v = umod(); --v; return *this; }
    mint operator ++ (int) { mint old = *this; ++*this; return old; }
    mint operator -- (int) { mint old = *this; --*this; return old; }
    mint operator + () { return *this; }
    mint operator - () { return raw(!v ? 0 : umod() - v); }
    mint & operator += (const mint &rhs) { v += rhs.v; if (v >= umod()) v -= umod(); return *this; }
    mint & operator -= (const mint &rhs) { v -= rhs.v; if (v >= umod()) v += umod(); return *this; }
    mint & operator *= (const mint &rhs) {
        v = bt.mul(v, rhs.v);
        return *this;
    }
    mint & operator /= (const mint &rhs) { return *this *= rhs.inv(); }
    friend mint operator + (const mint &lhs, const mint &rhs) { return mint(lhs) += rhs; }
    friend mint operator - (const mint &lhs, const mint &rhs) { return mint(lhs) -= rhs; }
    friend mint operator * (const mint &lhs, const mint &rhs) { return mint(lhs) *= rhs; }
    friend mint operator / (const mint &lhs, const mint &rhs) { return mint(lhs) /= rhs; }
    mint pow(long long n) const {
        assert(0 <= n);
        mint res = 1, a = *this;
        for (; n; n >>= 1, a *= a) if (n & 1) res *= a;
        return res;
    }
    mint inv() const {
        int i = invGeneral((int) v, mod());
        assert(~i);
        return i;
    }
    friend bool operator == (const mint& lhs, const mint& rhs) { return lhs.v == rhs.v; }
    friend bool operator != (const mint& lhs, const mint& rhs) { return lhs.v != rhs.v; }
    friend ostream & operator << (ostream &out, const mint &x) { return out << x.v; }
    friend istream & operator >> (istream &in, mint &x) { long long a; in >> a; x = a; return in; }
    explicit operator bool() const { return v; }
    explicit operator int() const { return v; }
private:
    unsigned int v;
    static barrett bt;
    static unsigned int umod() { return bt.umod(); }
};
template <int id> barrett dynamic_modint<id>::bt(998244353);
using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;
using modint = dynamic_modint <-1>;
using Modular = modint1000000007;
} using namespace MODINT;
