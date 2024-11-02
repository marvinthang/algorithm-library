// not tested yet
template<
    class T,  // for segment & coordinates data types, e.g. long long
    class TM  // for intermediate computations, e.g. __int128_t
> 
struct LichaoTree {
    struct Line {
        T a, b;
        int id;
        bool is_valid;
        Line(T a, T b, int id, bool is_valid): a(a), b(b), id(id), is_valid(is_valid) {}
        TM eval(T x) { return TM(a) * x + b; }
    };
    struct Result {
        T line_a, line_b;
        int line_id;
        bool is_valid;
        TM min_val;
        Result(T a, T b, int id, bool iv, TM m): line_a(a), line_b(b), line_id(id), is_valid(iv), min_val(m) {}
    };
    int n, size, log;
    vector <T> xs;
    vector <Line> lines;
    LichaoTree(const vector <T> &_xs): xs(_xs) {
        sort(xs.begin(), xs.end());
        xs.erase(unique(ALL(xs)), xs.end());
        n = xs.size();
        log = 0;
        while ((1 << log) < n) ++log;
        size = 1 << log;
        xs.resize(size, xs.back());
        lines.assign(size << 1, Line(0, 0, -1, false));
    }
    void update(int i, int l, int r, int u, int v, Line line) {
        if (l >= v || r <= u) return;
        if (u <= l && r <= v) {
            if (!lines[i].is_valid) {
                lines[i] = line;
                return;
            }
            int m = l + r >> 1;
            bool lef = line.eval(l) < lines[i].eval(l);
            bool mid = line.eval(m) < lines[i].eval(m);
            if (mid) swap(lines[i], line);
            if (r - l == 1) return;
            i << 1;
            if (lef != mid) i <<= 1, r = m;
            else i = i << 1 | 1, l = m;
            return;
        }
        int m = l + r >> 1;
        update(i << 1, l, m, u, v, line);
        update(i << 1 | 1, m, r, u, v, line);
    };
    void add_line(T a, T b, int id = -1) {
        update(1, 0, size, 0, size, Line(a, b, id, true));
    }
    void add_segment(T l, T r, T a, T b, int id = -1) {
        l = lower_bound(ALL(xs), l) - xs.begin();
        r = upper_bound(ALL(xs), r) - xs.begin();
        if (l >= r) return;
        update(1, 0, size, l, r, Line(a, b, id, true));
    }
    Result get(T x) {
        int i = lower_bound(xs.begin(), xs.end(), x) - xs.begin();
        Line res = lines[i];
        TM val = res.eval(x);
        for (i += size; (i >>= 1) > 0; ) if (lines[i].is_valid) {
            TM cur = lines[i].eval(x);
            if (!res.is_valid || val > cur) res = lines[i], val = cur;
        }
        return Result(res.a, res.b, res.id, res.is_valid, val);
    }
};
