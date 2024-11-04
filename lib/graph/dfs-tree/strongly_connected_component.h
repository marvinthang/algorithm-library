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

struct StronglyConnectedComponent {
    int n;
    vector <pair <int, int>> edges;
    vector <int> low, num;
    StronglyConnectedComponent(int _n) : n(_n) {}
    void add(int from, int to) { edges.emplace_back(from, to); }
    pair <int, vector <int>> scc_ids() {
        auto g = csr<int>(n, edges);
        int t = 0, group_num = 0;
        low.resize(n);
        num.assign(n, -1);
        vector <int> visited, ids(n);
        visited.reserve(n);
        auto dfs = [&] (auto self, int u) -> void {
            low[u] = num[u] = t++;
            visited.push_back(u);
            for (int i = g.st[u]; i < g.st[u + 1]; i++) {
                int v = g.lst[i];
                if (num[v] == -1) {
                    self(self, v);
                    low[u] = min(low[u], low[v]);
                } else low[u] = min(low[u], num[v]);
            }
            if (low[u] == num[u]) {
                int v;
                do {
                    v = visited.back(); visited.pop_back();
                    num[v] = n;
                    ids[v] = group_num;
                } while (v != u);
                group_num++;
            }
        };
        for (int i = 0; i < n; i++) if (num[i] == -1) dfs(dfs, i);
        for (auto &x: ids) x = group_num - 1 - x;
        return {group_num, ids};
    }
    vector <vector <int>> scc() {
        auto [group_num, ids] = scc_ids();
        vector<int> cnt(group_num);
        for (int x: ids) cnt[x]++;
        vector<vector<int>> groups(group_num);
        for (int i = 0; i < group_num; i++) groups[i].reserve(cnt[i]);
        for (int i = 0; i < n; i++) groups[ids[i]].push_back(i);
        return groups;
    }
};
