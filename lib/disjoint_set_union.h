struct DisjointSet {
    vector <int> par;
    DisjointSet(int n = 0): par(n, -1) {}
    void reset(void) { fill(par.begin(), par.end(), -1); }
    void resize(int n) { par.assign(n, -1); }
    bool connected(int u, int v) { return find(u) == find(v); }
    bool isRoot(int u) { return par[u] < 0; }
    int size(int u) { return -par[find(u)]; }
    int find(int u) { return par[u] < 0 ? u : par[u] = find(par[u]); }
    bool join(int u, int v) {
        if ((u = find(u)) == (v = find(v))) return false;
        if (par[u] > par[v]) swap(u, v);
        par[u] += par[v]; par[v] = u;
        return true;
    }
};
using DSU = DisjointSet;
