template <typename T> vector <int> manacher(int n, const T &s) {
    if (!n) return vector<int>();
    vector <int> res(2 * n - 1);
    int l = -1, r = -1;
    for (int z = 0; z < 2 * n - 1; z++) {
        int i = (z + 1) >> 1;
        int j = z >> 1;
        int p = (i >= r ? 0 : min(r - i, res[2 * (l + r) - z]));
        while (j + p + 1 < n && i - p - 1 >= 0 && s[j + p + 1] == s[i - p - 1]) ++p;
        if (j + p > r) l = i - p, r = j + p;
        res[z] = p;
    }
    return res;
    // res[2 * i] = odd radius in position i
    // res[2 * i + 1] = even radius between positions i and i + 1
}
template <typename T> vector<int> manacher(const T &s) { return manacher(s.size(), s); }
