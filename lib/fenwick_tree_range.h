template<
    class T,  // data type for nodes
    T (*op) (T, T),  // operator to combine 2 nodes
    T (*e)() // identity element
    >
struct FenwickTree_R {
    int n; vector <T> bit1, bit2, a;
    FenwickTree_R() = default;
    FenwickTree_R(int _n): n(_n), bit1(n + 1, e()), bit2(n + 1, e()), a(n + 1, e()) {}
    void reset(void) {
        fill(bit1.begin(), bit1.end(), e());
        fill(bit2.begin(), bit2.end(), e());
        fill(a.begin(), a.end(), e());
    }
#define lowbit(x) ((x) & -(x))
    FenwickTree_R(const vector <T> &v): n(v.size()), bit1(n + 1, e()), bit2(n + 1, e()), a(n + 1, e()) {
        for (int i = 1; i <= n; ++i) {
            a[i] = v[i - 1];
            bit1[i] = op(bit1[i], a[i]);
            if (i + lowbit(i) <= n) bit1[i + lowbit(i)] = op(bit1[i + lowbit(i)], bit1[i]);
        }
        for (int i = n; i > 0; --i) {
            bit2[i] = op(a[i], bit2[i]);
            if (i - lowbit(i) > 0) bit2[i - lowbit(i)] = op(bit2[i], bit2[i - lowbit(i)]);
        }
    }
    void set(int p, T val) {
        assert(0 <= p && p < n);
        ++p;
        a[p] = val;
        T cur = val;
        for (int i = p, l = p - 1, r = p + 1; i <= n; i += lowbit(i)) {
            for (; r < i; r += lowbit(r)) cur = op(cur, bit2[r]);
            for (; l > i - lowbit(i); l -= lowbit(l)) cur = op(bit1[l], cur);
            bit1[i] = i == p ? cur : op(cur, a[r]);
        }
        cur = val;
        for (int i = p, l = p - 1, r = p + 1; i > 0; i -= lowbit(i)) {
            for (; r < min(n + 1, i + lowbit(i)); r += lowbit(r)) cur = op(cur, bit2[r]);
            for (; l > i; l -= lowbit(l)) cur = op(bit1[l], cur);
            bit2[i] = i == p ? cur : op(a[l], cur);
        }
    }
    void get(int p) const { return a[p]; }
    // Get product in range [l, r-1]
    // For empty segment (l == r) -> return e()
    T prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return e();
        ++l;
        T lft = e(), rgt = e();
        int i;
        for (i = l; i + lowbit(i) <= r; i += lowbit(i)) lft = op(lft, bit2[i]);
        lft = op(lft, a[i]);
        for (i = r; i - lowbit(i) >= l; i -= lowbit(i)) rgt = op(bit1[i], rgt);
        return op(lft, rgt);
    }
    T all_prod() const {
        T res = e();
        for (int i = n; i > 0; i -= lowbit(i)) res = op(bit1[i], res);
        return res;
    }
};
// FenwickTree_R examples {{{
// Examples: Commonly used FenwickTree_R ops: max / min
struct MaxFenwickOp {
    static int op(int x, int y) { return max(x, y); }
    static int e() { return -1e9; }
};
struct MinFenwickOP {
    static int op(int x, int y) { return min(x, y); }
    static int e() { return 1e9; }
};
using MaxFenwick = FenwickTree_R <int, MaxFenwickOp::op, MaxFenwickOp::e>;
using MinFenwick = FenwickTree_R <int, MinFenwickOP::op, MinFenwickOP::e>;
// }}}
