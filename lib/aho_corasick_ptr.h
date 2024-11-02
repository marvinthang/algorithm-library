const int ALPHABET_SIZE = 26;
struct Node {
    Node *parent;
    int parChar;
    Node *children[ALPHABET_SIZE];
    Node *fail, *nxt[ALPHABET_SIZE];
    int cnt;
    Node(Node *parent = nullptr, int parChar = -1): parent(parent), parChar(parChar) {
        fail = nullptr;
        for (int i = 0; i < ALPHABET_SIZE; ++i) children[i] = nxt[i] = nullptr;
        cnt = 0;
    }
};
 
int numNode;
Node *root;
 
void addString(const string &s) {
    Node *p = root;
    REP(i, s.size()) {
        int c = s[i] - 'a';
        if (p->children[c] == nullptr) p->children[c] = new Node(p, c);
        p = p->children[c];
    }
    ++p->cnt;
}
 
void AC_BFS(void) {
    queue <Node*> q;
    root->fail = root;
    root->cnt = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (root->children[i] != nullptr) {
            root->nxt[i] = root->children[i];
            q.push(root->children[i]);
        } else root->nxt[i] = root;
    }
    while (!q.empty()) {
        Node *p = q.front(); q.pop();
        p->fail = p->parent == root ? root : p->parent->fail->nxt[p->parChar];
        p->cnt += p->fail->cnt;
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (p->children[i] != nullptr) {
                p->nxt[i] = p->children[i];
                q.push(p->children[i]);
            } else p->nxt[i] = p->fail->nxt[i];
        }
    }
}
 
