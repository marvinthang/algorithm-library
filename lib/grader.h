#include <bits/stdc++.h>

using namespace std;

#define               fi  first
#define               se  second
#define             left  ___left
#define            right  ___right
#define     scan_op(...)  istream & operator >> (istream &in, __VA_ARGS__ &u)
#define    print_op(...)  ostream & operator << (ostream &out, const __VA_ARGS__ &u)
#ifdef LOCAL
    #include "debug.h"
#else
    #define file(name) if (fopen(name".inp", "r")) { freopen(name".inp", "r", stdin); freopen(name".out", "w", stdout); }
    #define DB(...) 23
    #define db(...) 23
    #define debug(...) 23
#endif

namespace std {
template <class U, class V> scan_op(pair <U, V>) { return in >> u.first >> u.second; }
template <class T> scan_op(vector <T>) { for (size_t i = 0; i < u.size(); ++i) in >> u[i]; return in; }
template <class U, class V> print_op(pair <U, V>) { return out << '(' << u.first << ", " << u.second << ')'; }
template <size_t i, class T> ostream &print_tuple_utils(ostream &out, const T &tup) { if constexpr(i == tuple_size<T>::value) return out << ")"; else return print_tuple_utils<i + 1, T>(out << (i ? ", " : "(") << get<i>(tup), tup); }
template <class...U> print_op(tuple <U...>) { return print_tuple_utils<0, tuple <U...>>(out, u); }
template <class Con, class = decltype(begin(declval<Con>()))>typename enable_if <!is_same<Con, string>::value, ostream &>::type operator << (ostream &out, const Con &con) { out << '{'; for (__typeof(con.begin()) it = con.begin(); it != con.end(); ++it) out << (it == con.begin() ? "" : ", ") << *it; return out << '}'; }
template <class T> print_op(stack <T>) { vector <T> v; stack <T> st = u; while (!st.empty()) v.push_back(st.top()), st.pop(); reverse(v.begin(), v.end()); return out << v; }
template <class T> print_op(queue <T>) { queue <T> q = u; out << '{'; while (!q.empty()) { out << q.front(); q.pop(); if (!q.empty()) out << ", "; } out << '}'; return out; }
template <class T, class X, class Y> print_op(priority_queue <T, X, Y>) { priority_queue <T, X, Y> pq = u; out << '{'; while (!pq.empty()) { out << pq.top(); pq.pop(); if (!pq.empty()) out << ", "; } out << '}'; return out; }
template <class Fun> class y_combinator_result { Fun fun_; public: template <class T> explicit y_combinator_result(T &&fun): fun_(forward<T>(fun)) {} template <class...Args> decltype(auto)operator()(Args &&...args) { return fun_(ref(*this), forward<Args>(args)...); } };
template <class Fun> decltype(auto)y_combinator(Fun &&fun) { return y_combinator_result<decay_t<Fun>>(forward<Fun>(fun)); }
template <typename T, int D> struct Vec: public vector <Vec<T, D - 1>> { static_assert(D >= 1, "Vector dimension must be greater than zero!"); template <typename ...Args> Vec(int n = 0, Args ...args): vector <Vec<T, D - 1>>(n, Vec<T, D - 1>(args...)) {} };
template <typename T> struct Vec<T, 1>: public vector<T>{ Vec(int n = 0, const T &val = T()): vector<T>(n, val) {} };
#if __cplusplus < 202002L
template <class T> int ssize(const T &a) { return a.size(); }
#endif
}

struct Benchmark {
    unsigned long long tim, newtim;
    unsigned long long get_cur_time() { return chrono::steady_clock::now().time_since_epoch().count(); }
    Benchmark() { tim = newtim = get_cur_time(); }
    pair <string, long double> section(string name) {
        newtim = get_cur_time(); long double res = (long double) (newtim - tim) / 1e9;
        stringstream ss; ss << fixed << setprecision(9) << name << ": " << res << "s.";
        tim = newtim; string ans; getline(ss, ans); return pair {ans, res};
    }
};

const string NAME = "${file_name}";
const int NUM_TEST = 1000;
const long double TIME_LIMIT = 10;

void check_status(int status, const string &where) {
    if (!status) return;
    cout << where << " -> program exited abnormally\n";
    exit(0);
}

int main(void) {
    for (int iTest = 1; iTest <= NUM_TEST; ++iTest) {
        cout << "Running on test " << iTest;
        check_status(system("./gen.exe"), "\t./generator");
        Benchmark bm;
        check_status(system(("./" + NAME + ".exe").c_str()), "\t./" + NAME);
        auto bms = bm.section("\ttime");
        // cout << bms.fi; $0
        cout << endl;
        if (bms.se > TIME_LIMIT) {
            cout << "Time limit exceeded on test " << iTest << '\n';
            return 0;
        }
        check_status(system(("./" + NAME + "_bf.exe").c_str()), "\t./" + NAME + "_bf");
        if (system(("diff -w " + NAME + ".out " + NAME + ".ans").c_str())) {
            cout << "Wrong answer on test " << iTest << '\n';
            return 0;
        }
        if (iTest % 30 == 0) system("clear");
    }
    cout << "Accepted!\n";
    return 0;
}
