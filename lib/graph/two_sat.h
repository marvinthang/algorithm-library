class TwoSat {
private:
	int n;
	vector <int> order, used, comp;
	vector <bool> _answer;
	vector <vector <int>> adj, r_adj;
	void addEdge(int u, int v) {
		adj[u].push_back(v);
		r_adj[v].push_back(u);
	}
	void dfs1(int u) {
		used[u] = true;
		for (int v: adj[u]) if (!used[v]) dfs1(v);
		order.push_back(u);
	}
	void dfs2(int u, int cl) {
		comp[u] = cl;
		for (int v: r_adj[u]) if (!~comp[v]) dfs2(v, cl);
	}
public:
	TwoSat(int _n = 0) {
		n = _n;
		adj.assign(n << 1, vector<int>());
		r_adj.assign(n << 1, vector<int>());
		used.resize(n << 1);
		comp.resize(n << 1);
	}
	void addChoiceCondition(int var1, int type1, int var2, int type2) {
		// condition type: A or B
		assert(0 <= var1 && var1 < n);
		assert(0 <= type1 && type1 < 2);
		assert(0 <= var2 && var2 < n);
		assert(0 <= type2 && type2 < 2);
		var1 = var1 << 1 | type1;
		var2 = var2 << 1 | type2;
		addEdge(var1 ^ 1, var2);
		addEdge(var2 ^ 1, var1);
	}
	void addMustFalseCondition(int var) {
		// Condition type: A = false
		addChoiceCondition(var, false, var, false);
	}
	void addMustTrueCondition(int var) {
		// Condition type: A = true
		addChoiceCondition(var, true, var, true);
	}
	void addInferCondition(int var1, int type1, int var2, int type2) {
		// Condition type: A -> B
		addChoiceCondition(var1, !type1, var2, type2);
	}
	bool satisfiable(void) {
		order.clear();
		fill(used.begin(), used.end(), false);
		fill(comp.begin(), comp.end(), -1);
		for (int i = 0; i < n << 1; ++i) if (!used[i]) dfs1(i);
		for (int i = n << 1, j = 0; i--; ) {
			int u = order[i];
			if (!~comp[u]) dfs2(u, j++);
		}
		_answer.resize(n);
		for (int i = 0; i < n << 1; i += 2) {
			if (comp[i] == comp[i + 1]) return false;
			_answer[i >> 1] = comp[i] < comp[i + 1];
		}
		return true;
	}
	vector <bool> answer() { return _answer; }
};
