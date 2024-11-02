const int ALPHABET_SIZE = 26;
struct Node {
    int parent, parChar;
    int children[ALPHABET_SIZE];
    int fail, nxt[ALPHABET_SIZE];
    int cnt;
    Node(int parent = -1, int parChar = -1): parent(parent), parChar(parChar) {
        fail = -1;
        cnt = 0;
        for (int i = 0; i < ALPHABET_SIZE; ++i) children[i] = nxt[i] = -1;
    }
};
 
vector <Node> nodes;

void addString(const string &s) {
    int p = 0;
    REP(i, s.size()) {
        int c = s[i] - 'a';
        if (!~nodes[p].children[c]) {
            nodes[p].children[c] = nodes.size();
            nodes.push_back(Node(p, c));
        }
        p = nodes[p].children[c];
    }
    ++nodes[p].cnt;
}
 
void AC_BFS(void) {
    queue <int> q;
    nodes[0].fail = 0;
    nodes[0].cnt = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (~nodes[0].children[i]) {
            nodes[0].nxt[i] = nodes[0].children[i];
            q.push(nodes[0].children[i]);
        } else nodes[0].nxt[i] = 0;
    }
    while (!q.empty()) {
        int p = q.front(); q.pop();
        nodes[p].fail = !nodes[p].parent ? 0 : nodes[nodes[nodes[p].parent].fail].nxt[nodes[p].parChar];
        nodes[p].cnt += nodes[nodes[p].fail].cnt;
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (~nodes[p].children[i]) {
                nodes[p].nxt[i] = nodes[p].children[i];
                q.push(nodes[p].children[i]);
            } else nodes[p].nxt[i] = nodes[nodes[p].fail].nxt[i];
        }
    }
}
