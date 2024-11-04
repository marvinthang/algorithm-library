template <class T> struct FenwickTree {
    int n; vector <T> bit;
    FenwickTree() = default;
    FenwickTree(int _n): n(_n), bit(n + 1, T{}) {}
    FenwickTree(const vector <T> &a): n(a.size()), bit(a.size() + 1, T{}) {
        for (int i = 1; i <= n; ++i) {
            bit[i] += a[i - 1];
            if (i + (i & -i) <= n) bit[i + (i & -i)] += bit[i];
        }
    }
    void reset(void) { fill(bit.begin(), bit.end(), T{}); }
    void update(int i, T val) { for (i = max(1, i + 1); i <= n; i += i & -i) bit[i] += val; }
    T get(int i) {
        if (i < 0) return T{};
        T res = T{};
        for (i = min(i + 1, n); i > 0; i &= i - 1) res += bit[i];
        return res;
    }
    T get(int l, int r) { return get(r) - get(l - 1); }
    int upper_bound(T val) {
        int res = 0;
        for (int i = __lg(n); i >= 0; --i)
            if ((res | (1 << i)) <= n && val >= bit[res | (1 << i)]) res |= (1 << i), val -= bit[res];
        return res;
    }
    int lower_bound(T val) {
        int res = 0;
        for (int i = __lg(n); i >= 0; --i)
            if ((res | (1 << i)) <= n && val > bit[res | (1 << i)]) res |= (1 << i), val -= bit[res];
        return res;
    }
};
