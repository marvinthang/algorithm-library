template<
    class T,        // data type for nodes
    T (*op) (T, T), // operator to combine 2 nodes
    T (*e)()        // identity element
    >
struct SegTree {
    SegTree(): SegTree(0) {}
    SegTree(int n) : SegTree(vector<T>(n, e())) {}
    SegTree(const vector <T>& v) : n(v.size()) {
        log = 0;
        while ((1 << log) < n) ++log;
        size = 1 << log;
        d = vector<T>(size << 1, e());
        for (int i = 0; i < n; ++i) d[size + i] = v[i];
        for (int i = size - 1; i > 0; --i) update(i);
    }

    // 0 <= p < n
    void set(int p, T x) {
        assert(0 <= p && p < n);
        p += size; d[p] = x;
        for (int i = 1; i <= log; ++i) update(p >> i);
    }

    // 0 <= p < n
    T get(int p) const {
        assert(0 <= p && p < n);
        return d[p + size];
    }

    // Get product in range [l, r-1]
    // 0 <= l <= r <= n
    // For empty segment (l == r) -> return e()
    T prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        T sml = e(), smr = e();
        l += size; r += size;
        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1; r >>= 1;
        }
        return op(sml, smr);
    }

    T all_prod() const { return d[1]; }

    // Binary search on SegTree to find largest r:
    //    f(op(a[l] .. a[r-1])) = true   (assuming empty array is always true)
    //    f(op(a[l] .. a[r])) = false    (assuming op(..., a[n]), which is out of bound, is always false)
    template <bool (*f)(T)> int max_right(int l) const { return max_right(l, [](T x) { return f(x); }); }
    template <class F> int max_right(int l, F f) const {
        assert(0 <= l && l <= n);
        assert(f(e()));
        if (l == n) return n;
        l += size;
        T sm = e();
        do {
            while (!(l & 1)) l >>= 1;
            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l = l << 1;
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    // Binary search on SegTree to find smallest l:
    //    f(op(a[l] .. a[r-1])) = true      (assuming empty array is always true)
    //    f(op(a[l-1] .. a[r-1])) = false   (assuming op(a[-1], ..), which is out of bound, is always false)
    template <bool (*f)(T)> int min_left(int r) const { return min_left(r, [](T x) { return f(x); }); }
    template <class F> int min_left(int r, F f) const {
        assert(0 <= r && r <= n);
        assert(f(e()));
        if (r == 0) return 0;
        r += size;
        T sm = e();
        do {
            r--;
            while (r > 1 && (r & 1)) r >>= 1;
            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = r << 1 | 1;
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

private:
    int n, size, log;
    vector <T> d;
    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
};
