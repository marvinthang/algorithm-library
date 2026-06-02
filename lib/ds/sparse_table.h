template <class T, T (*op) (T, T)> class SparseTable {
public:
    SparseTable() = default;
    SparseTable(const vector <T> &v): table{v}, n(v.size()) {
        for (int k = 1; (1 << k) <= n; ++k) {
            table.emplace_back(n - (1 << k) + 1);
            for (int i = 0; i + (1 << k) <= n; i++) table[k][i] = op(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
        }
    }
    T get(int l, int r) { // [l, r - 1]
        assert(0 <= l && l < r && r <= n);
        int k = __lg(r - l);
        return op(table[k][l], table[k][r - (1 << k)]);
    }
private:
    vector <vector <T>> table;
    int n;
};
template <class T> T _min(T x, T y) { return x < y ? x : y; }
template <class T> T _max(T x, T y) { return x > y ? x : y; }
