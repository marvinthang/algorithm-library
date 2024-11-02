struct Point {
    long long x, y;
    Point(long long x = 0, long long y = 0): x(x), y(y) {}
    friend scan_op(Point) { return in >> u.x >> u.y; }
    friend print_op(Point) { return out << make_pair(u.x, u.y); }
    bool operator == (const Point &other) const { return x == other.x && y == other.y; }
    bool operator < (const Point &other) const { return make_pair(x, y) < make_pair(other.x, other.y); }
    Point & operator += (const Point &other) {x += other.x; y += other.y; return *this; }
    Point & operator -= (const Point &other) { x -= other.x; y -= other.y; return *this; }
    Point & operator *= (long long v) { x *= v; y *= v; return *this; }
    Point & operator /= (long long v) { x /= v; y /= v; return *this; }
    Point operator + (const Point &other) const { return Point(*this) += other; }
    Point operator - (const Point &other) const { return Point(*this) -= other; }
    Point operator * (long long v) const { return Point(*this) *= v; }
    Point operator / (long long v) const { return Point(*this) /= v; }
    long long dot(const Point &other) const { return x * other.x + y * other.y; }
    long long cross(const Point &other) const { return x * other.y - y * other.x; }
    long long norm(void) const { return x * x + y * y; }
    double length(void) const { return sqrtl(norm()); }
    double angle(const Point &other) const { return acos(dot(other) / length() / other.length()); }
    long long cross(const Point &a, const Point &b) const { return (a - *this).cross(b - *this); }
};
int sgn(long long val) { return val < 0 ? -1 : !!val; }
int ccw(const Point &a, const Point &b, const Point &c) { return sgn(a.cross(b, c)); }
double linePointDist(const Point &a, const Point &b, const Point &c, bool isSegment) {
    double dist = abs((b - a).cross(c - a)) / (a - b).length();
    if (isSegment) {
        if ((a - b).dot(c - b) < 0) return (b - c).length();
        if ((b - a).dot(c - a) < 0) return (a - c).length();
    }
    return dist;
}
bool inter1(long long a, long long b, long long c, long long d) {
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return max(a, c) <= min(b, d);
}
bool check_inter(const Point &a, const Point &b, const Point &p) {
    return !a.cross(b, p) && inter1(a.x, b.x, p.x, p.x) && inter1(a.y, b.y, p.y, p.y);
}
bool check_inter(const Point &a, const Point &b, const Point &c, const Point &d) {
    if (!c.cross(a, d) && !c.cross(b, d)) 
        return inter1(a.x, b.x, c.x, d.x) && inter1(a.y, b.y, c.y, d.y);
    return ccw(a, b, c) != ccw(a, b, d) && ccw(c, d, a) != ccw(c, d, b);
}
struct Line {
    // ax + by = c
    long long a, b, c;
    Line(long long a = 0, long long b = 0, long long c = 0): a(a), b(b), c(c) {}
    Line(const Point &m, const Point &n) {
        a = m.y - n.y;
        b = n.x - m.x;
        c = a * m.x + b * m.y;
    }
    bool intersect(const Line &other, Point &res) const {
        long long x = a * other.b - b * other.a;
        if (!x) return false;
        res.x = (other.b * c - b * other.c) / x;
        res.y = (a * other.c - c * other.a) / x;
        return true;
    }
    bool parallel(const Line &other) const {
        return a * other.b - b * other.a == 0;
    }
    bool equivalent(const Line &other) const {
        return a * other.b - b * other.a == 0
            && a * other.c - c * other.a == 0
            && b * other.c - c * other.b == 0;
    }
};
void convex_hull(vector <Point> &p) {
    if (p.size() <= 1) return;
    sort(p.begin(), p.end());
    Point p1 = p[0], p2 = p.back();
    vector <Point> up {p1}, down {p1};
    for (int i = 1; i < (int) p.size(); ++i) {
        int c = ccw(p1, p[i], p2);
        if (i == (int) p.size() - 1 || c < 0) {
            while (up.size() > 1 && ccw(up.end()[-2], up.back(), p[i]) >= 0) up.pop_back();
            up.push_back(p[i]);
        }
        if (i == (int) p.size() - 1 || c > 0) {
            while (down.size() > 1 && ccw(down.end()[-2], down.back(), p[i]) <= 0) down.pop_back();
            down.push_back(p[i]);
        }
    }
    p = move(up);
    p.insert(p.end(), down.rbegin() + 1, down.rend() - 1);
    if (p.size() == 2 && p[0] == p[1]) p.pop_back();
}
long long area_x2(const vector <Point> &p) {
    long long res = 0;
    for (int i = 0; i < (int) p.size(); ++i) res += p[i].cross(p[i == (int) p.size() - 1 ? 0 : i + 1]);
    return abs(res);
}
int n;
vector <Point> seq;
Point translation;
void prepareConvexPolygon(vector <Point> &p) {
    if (ccw(p[0], p[1], p[2]) < 0) reverse(p.begin(), p.end());
    n = p.size();
    int pos = 0;
    for (int i = 1; i < n; ++i) if (p[i] < p[pos]) pos = i;
    rotate(p.begin(), p.begin() + pos, p.end());
    --n;
    seq.resize(n);
    translation = p[0];
    for (int i = 0; i < n; ++i) seq[i] = p[i + 1] - translation;
}
bool pointInTriangle(const Point &a, const Point &b, const Point &c, const Point &p) {
    return abs(a.cross(b, c)) == abs(p.cross(a, b)) + abs(p.cross(b, c)) + abs(p.cross(c, a));
}
bool pointInConvexPolygon(Point p) {
    p -= translation;
    if (seq[0].cross(p) < 0) return false;
    if (seq[n - 1].cross(p) > 0) return false;
    if (!seq[0].cross(p)) return seq[0].norm() >= p.norm();
    int l = 0, r = n - 2;
    while (l <= r) {
        int m = (l + r) >> 1;
        if (seq[m].cross(p) > 0) l = m + 1;
        else r = m - 1;
    }
    return pointInTriangle(seq[r], seq[r + 1], Point(0, 0), p);
}
