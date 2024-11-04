namespace GRAPH {
template <typename T> class graph {
public:
    struct edge {
        int from, to; T cost;
        edge(int from, int to, T cost): from(from), to(to), cost(cost) {}
    };
    vector <edge> edges;
    int n;
    vector <vector<int>> adj;
    graph(int _n): n(_n), adj(_n) {}
    virtual int add(int from, int to, T cost) = 0;
};
template <typename T> class undigraph: public graph<T> {
public:
    using graph<T>::edges, graph<T>::adj, graph<T>::n;
    undigraph(int _n): graph<T>(_n) {}
    int add(int u, int v, T cost = 1) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        int i = (int) edges.size();
        adj[u].push_back(i);
        adj[v].push_back(i);
        edges.emplace_back(u, v, cost);
        return i;
    }
};
template <typename T> class forest: public graph<T> {
public:
    using graph<T>::edges, graph<T>::adj, graph<T>::n;
    forest(int _n): graph<T>(_n) {}
    int add(int u, int v, T cost = 1) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        int i = (int) edges.size();
        adj[u].push_back(i);
        adj[v].push_back(i);
        edges.emplace_back(u, v, cost);
        return i;
    }
};
template <typename T> class dfs_undigraph: public undigraph<T> {
public:
    using undigraph<T>::edges, undigraph<T>::adj, undigraph<T>::n;
    vector <int> par, par_e, order, tin, tout, sz, root, depth, min_depth, was;
    vector<T> dist;
    int attempt;
    dfs_undigraph(int _n): undigraph<T>(_n) {}
    void init() {
        par = par_e = tin = tout = root = depth = min_depth = was = vector<int>(n, -1);
        order.clear();
        sz = vector<int>(n, 0);
        dist = vector<T>(n);
        attempt = 0;
    }
    void clear() {
        par.clear(); par_e.clear(); order.clear(); tin.clear(); tout.clear(); sz.clear();
        root.clear(); depth.clear(); min_depth.clear(); dist.clear(); was.clear();
    }
private:
    void do_dfs(int u) {
        was[u] = attempt;
        tin[u] = (int) order.size();
        order.push_back(u);
        sz[u] = 1;
        min_depth[u] = depth[u];
        for (int i: adj[u]) if (i != par_e[u]) {
                auto &e = edges[i];
                int v = e.from ^ e.to ^ u;
                if (was[v] == attempt) min_depth[u] = min(min_depth[u], depth[v]);
                else {
                    root[v] = root[u];
                    par[v] = u;
                    par_e[v] = i;
                    depth[v] = depth[u] + 1;
                    dist[v] = dist[u] + e.cost;
                    do_dfs(v);
                    sz[u] += sz[v];
                    min_depth[u] = min(min_depth[u], min_depth[v]);
                }
            }
        tout[u] = order.size();
    }
    void dfs_from(int u) {
        ++attempt;
        depth[u] = 0; dist[u] = T{}; root[u] = u;
        par[u] = par_e[u] = -1;
        do_dfs(u);
    }
public:
    void dfs(int u, bool clear_order = true) {
        if (par.empty()) init();
        else if (clear_order) order.clear();
        dfs_from(u);
    }
    void dfs_all() {
        init();
        for (int u = 0; u < n; u++) if (depth[u] == -1) dfs_from(u);
    }
};
template <typename T> class dfs_forest: public forest<T> {
public:
    using forest<T>::edges, forest<T>::adj, forest<T>::n;
    vector <int> par, par_e, order, tin, tout, sz, root, depth;
    vector<T> dist;
    dfs_forest(int _n): forest<T>(_n) {}
    void init() {
        par = par_e = tin = tout = root = depth = vector<int>(n, -1);
        order.clear();
        sz = vector<int>(n, 0);
        dist = vector<T>(n);
    }
    void clear() {
        par.clear(); par_e.clear(); order.clear(); tin.clear();
        tout.clear(); sz.clear(); root.clear(); depth.clear();
    }
private:
    void do_dfs(int u) {
        tin[u] = order.size();
        order.push_back(u);
        sz[u] = 1;
        for (int i: adj[u]) if (i != par_e[u]) {
                auto &e = edges[i];
                int v = e.from ^ e.to ^ u;
                root[v] = (root[u] != -1 ? root[u]: v);
                par[v] = u;
                par_e[v] = i;
                depth[v] = depth[u] + 1;
                dist[v] = dist[u] + e.cost;
                do_dfs(v);
                sz[u] += sz[v];
            }
        tout[u] = order.size();
    }
    void dfs_from(int u) {
        depth[u] = 0;
        dist[u] = T{};
        root[u] = u;
        par[u] = par_e[u] = -1;
        do_dfs(u);
    }
public:
    void dfs(int u, bool clear_order = true) {
        if (par.empty()) init();
        else if (clear_order) order.clear();
        dfs_from(u);
    }
    void dfs_all() {
        init();
        for (int u = 0; u < n; u++) if (depth[u] == -1) dfs_from(u);
    }
    bool is_anc(int x, int y) { return tin[x] <= tin[y] && tout[y] <= tout[x]; }
};
template <typename T> class lca_binlift_forest: public dfs_forest<T> {
public:
    using dfs_forest<T>::adj, dfs_forest<T>::n, dfs_forest<T>::par, dfs_forest<T>::depth, dfs_forest<T>::is_anc, dfs_forest<T>::root;
    int h;
    vector <vector <int>> anc;
    lca_binlift_forest(int _n): dfs_forest<T>(_n) {}
    void build_lca() {
        assert(!par.empty());
        int max_depth = *max_element(depth.begin(), depth.end());
        h = 1; while ((1 << h) <= max_depth) h++;
        anc.resize(n);
        for (int i = 0; i < n; ++i) {
            anc[i].resize(h);
            anc[i][0] = par[i];
        }
        for (int j = 1; j < h; j++) for (int i = 0; i < n; ++i)
                anc[i][j] = (anc[i][j - 1] == -1 ? -1: anc[anc[i][j - 1]][j - 1]);
    }
    int go_up(int x, int up) {
        up = min(up, (1 << h) - 1);
        for (int j = h; j-- > 0; ) if (up & (1 << j)) {
                x = anc[x][j];
                if (x == -1) break;
            }
        return x;
    }
    int lca(int x, int y) {
        if (root[x] != root[y]) return -1;
        if (is_anc(x, y)) return x;
        if (is_anc(y, x)) return y;
        for (int j = h; j-- > 0; ) if (anc[x][j] != -1 && !is_anc(anc[x][j], y)) x = anc[x][j];
        return anc[x][0];
    }
};
template <typename T> class lca_rmq_forest: public dfs_forest<T> {
public:
    using dfs_forest<T>::edges, dfs_forest<T>::adj, dfs_forest<T>::n, dfs_forest<T>::par, dfs_forest<T>::depth, dfs_forest<T>::root;
    lca_rmq_forest(int _n): dfs_forest<T>(_n) {}
    void build_lca() {
        assert(!par.empty());
        pos.resize(n, -1);
        rmq.emplace_back(0);
        for (int i = 0; i < n; ++i) if (root[i] == i) do_dfs(i);
        int m = rmq[0].size();
        for (int k = 1; (1 << k) <= m; ++k) {
            rmq.emplace_back(m - (1 << k) + 1);
            for (int i = 0; i + (1 << k) <= m; ++i) rmq[k][i] = op(rmq[k - 1][i], rmq[k - 1][i + (1 << (k - 1))]);
        }
    }
private:
    int h;
    vector <vector <int>> rmq;
    vector <int> pos;
    int op(int x, int y) { return depth[x] < depth[y] ? x : y; }
    void do_dfs(int u) {
        pos[u] = rmq[0].size();
        rmq[0].push_back(u);
        for (int i: adj[u]) {
            auto &e = edges[i];
            int v = e.from ^ e.to ^ u;
            if (v == par[u]) continue;
            do_dfs(v);
            rmq[0].push_back(u);
        }
    }
public:
    int lca(int u, int v) {
        if (root[u] != root[v]) return -1;
        u = pos[u]; v = pos[v];
        if (u > v) swap(u, v);
        int h = __lg(v - u + 1);
        return op(rmq[h][u], rmq[h][v - (1 << h) + 1]);
    }
};
template <typename T> class hld_forest: public dfs_forest<T> {
public:
    using dfs_forest<T>::edges, dfs_forest<T>::adj, dfs_forest<T>::n, dfs_forest<T>::par, dfs_forest<T>::depth, dfs_forest<T>::sz, dfs_forest<T>::tin, dfs_forest<T>::order, dfs_forest<T>::root, dfs_forest<T>::dfs, dfs_forest<T>::dfs_all;
    vector <int> head, visited;
    hld_forest(int _n) : dfs_forest<T>(_n), visited(_n) {}
    void build_hld(const vector <int> &vs) {
        for (int tries = 0; tries < 2; ++tries) {
            if (vs.empty()) dfs_all();
            else {
                order.clear();
                for (int v: vs) dfs(v, false);
                assert((int) order.size() == n);
            }
            if (tries) break;
            for (int u = 0; u < n; ++u) {
                if (adj[u].empty()) continue;
                int best = -1, bid = 0;
                for (int i = 0; i < (int) adj[u].size(); ++i) {
                    int id = adj[u][i];
                    int v = edges[id].from ^ edges[id].to ^ u;
                    if (par[v] != u) continue;
                    if (sz[v] > best) {
                        best = sz[v];
                        bid = i;
                    }
                }
                swap(adj[u][0], adj[u][bid]);
            }
        }
        head.resize(n);
        iota(head.begin(), head.end(), 0);
        for (int i = 0; i < n - 1; ++i) {
            int x = order[i];
            int y = order[i + 1];
            if (par[y] == x) head[y] = head[x];
        }
    }
    void build_hld(int v) { build_hld(vector<int>(1, v)); }
    void build_hld_all() { build_hld(vector<int>()); }
    int lca(int u, int v) {
        if (root[u] != root[v]) return -1;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            u = par[head[u]];
        }
        return tin[u] < tin[v] ? u : v;
    }
    int go_up(int x, int up) {
        int target = depth[x] - up;
        if (target < 0) return -1;
        while (depth[head[x]] > target) x = par[head[x]];
        return order[tin[x] - depth[x] + target];
    }
    bool apply_on_path(int u, int v, bool with_lca, function<void(int, int, bool)> f) {
        // f(u, v, up): up -- whether this part of the path goes up
        assert(!head.empty());
        if (root[u] != root[v]) return false;
        int cnt = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) {
                f(tin[head[u]], tin[u], true);
                u = par[head[u]];
            } else {
                visited[cnt++] = v;
                v = par[head[v]];
            }
        }
        if (tin[u] > tin[v]) f(tin[v], tin[u] - !with_lca, true);
        else if (tin[u] + !with_lca <= tin[v]) f(tin[u] + !with_lca, tin[v], false);
        for (int i = cnt - 1; i >= 0; i--) {
            v = visited[i];
            f(tin[head[v]], tin[v], false);
        }
        return true;
    }
};
template <typename T> class digraph: public graph<T> {
public:
    using graph<T>::edges, graph<T>::adj, graph<T>::n;
    digraph(int _n) : graph<T>(_n) {}
    int add(int from, int to, T cost = 1) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        int i = (int) edges.size();
        adj[from].push_back(i);
        edges.push_back({from, to, cost});
        return i;
    }
    digraph<T> reverse() const {
        digraph <T> rev(n);
        for (auto &e : edges) rev.add(e.to, e.from, e.cost);
        return rev;
    }
};
template <typename T> vector <bool> find_bridges(const dfs_undigraph <T> &g) {
    assert(!g.par.empty());
    vector <bool> bridge(g.edges.size());
    for (int i = 0; i < g.n; ++i) if (g.par[i] != -1 && g.min_depth[i] == g.depth[i]) bridge[g.par_e[i]] = true;
    return bridge;
}
template <typename T> vector <bool> find_cutpoints(const dfs_undigraph <T> &g) {
    assert(!g.par.empty());
    vector <bool> cutpoint(g.n);
    for (int i = 0; i < g.n; ++i) if (g.par[i] != -1 && g.min_depth[i] >= g.depth[g.par[i]]) cutpoint[g.par[i]] = true;
    vector <int> children(g.n, 0);
    for (int i = 0; i < g.n; ++i) if (g.par[i] != -1) children[g.par[i]]++;
    for (int i = 0; i < g.n; ++i) if (g.par[i] == -1 && children[i] < 2) cutpoint[i] = false;
    return cutpoint;
}
template <typename T> vector <int> find_bicone(const dfs_undigraph <T> &g, int &cnt) {
    assert(!g.par.empty());
    vector <int> groups(g.n);
    cnt = 0;
    for (int u: g.order) {
        if (g.par[u] == -1 || g.min_depth[u] == g.depth[u]) groups[u] = cnt++;
        else groups[u] = groups[g.par[u]];
    }
    return groups;
}
template <typename T> vector <int> find_biconv(const dfs_undigraph <T> &g, int &cnt) {
    assert(!g.par.empty());
    vector <int> vertex_comp(g.n);
    cnt = 0;
    for (int u: g.order) {
        if (g.par[u] == -1) vertex_comp[u] = -1;
        else if (g.min_depth[u] >= g.depth[g.par[u]]) vertex_comp[u] = cnt++;
        else vertex_comp[u] = vertex_comp[g.par[u]];
    }
    vector <int> edge_comp(g.edges.size(), -1);
    for (int i = 0; i < (int) g.edges.size(); ++i) {
        int x = g.edges[i].from;
        int y = g.edges[i].to;
        edge_comp[i] = vertex_comp[g.depth[x] > g.depth[y] ? x : y];
    }
    return edge_comp;
}
template <typename T> vector <int> find_mst(const undigraph <T> &g, T &res) {
    vector <int> order(g.edges.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&g] (int a, int b) {
        return g.edges[a].cost < g.edges[b].cost;
    });
    vector <int> par(g.n, -1);
    auto find = [&] (auto find, int u) -> int {
        return par[u] < 0 ? u : par[u] = find(find, par[u]);
    };
    auto join = [&] (int u, int v) -> bool {
        if ((u = find(find, u)) == (v = find(find, v))) return false;
        if (par[u] > par[v]) swap(u, v);
        par[u] += par[v];
        par[v] = u;
        return true;
    };
    vector <int> lst;
    res = 0;
    for (int i: order) {
        auto &e = g.edges[i];
        if (join(e.from, e.to)) {
            lst.push_back(i);
            res += e.cost;
        }
    }
    return lst;
}
template <typename T> vector <T> dijkstra(const graph <T> &g, int start) {
    assert(0 <= start && start < g.n);
    vector <T> dist(g.n, numeric_limits<T>::max());
    priority_queue <pair <T, int>, vector <pair <T, int>>, greater <pair <T, int>>> pq;
    pq.emplace(dist[start] = 0, start);
    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (dist[u] != du) continue;
        for (int i: g.adj[u]) {
            auto &e = g.edges[i];
            int v = e.from ^ e.to ^ u;
            if (dist[v] > du + e.cost) pq.emplace(dist[v] = du + e.cost, v);
        }
    }
    return dist;
}
template <typename T> vector <T> dijkstra(const graph <T> &g, int start, vector <int> &trace) {
    assert(0 <= start && start < g.n);
    trace.assign(g.n, -1);
    vector <T> dist(g.n, numeric_limits<T>::max());
    priority_queue <pair <T, int>, vector <pair <T, int>>, greater <pair <T, int>>> pq;
    pq.emplace(dist[start] = 0, start);
    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (dist[u] != du) continue;
        for (int i: g.adj[u]) {
            auto &e = g.edges[i];
            int v = e.from ^ e.to ^ u;
            if (dist[v] > du + e.cost) {
                trace[v] = i;
                pq.emplace(dist[v] = du + e.cost, v);
            }
        }
    }
    return dist;
}
template <typename T> vector <int> find_eulerian_path(const graph <T> &g, int &root) {
    if (g.edges.empty()) {
        root = 0;
        return vector<int>();
    }
    vector <int> in_deg(g.n, 0), out_deg(g.n, 0);
    for (int i = 0; i < (int) g.edges.size(); ++i) {
        auto &e = g.edges[i];
        ++out_deg[e.from]; ++in_deg[e.to];
    }
    root = -1;
    int odd = 0;
    for (int i = 0; i < g.n; ++i) if ((in_deg[i] + out_deg[i]) & 1) {
        odd++;
        if (root == -1 || out_deg[i] - in_deg[i] > out_deg[root] - in_deg[root]) root = i;
    }
    if (odd > 2) {
        root = -1;
        return vector<int>();
    }
    if (root == -1) {
        root = 0;
        while (root < g.n && in_deg[root] + out_deg[root] == 0) root++;
    }
    vector <bool> used(g.edges.size(), false);
    vector <int> ptr(g.n, 0), balance(g.n, 0), res(g.edges.size());
    int stack_ptr = 0, write_ptr = g.edges.size(), u = root;
    while (true) {
        bool found = false;
        while (ptr[u] < (int) g.adj[u].size()) {
            int i = g.adj[u][ptr[u]++];
            if (used[i]) continue;
            used[i] = true;
            res[stack_ptr++] = i;
            auto &e = g.edges[i];
            balance[u]++;
            u ^= e.from ^ e.to;
            balance[u]--;
            found = true;
            break;
        }
        if (!found) {
            if (!stack_ptr) break;
            int i = res[--stack_ptr];
            res[--write_ptr] = i;
            auto &e = g.edges[i];
            u ^= e.from ^ e.to;
        }
    }
    int disbalance = 0;
    for (int i = 0; i < g.n; ++i) disbalance += abs(balance[i]);
    if (write_ptr != 0 || disbalance > 2) {
        root = -1;
        return vector<int>();
    }
    return res;
}
template <typename T> vector <int> find_topsort(const digraph <T> &g) {
    vector <int> deg(g.n, 0), res;
    for (int i = 0; i < (int) g.edges.size(); ++i) ++deg[g.edges[i].to];
    for (int i = 0; i < g.n; ++i) if (!deg[i]) res.push_back(i);
    for (int i = 0; i < (int) res.size(); i++) {
        int u = res[i];
        for (int i: g.adj[u]) {
            int v = g.edges[i].to;
            if (!--deg[v]) res.push_back(v);
        }
    }
    if ((int) res.size() != g.n) return vector<int>();
    return res;
}
template <typename T> vector <int> tarjan(const digraph <T> &g, int &cnt) {
    vector <int> low(g.n, -1), num(g.n, -1), groups(g.n, -1);
    stack <int> st;
    int t = 0;
    cnt = 0;
    auto dfs = [&] (auto dfs, int u) -> void {
        low[u] = num[u] = t++;
        st.push(u);
        for (int i: g.adj[u]) {
            int v = g.edges[i].to;
            if (num[v] == -1) {
                dfs(dfs, v);
                low[u] = min(low[u], low[v]);
            } else low[u] = min(low[u], num[v]);
        }
        if (low[u] == num[u]) {
            int v;
            do {
                v = st.top(); st.pop();
                low[v] = num[v] = g.n;
                groups[v] = cnt;
            } while (v != u);
            ++cnt;
        }
    };
    for (int i = 0; i < g.n; ++i) if (num[i] == -1) dfs(dfs, i);
    return groups;
}
template <typename T> vector <int> kosaraju(const digraph <T> &g, int &cnt) {
    digraph <T> g_rev = g.reverse();
    vector <int> order;
    vector <bool> visited(g.n);
    auto dfs1 = [&] (auto dfs1, int u) -> void {
        visited[u] = true;
        for (int i: g.adj[u]) {
            auto &e = g.edges[i];
            int v = e.to;
            if (!visited[v]) dfs1(dfs1, v);
        }
        order.push_back(u);
    };
    for (int i = 0; i < g.n; ++i) if (!visited[i]) dfs1(dfs1, i);
    vector <int> c(g.n, -1);
    auto dfs2 = [&] (auto dfs2, int u) -> void {
        for (int i: g_rev.adj[u]) {
            auto &e = g_rev.edges[i];
            int v = e.to;
            if (c[v] == -1) {
                c[v] = c[u];
                dfs2(dfs2, v);
            }
        }
    };
    cnt = 0;
    for (int id = g.n; id-- > 0; ) {
        int i = order[id];
        if (c[i] != -1) continue;
        c[i] = cnt++;
        dfs2(dfs2, i);
    }
    return c;
}
template <typename T> vector<int> find_dominators(const digraph <T> &g, int root) {
    int n = g.n;
    vector <int> pos(n, -1), order, parent(n, -1);
    function <void(int)> dfs = [&g, &pos, &order, &parent, &dfs] (int u) {
        pos[u] = (int) order.size();
        order.push_back(u);
        for (int i: g.adj[u]) {
            int v = g.edges[i].to;
            if (pos[v] == -1) {
                parent[v] = u;
                dfs(v);
            }
        }
    };
    dfs(root);
    vector <int> p(n), best(n), sdom = pos;
    iota(p.begin(), p.end(), 0);
    iota(best.begin(), best.end(), 0);
    function <int(int)> find_best = [&p, &best, &sdom, &find_best] (int x) {
        if (p[x] != x) {
            int u = find_best(p[x]);
            if (sdom[u] < sdom[best[x]]) best[x] = u;
            p[x] = p[p[x]];
        }
        if (sdom[best[p[x]]] < sdom[best[x]]) best[x] = best[p[x]];
        return best[x];
    };
    digraph <T> g_rev = g.reverse();
    vector <int> idom(n, -1), link(n, 0);
    vector <vector <int>> bucket(n);
    for (int i = (int) order.size() - 1; i >= 0; i--) {
        int u = order[i];
        for (int i: g_rev.adj[u]) {
            int v = g_rev.edges[i].to;
            if (pos[v] != -1) sdom[u] = min(sdom[u], sdom[find_best(v)]);
        }
        idom[u] = order[sdom[u]];
        for (int v: bucket[u]) link[v] = find_best(v);
        for (int i: g.adj[u]) {
            int v = g.edges[i].to;
            if (parent[v] == u) p[v] = u;
        }
        bucket[order[sdom[u]]].push_back(u);
    }
    for (int i = 1; i < (int) order.size(); ++i) {
        int u = order[i];
        idom[u] = idom[link[u]];
    }
    return idom;
}
} using namespace GRAPH;
