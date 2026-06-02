struct WaveletTree {
    int lo, hi, mid;
    vector <int> b;
    WaveletTree *left, *right;
    template <class RandomIt> WaveletTree(RandomIt first, RandomIt last) {
        if (first == last) return;
        auto x = minmax_element(first, last);
        lo = *x.fi; hi = *x.se;
        if (lo == hi) return;
        mid = lo + hi >> 1;
        auto f = [&] (int x) { return x <= mid; };
        b.reserve(last - first + 1);
        b.push_back(0);
        for (auto it = first; it != last; ++it) b.push_back(b.back() + f(*it));
        auto mid = stable_partition(first, last, f);
        left = new WaveletTree(first, mid);
        right = new WaveletTree(mid, last);
    }
    // kth smallest element in [l, r)
    int kth_smallest(int l, int r, int k) {
        if (l > r) return 0;
        if (lo == hi) return lo;
        int inLeft = b[r] - b[l];
        if (k <= inLeft) return left->kth_smallest(b[l], b[r], k);
        return right->kth_smallest(l - b[l], r - b[r], k - inLeft);
    }
    // kth largest element in [l, r)
    int kth_largest(int l, int r, int k) {
        if (l > r) return 0;
        if (lo == hi) return lo;
        int inRight = r - l - b[r] + b[l];
        if (k <= inRight) return right->kth_largest(l - b[l], r - b[r], k);
        return left->kth_largest(b[l], b[r], k - inRight);
    }
    // count of nos in [l, r) Less than or equal to k
    int LTE(int l, int r, int k) {
        if (l > r || k < lo) return 0;
        if (hi <= k) return r - l;
        return left->LTE(b[l], b[r], k) + right->LTE(l - b[l], r - b[r], k);
    }
    // count of nos in [l, r) equal to k
    int count(int l, int r, int k) {
        if (l > r || k < lo || k > hi) return 0;
        if (lo == hi) return r - l;
        if (k <= mid) return left->count(b[l], b[r], k);
        return right->count(l - b[l], r - b[r], k);
    }
};
