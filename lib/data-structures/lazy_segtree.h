template <
    class S,                 // node data type
    S (*op) (S, S),          // combine 2 nodes
    S (*e) (),               // identity element
    class F,                 // lazy propagation tag
    S (*mapping) (F, S),     // apply tag F on a node
    F (*composition) (F, F), // combine 2 tags
    F (*id)()                // identity tag
    >
struct LazySegTree {
    LazySegTree() : LazySegTree(0) {}
    LazySegTree(int n) : LazySegTree(vector<S>(n, e())) {}
    LazySegTree(const vector <S> &v) : n(v.size()) {
        log = 0;
        while ((1 << log) < n) ++log;
        size = 1 << log;
        d = vector<S>(size << 1, e());
        lz = vector<F>(size, id());
        for (int i = 0; i < n; ++i) d[i + size] = v[i];
        for (int i = size - 1; i > 0; --i) update(i);
    }

    // 0 <= p < n
    void set(int p, S x) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i > 0; --i) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; ++i) update(p >> i);
    }

    // 0 <= p < n
    S get(int p) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i > 0; --i) push(p >> i);
        return d[p];
    }

    // Get product in range [l, r-1]
    // 0 <= l <= r <= n
    // For empty segment (l == r) -> return e()
    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return e();
        l += size; r += size;
        for (int i = log; i > 0; --i) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        S sml = e(), smr = e();
        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1; r >>= 1;
        }
        return op(sml, smr);
    }

    S all_prod() { return d[1]; }

    // 0 <= p < n
    void apply(int p, F f) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i > 0; --i) push(p >> i);
        d[p] = mapping(f, d[p]);
        for (int i = 1; i <= log; ++i) update(p >> i);
    }

    // Apply f on all elements in range [l, r-1]
    // 0 <= l <= r <= n
    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return;
        l += size; r += size;
        for (int i = log; i > 0; --i) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        int l2 = l, r2 = r;
        while (l < r) {
            if (l & 1) all_apply(l++, f);
            if (r & 1) all_apply(--r, f);
            l >>= 1; r >>= 1;
        }
        l = l2; r = r2;
        for (int i = 1; i <= log; ++i) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

    // Binary search on SegTree to find largest r:
    //    f(op(a[l] .. a[r-1])) = true   (assuming empty array is always true)
    //    f(op(a[l] .. a[r])) = false    (assuming op(..., a[n]), which is out of bound, is always false)
    template <bool (*g)(S)> int max_right(int l) { return max_right(l, [](S x) { return g(x); }); }
    template <class G> int max_right(int l, G g) {
        assert(0 <= l && l <= n);
        assert(g(e()));
        if (l == n) return n;
        l += size;
        for (int i = log; i > 0; --i) push(l >> i);
        S sm = e();
        do {
            while (!(l & 1)) l >>= 1;
            if (!g(op(sm, d[l]))) {
                while (l < size) {
                    push(l);
                    l = l << 1;
                    if (g(op(sm, d[l]))) {
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
    template <bool (*g)(S)> int min_left(int r) { return min_left(r, [](S x) { return g(x); }); }
    template <class G> int min_left(int r, G g) {
        assert(0 <= r && r <= n);
        assert(g(e()));
        if (!r) return 0;
        r += size;
        for (int i = log; i > 0; --i) push((r - 1) >> i);
        S sm = e();
        do {
            r--;
            while (r > 1 && (r & 1)) r >>= 1;
            if (!g(op(d[r], sm))) {
                while (r < size) {
                    push(r);
                    r = r << 1 | 1;
                    if (g(op(d[r], sm))) {
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
    vector<S> d;
    vector<F> lz;
    void update(int k) { d[k] = op(d[k << 1], d[k << 1 | 1]); }
    void all_apply(int k, F f) {
        d[k] = mapping(f, d[k]);
        if (k < size) lz[k] = composition(f, lz[k]);
    }
    void push(int k) {
        all_apply(k << 1, lz[k]);
        all_apply(k << 1 | 1, lz[k]);
        lz[k] = id();
    }
};
