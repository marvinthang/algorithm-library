template <class T, T (*op) (T, T)> class SparseTable {
public:
    SparseTable() = default;
    SparseTable(const vector <T> &v) { build(v); }
    void build(const vector <T> &v) {
        n = v.size();
        int h = n ? __lg(n) + 1 : 0;
        table.assign((long long) h * n, T());
        if (!n) return;
        copy(v.begin(), v.end(), table.begin());
        for (int k = 1; k < h; ++k) {
            int len = 1 << k, half = len >> 1;
            long long cur = 1LL * k * n, prev = 1LL * (k - 1) * n;
            for (int i = 0; i + len <= n; ++i)
                table[cur + i] = op(table[prev + i], table[prev + i + half]);
        }
    }
    T get(int l, int r) const { // [l, r - 1]
        assert(0 <= l && l < r && r <= n);
        int k = __lg(r - l);
        long long offset = 1LL * k * n;
        return op(table[offset + l], table[offset + r - (1 << k)]);
    }
    int size(void) const { return n; }
    bool empty(void) const { return !n; }

private:
    vector <T> table;
    int n = 0;
};
template <class T> T sparse_table_min(T x, T y) { return x < y ? x : y; }
template <class T> T sparse_table_max(T x, T y) { return x > y ? x : y; }
