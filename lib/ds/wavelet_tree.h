class WaveletTree {
public:
    WaveletTree(): lo(0), hi(-1), mid(0), n(0) {}
    template <class RandomIt> WaveletTree(RandomIt first, RandomIt last) {
        vector <int> v(first, last);
        build(v.begin(), v.end());
    }
    // kth smallest element in [l, r)
    int kth_smallest(int l, int r, int k) const {
        assert(0 <= l && l < r && r <= n && 1 <= k && k <= r - l);
        if (lo == hi) return lo;
        int inLeft = b[r] - b[l];
        if (k <= inLeft) return left->kth_smallest(b[l], b[r], k);
        return right->kth_smallest(l - b[l], r - b[r], k - inLeft);
    }
    // kth largest element in [l, r)
    int kth_largest(int l, int r, int k) const {
        return kth_smallest(l, r, r - l - k + 1);
    }
    // count of numbers in [l, r) less than or equal to k
    int LTE(int l, int r, int k) const {
        assert(0 <= l && l <= r && r <= n);
        if (l == r || k < lo) return 0;
        if (hi <= k) return r - l;
        return left->LTE(b[l], b[r], k) + right->LTE(l - b[l], r - b[r], k);
    }
    // count of numbers in [l, r) less than k
    int count_less(int l, int r, int k) const {
        assert(0 <= l && l <= r && r <= n);
        if (k <= lo) return 0;
        return LTE(l, r, k - 1);
    }
    // count of numbers in [l, r) equal to k
    int count(int l, int r, int k) const {
        assert(0 <= l && l <= r && r <= n);
        if (l == r || k < lo || k > hi) return 0;
        if (lo == hi) return r - l;
        if (k <= mid) return left->count(b[l], b[r], k);
        return right->count(l - b[l], r - b[r], k);
    }

private:
    int lo, hi, mid, n;
    vector <int> b;
    unique_ptr <WaveletTree> left, right;

    template <class RandomIt> void build(RandomIt first, RandomIt last) {
        n = last - first;
        left = nullptr;
        right = nullptr;
        if (!n) { lo = 0; hi = -1; mid = 0; return; }
        auto x = minmax_element(first, last);
        lo = *x.first; hi = *x.second;
        mid = lo + (int) (((long long) hi - lo) / 2);
        if (lo == hi) return;
        auto f = [&] (int x) { return x <= mid; };
        b.reserve(n + 1);
        b.push_back(0);
        for (auto it = first; it != last; ++it) b.push_back(b.back() + f(*it));
        auto pivot = stable_partition(first, last, f);
        left = make_unique <WaveletTree> ();
        right = make_unique <WaveletTree> ();
        left->build(first, pivot);
        right->build(pivot, last);
    }
};
