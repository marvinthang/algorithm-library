namespace NUMBER_THEORETIC_TRANSFORM {
template <typename Modular> class NTT {
public:
    static int md, base, max_base;
    static Modular root;
    static vector <Modular> roots;
    static vector <int> rev;
    static void clear() {
        root = base = max_base = 0;
        roots.clear();
        rev.clear();
    }
    static void init() {
        md = Modular::mod();
        assert(md >= 3 && md % 2 == 1);
        auto tmp = md - 1;
        max_base = 0;
        while (!(tmp & 1)) tmp >>= 1, ++max_base;
        root = 2;
        while (root.pow((md - 1) >> 1).val() == 1) ++root;
        assert(root.pow(md - 1) == 1);
        root = root.pow((md - 1) >> max_base);
        base = 1; rev = {0, 1}; roots = {0, 1};
    }
    static void ensure_base(int nbase) {
        if (md != Modular::mod()) clear();
        if (roots.empty()) init();
        if (nbase <= base) return;
        assert(nbase <= max_base);
        rev.resize(1 << nbase);
        for (int i = 0; i < (1 << nbase); ++i) rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
        roots.resize(1 << nbase);
        while (base < nbase) {
            Modular z = root.pow(1 << (max_base - 1 - base));
            for (int i = 1 << (base - 1); i < (1 << base); ++i) {
                roots[i << 1] = roots[i];
                roots[i << 1 | 1] = roots[i] * z;
            }
            ++base;
        }
    }
    static void fft(vector <Modular> &a) {
        int n = (int) a.size();
        assert((n & (n - 1)) == 0);
        int zeros = __builtin_ctz(n);
        ensure_base(zeros);
        int shift = base - zeros;
        for (int i = 0; i < n; ++i) if (i < (rev[i] >> shift)) swap(a[i], a[rev[i] >> shift]);
        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += k << 1) {
                for (int j = 0; j < k; j++) {
                    Modular x = a[i + j];
                    Modular y = a[i + j + k] * roots[j + k];
                    a[i + j] = x + y;
                    a[i + j + k] = x - y;
                }
            }
        }
    }
    static vector <Modular> multiply(vector <Modular> a, vector <Modular> b) {
        if (a.empty() || b.empty()) return {};
        int eq = (a == b);
        int need = (int) a.size() + (int) b.size() - 1;
        int nbase = 0;
        while ((1 << nbase) < need) ++nbase;
        ensure_base(nbase);
        int sz = 1 << nbase;
        a.resize(sz); b.resize(sz);
        fft(a);
        if (eq) b = a;
        else fft(b);
        Modular inv_sz = static_cast<Modular>(sz).inv();
        for (int i = 0; i < sz; ++i) a[i] *= b[i] * inv_sz;
        reverse(a.begin() + 1, a.end());
        fft(a);
        a.resize(need);
        return a;
    }
};
template <typename Modular> int NTT<Modular>::md;
template <typename Modular> Modular NTT<Modular>::root;
template <typename Modular> int NTT<Modular>::base;
template <typename Modular> int NTT<Modular>::max_base;
template <typename Modular> vector<Modular> NTT<Modular>::roots;
template <typename Modular> vector<int> NTT<Modular>::rev;
template <typename Modular> vector <Modular> inverse(const vector <Modular> &a) {
    assert(!a.empty());
    int n = (int) a.size();
    vector <Modular> b = {a[0].inv()};
    while ((int) b.size() < n) {
        vector <Modular> x(a.begin(), a.begin() + min(a.size(), b.size() << 1));
        x.resize(b.size() << 1); b.resize(b.size() << 1);
        vector <Modular> c = b;
        NTT<Modular>::fft(c); NTT<Modular>::fft(x);
        Modular inv = static_cast<Modular>((int)x.size()).inv();
        for (int i = 0; i < (int) x.size(); ++i) x[i] *= c[i] * inv;
        reverse(x.begin() + 1, x.end());
        NTT<Modular>::fft(x);
        rotate(x.begin(), x.begin() + (x.size() >> 1), x.end());
        fill(x.begin() + (x.size() >> 1), x.end(), 0);
        NTT<Modular>::fft(x);
        for (int i = 0; i < (int) x.size(); ++i) x[i] *= c[i] * inv;
        reverse(x.begin() + 1, x.end());
        NTT<Modular>::fft(x);
        for (int i = 0; i < (int) x.size() >> 1; ++i) b[i + ((int)x.size() >> 1)] = -x[i];
    }
    b.resize(n);
    return b;
}
template <typename Modular> vector <Modular> inverse_old(vector <Modular> a) {
    assert(!a.empty());
    int n = a.size();
    if (n == 1) return { a[0].inv() };
    int m = (n + 1) >> 1;
    vector <Modular> b = inverse(vector<Modular>(a.begin(), a.begin() + m));
    int need = n << 1, nbase = 0;
    while ((1 << nbase) < need) ++nbase;
    NTT<Modular>::ensure_base(nbase);
    int size = 1 << nbase;
    a.resize(size); b.resize(size);
    NTT<Modular>::fft(a); NTT<Modular>::fft(b);
    Modular inv = static_cast<Modular>(size).inv();
    for (int i = 0; i < size; ++i) a[i] = (2 - a[i] * b[i]) * b[i] * inv;
    reverse(a.begin() + 1, a.end());
    NTT<Modular>::fft(a);
    a.resize(n);
    return a;
}
template <typename Modular> vector <Modular> operator * (const vector <Modular> &a, const vector <Modular> &b) {
    if (a.empty() || b.empty()) return {};
    if (min(a.size(), b.size()) < 150) {
        vector <Modular> c(a.size() + b.size() - 1, 0);
        for (int i = 0; i < (int) a.size(); ++i)
            for (int j = 0; j < (int) b.size(); j++)
                c[i + j] += a[i] * b[j];
        return c;
    }
    return NTT<Modular>::multiply(a, b);
}
template <typename Modular> vector <Modular> & operator *= (vector<Modular> &a, const vector <Modular> &b) { return a = a * b; }
}
using NUMBER_THEORETIC_TRANSFORM::operator *;
using NUMBER_THEORETIC_TRANSFORM::operator *=;
