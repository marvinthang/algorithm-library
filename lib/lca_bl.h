struct LCA {
    int n, root, t, log;
    vector <vector <int>> adj, anc;
    vector <int> tin, tout, depth;
    LCA(const vector <vector <int>> & _adj, int _root): n(_adj.size()), root(_root), adj(_adj) { build(); }
    LCA(const vector <vector <pair <int, int>>> & _adj, int _root): n(_adj.size()), root(_root) {
        adj.resize(n);
        for (int u = 0; u < (int) adj.size(); ++u) {
            adj[u].reserve(_adj[u].size());
            for (auto [w, v]: _adj[u]) adj[u].push_back(v);
        }
        build();
    }
    void build(void) {
        assert(0 <= root && root < n);
        log = 1;
        while ((1 << log) < n) ++log;
        anc.resize(n, vector<int>(log, -1));
        tin.resize(n);
        tout.resize(n);
        depth.resize(n);
        depth[root] = 0;
        t = 0;
        dfs(root);
    }
    void dfs(int u) {
        tin[u] = t++;
        for (int v: adj[u]) if (v != anc[u][0]) {
            anc[v][0] = u;
            for (int i = 1; i < log; ++i) {
                anc[v][i] = anc[anc[v][i - 1]][i - 1];
                if (anc[v][i] == -1) break;
            }
            depth[v] = depth[u] + 1;
            dfs(v);
        }
        tout[u] = t;
    }
    int lca(int u, int v) {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        if (depth[u] < depth[v]) swap(u, v);
        int h = depth[u] - depth[v];
        for (int i = 0; i < log; ++i) if (h >> i & 1) u = anc[u][i];
        if (u == v) return u;
        for (int i = log - 1; i >= 0; --i) if (anc[u][i] != anc[v][i]) {
            u = anc[u][i];
            v = anc[v][i];
        }
        return anc[u][0];
    }
};
