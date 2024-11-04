template <class T, T (*op) (T, T)> struct DisjointSparseTable {
    int n;
    vector <vector <T>> st;
    DisjointSparseTable() {}
    DisjointSparseTable(const vector <T> &a): n(a.size()) {
        st.push_back(a);
        for (int p = 1; 1 << p < n; p++) {
            st.emplace_back(n);
            for (int mid = 1 << p; mid < n; mid += 1 << (p + 1)) {
                st[p][mid - 1] = a[mid - 1];
                for (int j = mid - 2; j >= mid - (1 << p); --j) st[p][j] = op(a[j], st[p][j + 1]);
                st[p][mid] = a[mid];
                for (int j = mid + 1; j < min(n, mid + (1 << p)); ++j) st[p][j] = op(st[p][j - 1], a[j]);
            }
        }
    }
    T get(int l, int r) const {
        assert(0 <= l && l < r && r <= n);
        if (r - l == 1) return st[0][l];
        int p = __lg(l ^ (r - 1));
        return op(st[p][l], st[p][r - 1]);
    }
};
