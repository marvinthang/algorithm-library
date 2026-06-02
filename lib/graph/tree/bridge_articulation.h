template <class E> struct csr {
    vector <int> st;
    vector <E> lst;
    explicit csr(int n, const vector <pair <int, E>> &edges): st(n + 1), lst(edges.size()) {
        for (auto e: edges) st[e.first + 1]++;
        for (int i = 1; i <= n; i++) st[i] += st[i - 1];
        auto cnt = st;
        for (auto e: edges) lst[cnt[e.first]++] = e.second;
    }
};

struct BridgeArticulation {
    int n, m, tecc_num, bc_num;
    vector <int> cuts, low, num, tecc_ids;
    vector <vector <int>> bc_groups;
    vector <pair <int, int>> bridges;
    vector <bool> articulation, is_bridge;
    struct edge {
        int to, id;
        edge(int to = -1, int id = -1): to(to), id(id) {}
    };
    vector <pair <int, edge>> edges;
    BridgeArticulation(int _n = 0) : n(_n), m(0) {}
    void add(int u, int v) {
        edges.emplace_back(u, edge(v, m));
        edges.emplace_back(v, edge(u, m++));
    }
    void init() {
        csr <edge> g = csr<edge>(n, edges);
        int t = 0;
        articulation.resize(n);
        is_bridge.resize(m);
        low.resize(n);
        num.assign(n, -1);
        tecc_num = bc_num = 0;
        vector <bool> used(m);
        vector <int> tecc_st, bc_st;
        tecc_ids.resize(n);
        bc_groups.clear();
        for (int r = 0; r < n; r++) if (num[r] == -1) {
            int child = 0;
            auto dfs = [&] (auto dfs, int u) -> void {
                tecc_st.push_back(u);
                bc_st.push_back(u);
                num[u] = t++;
                low[u] = n;
                for (int i = g.st[u]; i < g.st[u + 1]; i++) {
                    int id = g.lst[i].id;
                    if (used[id]) continue;
                    used[id] = true;
                    int v = g.lst[i].to;
                    if (num[v] == -1) {
                        if (u == r) ++child;
                        dfs(dfs, v);
                        if (low[v] >= num[u]) {
                            articulation[u] = true;
                            int w;
                            bc_groups.push_back({u});
                            do {
                                w = bc_st.back(); bc_st.pop_back();
                                bc_groups.back().push_back(w);
                            } while (w != v);
                            ++bc_num;
                        }
                        if (low[v] > num[u]) {
                            is_bridge[id] = true;
                            bridges.emplace_back(u, v);
                        }
                        low[u] = min(low[u], low[v]);
                    } else low[u] = min(low[u], num[v]);
                }
                if (low[u] >= num[u]) {
                    int v;
                    do {
                        v = tecc_st.back(); tecc_st.pop_back();
                        tecc_ids[v] = tecc_num;
                    } while (v != u);
                    ++tecc_num;
                }
            };
            dfs(dfs, r);
            articulation[r] = child > 1;
            if (!child) {
                bc_groups.push_back({r});
                ++bc_num;
            }
        }
        for (int i = 0; i < n; ++i) if (articulation[i]) cuts.push_back(i);
    }
    vector <vector <int>> tecc() {
        vector <int> cnt(tecc_num);
        for (int x: tecc_ids) cnt[x]++;
        vector <vector <int>> groups(tecc_num);
        for (int i = 0; i < tecc_num; i++) groups[i].reserve(cnt[i]);
        for (int i = 0; i < n; i++) groups[tecc_ids[i]].push_back(i);
        return groups;
    }
};
