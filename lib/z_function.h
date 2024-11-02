template <typename T> vector <int> z_function(int n, const T &s) {
    if (!n) return {};
    vector <int> z(n);
    z[0] = 0;
    for (int i = 1, j = 0; i < n; i++) {
        int &k = z[i];
        k = j + z[j] <= i ? 0 : min(j + z[j] - i, z[i - j]);
        while (i + k < n && s[k] == s[i + k]) k++;
        if (j + z[j] < i + z[i]) j = i;
    }
    z[0] = n;
    return z;
}
template <typename T> vector <int> z_function(const T &s) { return z_function(s.size(), s); }
