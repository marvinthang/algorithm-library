struct PartiallyPersistentDisjointSet {
    vector <int> lab, t_unite;
    PartiallyPersistentDisjointSet(int n = 0): lab(n + 1, -1), t_unite(n + 1, (int) 1e9) {}
    void reset(void) {
        fill(lab.begin(), lab.end(), -1);
        fill(t_unite.begin(), t_unite.end(), (int) 1e9);
    }
    void resize(int n) {
        lab.assign(n + 1, -1);
        t_unite.assign(n + 1, (int) 1e9);
    }
    bool connected(int t, int u, int v) { return find(t, u) == find(t, v); }
    bool isRoot(int t, int u) { return t_unite[u] > t; }
    int size(int t, int u) { return -lab[find(t, u)]; }
    int find(int t, int u) { return t_unite[u] > t ? u : find(t, lab[u]); }
    bool join(int t, int u, int v) {
        if ((u = find(t, u)) == (v = find(t, v))) return false;
        if (lab[u] > lab[v]) swap(u, v);
        lab[u] += lab[v]; lab[v] = u; t_unite[v] = t;
        return true;
    }
};
using PPDS = PartiallyPersistentDisjointSet;
