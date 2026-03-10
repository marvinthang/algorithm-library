struct Blossom {
    int n;
    vector<vector<int>> adj;
    int n_matches;
    vector<int> mate;

    int q_n;
    vector<int> q;
    int book_mark;
    vector<int> book;

    vector<int> type;
    vector<int> fa;
    vector<int> bel;

    Blossom(int n) : n(n), adj(n), n_matches(0), mate(n, -1), q(n), book_mark(0), book(n), type(n), fa(n), bel(n) {}

    void add(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void augment(int u) {
        while (u != -1) {
            int pv = fa[u];
            int ppv = (pv != -1) ? mate[pv] : -1;
            mate[u] = pv;
            mate[pv] = u;
            u = ppv;
        }
    }

    int get_lca(int u, int v) {
        ++book_mark;
        u = bel[u];
        v = bel[v];
        while (true) {
            if (u != -1) {
                if (book[u] == book_mark) return u;
                book[u] = book_mark;
                if (mate[u] != -1) u = bel[fa[mate[u]]];
                else u = -1;
            }
            std::swap(u, v);
        }
    }

    void go_up(int u, int v, int mv) {
        while (bel[u] != mv) {
            fa[u] = v;
            v = mate[u];
            if (type[v] == 1) {
                type[v] = 0;
                q[q_n++] = v;
            }
            if (bel[u] == u) bel[u] = mv;
            if (bel[v] == v) bel[v] = mv;
            u = fa[v];
        }
    }

    bool match(int sv) {
        for (int i = 0; i < n; ++i) {
            bel[i] = i;
            type[i] = -1;
            fa[i] = -1;
        }

        q_n = 0;
        q[q_n++] = sv;
        type[sv] = 0;

        for (int i = 0; i < q_n; ++i) {
            int u = q[i];
            for (int v : adj[u]) {
                if (bel[u] == bel[v] || type[v] == 1) continue;
                if (type[v] == -1) {
                    fa[v] = u;
                    type[v] = 1;
                    if (mate[v] == -1) {
                        augment(v);
                        return true;
                    }
                    type[mate[v]] = 0;
                    q[q_n++] = mate[v];
                } else {
                    int lca = get_lca(u, v);
                    go_up(u, v, lca);
                    go_up(v, u, lca);
                    for (int j = 0; j < n; ++j) {
                        bel[j] = bel[bel[j]];
                    }
                }
            }
        }
        return false;
    }

    int max_matching() {
        n_matches = 0;
        for (int i = 0; i < n; ++i) {
            if (mate[i] == -1 && match(i)) {
                ++n_matches;
            }
        }
        return n_matches;
    }
};
