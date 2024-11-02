struct MaxFlow {

    static const long long INF = 1e18;

    struct Edge {
        int from, to;
        long long capa, flow;
        Edge(int from = 0, int to = 0, long long capa = 0):
            from(from), to(to), capa(capa), flow(0) {}
        long long residual(void) const { return capa - flow; }
        bool saturating(void) { return !residual(); }
    };

    vector <Edge> edges;
    vector <vector <int>> adj;
    vector <int> dist, par;

    MaxFlow(int n) {
        adj.assign(n + 1, vector<int> ());
        dist.resize(n + 1);
        par.resize(n + 1);
    }

    void addEdge(int u, int v, long long c) {
        adj[u].push_back(edges.size());
        edges.push_back(Edge(u, v, c));
        adj[v].push_back(edges.size());
        edges.push_back(Edge(v, u, 0));
    }

    bool findPath(int s, int t) {
        fill(dist.begin(), dist.end(), -1);
        fill(par.begin(), par.end(), -1);
        queue <int> q;
        dist[s] = 0; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int &id: adj[u]) if (edges[id].residual() > 0) {
                int v = edges[id].to;
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    par[v] = id;
                    q.push(v);
                }
            }
        }
        return dist[t] != -1;
    }

    long long getFlow(int s, int t) {
        for (Edge &e: edges) e.flow = 0;
        long long totFlow = 0;
        while (findPath(s, t)) {
            long long delta = INF;
            for (int u = t; u != s; u = edges[par[u]].from)
                delta = min(delta, edges[par[u]].residual());
            totFlow += delta;
            for (int u = t; u != s; u = edges[par[u]].from) {
                edges[par[u]].flow += delta;
                edges[par[u] ^ 1].flow -= delta;
            }
        }
        return totFlow;
    }

    bool saturating(int s, int t) {
        maxFlow(s, t);
        for (int &id: adj[s]) if (edges[id].from == s && !edges[id].saturating()) return false;
        return true;
    }

};
