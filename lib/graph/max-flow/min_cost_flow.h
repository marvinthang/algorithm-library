namespace MAX_FLOW {
template <class Cap, class Cost> struct CEdge {
    int from, to;
    Cap cap, flow;
    Cost cost;
    int nxt;
    CEdge(int from = 0, int to = 0, Cap cap = 0, Cost cost = 0, int nxt = 0): from(from), to(to), cap(cap), flow(0), cost(cost), nxt(nxt) {}
};

template <typename Cap, typename Cost> struct MinCostFlow {
    static constexpr Cap inf_cap = numeric_limits<Cap>::max();
    static constexpr Cost inf_cost = numeric_limits<Cost>::max() >> 1;
    int n;
    vector <CEdge <Cap, Cost>> edges;
    vector <Cost> d, pot;
    priority_queue <pair <Cost, int>> q;
    vector <int> trace, head;
    MinCostFlow(int n_ = 0): n(n_), d(n), pot(n, 0), trace(n), head(n, -1) {}
    int add(int from, int to, Cap cap, Cost cost) {
        int id = edges.size();
        edges.emplace_back(from, to, cap, cost, head[from]);
        head[from] = id;
        edges.emplace_back(to, from, 0, -cost, head[to]);
        head[to] = id + 1;
        return id;
    }
    void expath(int st) {
        fill(d.begin(), d.end(), inf_cost);
        while (!q.empty()) q.pop();
        d[st] = 0;
        q.push({pot[st], st});
        while (!q.empty()) {
            auto [du, u] = q.top();
            q.pop();
            if (du != pot[u] - d[u]) continue;
            for (int i = head[u]; i != -1; i = edges[i].nxt) {
                int v = edges[i].to;
                if (edges[i].cap != edges[i].flow && d[u] + edges[i].cost < d[v]) {
                    d[v] = d[u] + edges[i].cost;
                    trace[v] = i;
                    q.push({pot[v] - d[v], v});
                }
            }
        }
        swap(d, pot);
    }
    pair <Cap, Cost> max_flow(int s, int t) {
        Cap flow = 0;
        Cost cost = 0;
        bool ok = true;
        for (auto &e: edges) {
            if (e.cap != e.flow && e.cost + pot[e.from] - pot[e.to] < 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            expath(s);
        } else {
            vector <int> deg(n, 0);
            for (int u = 0; u < n; ++u) {
                for (int i = head[u]; i != -1; i = edges[i].nxt) {
                    if (edges[i].cap != edges[i].flow) ++deg[edges[i].to];
                }
            }
            vector <int> que;
            for (int i = 0; i < n; ++i) if (!deg[i]) que.push_back(i);
            for (int b = 0; b < (int) que.size(); ++b) {
                for (int i = head[que[b]]; i != -1; i = edges[i].nxt) if (edges[i].cap != edges[i].flow) {
                    --deg[edges[i].to];
                    if (!deg[edges[i].to]) que.push_back(edges[i].to);
                }
            }
            fill(pot.begin(), pot.end(), inf_cost);
            pot[s] = 0;
            if ((int) que.size() == n) {
                for (int u: que) if (pot[u] < inf_cost) {
                    for (int i = head[u]; i != -1; i = edges[i].nxt) {
                        if (edges[i].cap != edges[i].flow && pot[u] + edges[i].cost < pot[edges[i].to]) {
                            pot[edges[i].to] = pot[u] + edges[i].cost;
                            trace[edges[i].to] = i;
                        }
                    }
                }
            } else {
                que.assign(1, s);
                vector <bool> in_queue(n, false);
                in_queue[s] = true;
                for (int b = 0; b < (int) que.size(); ++b) {
                    int u = que[b];
                    in_queue[u] = false;
                    for (int i = head[u]; i != -1; i = edges[i].nxt) {
                        if (edges[i].cap != edges[i].flow && pot[u] + edges[i].cost < pot[edges[i].to]) {
                            pot[edges[i].to] = pot[u] + edges[i].cost;
                            trace[edges[i].to] = i;
                            if (!in_queue[edges[i].to]) {
                                que.push_back(edges[i].to);
                                in_queue[edges[i].to] = true;
                            }
                        }
                    }
                }
            }
        }
        while (pot[t] < inf_cost) {
            Cap push = inf_cap;
            for (int u = t; u != s; u = edges[trace[u]].from) {
                push = min(push, edges[trace[u]].cap - edges[trace[u]].flow);
            }
            for (int u = t; u != s; u = edges[trace[u]].from) {
                edges[trace[u]].flow += push;
                edges[trace[u] ^ 1].flow -= push;
            }
            flow += push;
            cost += push * pot[t];
            expath(s);
        }
        return {flow, cost};
    }
};
} using namespace MAX_FLOW;
