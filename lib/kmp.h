template <typename T> vector <int> kmp_table(int n, const T &s) {
    vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int &j = pi[i];
        j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
    }
    return pi;
}
template <typename T> vector <int> kmp_table(const T &s) { return kmp_table(s.size(), s); }
template <typename T> vector <int> kmp_search(int n, const T &s, int m, const T &w, const vector <int> &pi) {
    assert(n >= 1 && (int) pi.size() == n);
    vector <int> res;
    int j = 0;
    for (int i = 0; i < m; i++) {
        while (j > 0 && (j == n || w[i] != s[j])) j = pi[j - 1];
        if (w[i] == s[j]) j++;
        if (j == n) res.push_back(i - n + 1);
    }
    return res;
}
template <typename T> vector <int> kmp_search(const T &s, const T &w, const vector <int> &pi) { return kmp_search(s.size(), s, w.size(), w, pi); }
