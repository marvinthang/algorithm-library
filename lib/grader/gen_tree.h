#include <bits/stdc++.h>

using namespace std;

vector <pair <int, int>> prufer_decode(const vector <int> &code) {
	int n = (int) code.size() + 2;
	vector <int> deg(n, 0);
	for (int x: code) ++deg[x];
	int ptr = 0; while (deg[ptr]) ++ptr;
	int leaf = ptr;
	vector <pair <int, int>> edges;
	for (int u: code) {
		edges.emplace_back(leaf, u);
		if (!--deg[u] && u < ptr) leaf = u;
		else {
			do ++ptr; while (deg[ptr]);
			leaf = ptr;
		}
	}
	edges.emplace_back(leaf, n - 1);
	return edges;
}

void generate_binary_tree(int u, int par, int sz, int bias, vector <pair <int, int>> &edges) {
	if (!sz) return;
	if (par != -1) edges.emplace_back(par, u);
	int x = rnd.wnext(0, sz / 2, bias);
	int l = sz / 2 - x;
	int r = sz - 1 - l;
	generate_binary_tree(u + 1, u, l, bias, edges);
	generate_binary_tree(u + l + 1, u, r, bias, edges);
}

vector <pair <int, int>> random_tree(int n, int bias, string tree_type, bool _shuffle = true, int first = 1) {
	if (n == 1) return {};
	vector <pair <int, int>> edges;
	if (tree_type == "normal") {
		vector <int> code(n - 2);
		if (!bias) {
			for (int &u: code) u = rnd.next(n);
		} else {
			int number_of_distinct_values = rnd.wnext(1, n, bias);
			vector <int> nodes = rnd.distinct(number_of_distinct_values, 0, n - 1);
			for (auto &u: code) u = rnd.any(nodes);
		}
		edges = prufer_decode(code);
	} else if (tree_type == "binary") {
		generate_binary_tree(0, -1, n, bias, edges);
	} else if (tree_type == "caterpillar") {
        int line_length = rnd.wnext(1, n, bias);
        vector <int> line_nodes = rnd.distinct(line_length, 0, n - 1);
        for (int i = 1; i < (int)line_nodes.size(); i++) 
        	edges.emplace_back(line_nodes[i - 1], line_nodes[i]);
        vector <bool> used(n);
        for (int u: line_nodes) used[u] = true;
        for (int i = 0; i < n; i++) if (!used[i]) 
        	edges.emplace_back(i, rnd.any(line_nodes));
	} else if (tree_type == "sperm") {
        int line_length = rnd.wnext(1, n, bias);
        vector <int> line_nodes = rnd.distinct(line_length, 0, n - 1);
        for (int i = 1; i < (int)line_nodes.size(); i++) 
        	edges.emplace_back(line_nodes[i - 1], line_nodes[i]);
        vector <bool> used(n);
        for (int u: line_nodes) used[u] = true;
        for (int i = 0; i < n; i++) if (!used[i]) 
        	edges.emplace_back(i, line_nodes.back());
	} else if (tree_type == "line") {
        for (int i = 1; i < n; i++) edges.emplace_back(i - 1, i);
    } else __testlib_fail("Type [tree_type = " + tree_type + "] doesn't exist");
	vector <int> lab(rnd.perm(n, first));
	if (_shuffle) {
		shuffle(lab.begin(), lab.end());
		shuffle(edges.begin(), edges.end());
		for (auto &[u, v]: edges) if (rnd.next(2)) swap(u, v);
	}
	for (auto &[u, v]: edges) u = lab[u], v = lab[v];
	return edges;
}

int convert(int l, int r, double per) {
    int f = per * 4;
    if (f == 0) return l;
    if (f == 1) return -sqrt(r);
    if (f == 2) return 0;
    if (f == 3) return sqrt(r);
    return r;
}

void print_tree(vector <pair <int, int>> &edges) {
	cout << (int) edges.size() + 1 << '\n';
	for (auto &[u, v]: edges) cout << u << ' ' << v << '\n';
}

/* Print random weights in range [from,to]. */
void print_weighted_tree(vector <pair <int, int>> &edges, int from, int to) {
	cout << (int) edges.size() + 1 << '\n';
	for (auto &[u, v]: edges) cout << u << ' ' << v << ' ' << rnd.next(from, to) << '\n';
}
