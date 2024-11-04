int GaussianElimination(vector <vector <double>> a, vector <double> &res) {
    int n = a.size();
    int m = (int) a[0].size() - 1;
    vector <int> where(m, -1);
    for (int i = 0, j = 0; i < n && j < m; ++j) {
        int pos = i;
        for (int k = i + 1; k < n; ++k) if (abs(a[k][j]) > abs(a[pos][j])) pos = k;
        if (abs(a[pos][j]) < EPS) continue;
        for (int k = j; k <= m; ++k) swap(a[i][k], a[pos][k]);
        where[j] = i;
        for (int k = 0; k < n; ++k) if (k != i) {
            double c = a[k][j] / a[i][j];
            for (int l = j; l <= m; ++l) a[k][l] -= a[i][l] * c;
        }
        ++i;
    }
    res.assign(m, 0);
    for (int i = 0; i < m; ++i) if (where[i] != -1) res[i] = a[where[i]][m] / a[where[i]][i];
    for (int i = 0; i < n; ++i) {
        double sum = 0;
        for (int j = 0; j < m; ++j) sum += res[j] * a[i][j];
        if (abs(sum - a[i][m]) > EPS) return 0;
    }
    for (int i = 0; i < m; ++i) if (where[i] == -1) return 2; // INF
    return 1;
}
