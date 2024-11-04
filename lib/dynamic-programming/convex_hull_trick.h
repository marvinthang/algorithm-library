struct Line {
    long long a, b;
    Line(long long a = 0, long long b = INF): a(a), b(b) {}
    long long eval(long long x) { return a * x + b; }
};
struct ConvexHullTrick {
    vector <Line> lines; int ptr;
    ConvexHullTrick(): ptr(0) {}
    bool bad(Line a, Line b, Line c) { return (long double) (c.b - a.b) / (a.a - c.a) < (long double) (b.b - a.b) / (a.a - b.a); }
    void addLine(long long a, long long b) {
        Line l(a, b);
        while (lines.size() >= 2 && bad(lines.end()[-2], lines.back(), l)) lines.pop_back();
        lines.push_back(l);
    }
    Line getMin(long long x) {
        if (ptr >= (int) lines.size()) ptr = (int) lines.size() - 1;
        while (ptr < (int) lines.size() - 1 && lines[ptr].eval(x) > lines[ptr + 1].eval(x)) ++ptr;
        return lines[ptr];
    }
};
using CHT = ConvexHullTrick;
