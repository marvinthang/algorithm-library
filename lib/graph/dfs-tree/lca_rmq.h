struct LCA {
    int n, root;
    vector <vector <int>> adj;
    vector <int> tin, tout, depth;
    vector <pair <int, int>> order;
    vector <vector <int>> rmq;
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
        tin.resize(n);
        tout.resize(n);
        depth.resize(n);
        order.reserve(2 * n - 1);
        depth[root] = 0;
        rmq.emplace_back(0);
        dfs(root, -1);
        int m = rmq[0].size();
        for (int k = 1; (1 << k) <= m; ++k) {
            rmq.emplace_back(m - (1 << k) + 1);
            for (int i = 0; i + (1 << k) <= m; ++i) rmq[k][i] = op(rmq[k - 1][i], rmq[k - 1][i + (1 << (k - 1))]);
        }
    }
    bool is_anc(int x, int y) { return tin[x] <= tin[y] && tout[y] <= tout[x]; }
    int op(int x, int y) { return depth[x] < depth[y] ? x : y; }
    void dfs(int u, int par) {
        tin[u] = rmq[0].size();
        rmq[0].push_back(u);
        for (int v: adj[u]) if (v != par) {
            depth[v] = depth[u] + 1;
            dfs(v, u);
            rmq[0].push_back(u);
        }
        tout[u] = rmq[0].size();
    }
    int lca(int u, int v) {
        assert(0 <= u && u < n);
        assert(0 <= v && v < n);
        u = tin[u]; v = tin[v];
        if (u > v) swap(u, v);
        int h = __lg(v - u + 1);
        return op(rmq[h][u], rmq[h][v - (1 << h) + 1]);
    }
};
