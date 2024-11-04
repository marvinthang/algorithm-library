namespace SUBSET_CONVOLUTION {
    template <class T> void zeta_transform(vector <T> &a, bool rev = false) {
        int n = 0; while ((1 << n) < (int) a.size()) ++n;
        a.resize(1 << n);
        for (int i = 0; i < n; ++i)
            for (int mask = 0; mask < 1 << n; ++mask)
                if ((mask >> i & 1) ^ rev) a[mask] += a[mask ^ (1 << i)];
    }
    template <class T> void mobius_transform(vector <T> &a, bool rev = false) {
        int n = 0; while ((1 << n) < (int) a.size()) ++n;
        a.resize(1 << n);
        for (int i = 0; i < n; ++i)
            for (int mask = 0; mask < 1 << n; ++mask)
                if ((mask >> i & 1) ^ rev) a[mask] -= a[mask ^ (1 << i)];
    }
    template <class T> vector <T> subset_sum_convolution(const vector <T> &f, const vector <T> &g) {
        int n = 0; while ((1 << n) < (int) max(f.size(), g.size())) ++n;
        vector <vector <T>> fhat(n + 1, vector<T>(1 << n)), ghat(n + 1, vector<T>(1 << n));
        for (int mask = 0; mask < 1 << n; ++mask) {
            int i = __builtin_popcount(mask);
            if (mask < (int) f.size()) fhat[i][mask] = f[mask];
            if (mask < (int) g.size()) ghat[i][mask] = g[mask];
        }
        for (int i = 0; i < n; ++i) {
            zeta_transform(fhat[i]);
            zeta_transform(ghat[i]);
        }
        vector <T> res(1 << n), h(1 << n);
        for (int i = 0; i <= n; ++i) {
            for (int j = 0; j <= i; ++j)
                for (int mask = 0; mask < 1 << n; ++mask)
                        h[mask] += fhat[j][mask] * ghat[i - j][mask];
            mobius_transform(h);
            for (int mask = 0; mask < 1 << n; ++mask)
                if (__builtin_popcount(mask) == i) res[mask] = h[mask];
            fill(h.begin(), h.end(), T{});
        }
        return res;
    }
    template <class T> void fft(vector <T> &a, bool rev = false) {
        int n = a.size();
        for (int i = 1; i < n; i <<= 1) {
            for (int mask = 0; mask < n; ++mask) {
                if (!(mask & i)) {
                    T u = a[mask], v = a[mask ^ i];
                    a[mask] = u + v;
                    a[mask ^ i] = u - v;
                }
            }
        }
        if (rev) {
            T inv = T(1) / n;
            for (int mask = 0; mask < n; ++mask) a[mask] *= inv;
        }
    }
    template <class T> vector <T> or_convolution(vector <T> f, vector <T> g) {
        int n = 1;
        while (n < (int) max(f.size(), g.size())) n <<= 1;
        f.resize(n); g.resize(n);
        zeta_transform(f);
        zeta_transform(g);
        for (int i = 0; i < n; ++i) f[i] *= g[i];
        mobius_transform(f);
        return f;
    }
    template <class T> vector <T> and_convolution(vector <T> f, vector <T> g) {
        int n = 1;
        while (n < (int) max(f.size(), g.size())) n <<= 1;
        f.resize(n); g.resize(n);
        zeta_transform(f, true);
        zeta_transform(g, true);
        for (int i = 0; i < n; ++i) f[i] *= g[i];
        mobius_transform(f, true);
        return f;
    }
    template <class T> vector <T> xor_convolution(vector <T> f, vector <T> g) {
        int n = 1;
        while (n < (int) max(f.size(), g.size())) n <<= 1;
        f.resize(n); g.resize(n);
        fft(f); fft(g);
        for (int i = 0; i < n; ++i) f[i] *= g[i];
        fft(f, true);
        return f;
    }
} using namespace SUBSET_CONVOLUTION;
