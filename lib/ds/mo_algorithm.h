long long gilbertOrder(int x, int y, int p, int r) {
    if (p == 0) return 0;
    int h = 1 << (p - 1);
    int s = x < h ? y < h ? 0 : 3 : y < h ? 1 : 2;
    s = (s + r) & 3;
    const int d[4] = {3, 0, 0, 1};
    int nx = x & (x ^ h), ny = y & (y ^ h);
    int nrot = (r + d[s]) & 3;
    long long t = 1LL << (2 * p - 2);
    long long ans = s * t;
    long long add = gilbertOrder(nx, ny, p - 1, nrot);
    ans += s == 1 || s == 2 ? add : t - add - 1;
    return ans;
}

struct Query {
    int l, r, idx;
    long long ord;
    void calcOrder() { ord = gilbertOrder(l, r, 21, 0); }
};
