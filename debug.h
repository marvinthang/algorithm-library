bool I_want_colored_output = true; /* ONLY WORKS WITH TERMINAL */
string red = "\033[1;31m";
string green = "\033[1;32m";
string yellow = "\033[1;33m";
string blue = "\033[0;34m";
string purple = "\033[1;35m";
string cyan = "\033[1;36m";
string white = I_want_colored_output ? "\033[0;m" : "";
string outer = I_want_colored_output ? red : "";
string varName = I_want_colored_output ? cyan : "";
string varValue = I_want_colored_output ? yellow : "";

#define       db(val)  "[" #val " = " << (val) << "] "
#define CONCAT_(x, y)  x##y
#define  CONCAT(x, y)  CONCAT_(x, y)
#define          clog  cerr << setw(__db_level * 4) << setfill(' ') << ""
#define          DB()  debug_block CONCAT(dbbl, __LINE__)

int __db_level = 0;
struct debug_block {
    debug_block() {
        clog << "{" << endl;
        ++__db_level;
    }
    ~debug_block() {
        --__db_level;
        clog << "}" << endl;
    }
};

vector <string> vec_splitter(string s) {
    vector <string> res;
    int bracket = 0;
    for (size_t i = 0, last = 0; i <= s.size(); ++i) {
        if (i == s.size() || (s[i] == ',' && !bracket)) {
            while (last < i && s[last] == ' ') ++last;
            res.push_back(s.substr(last, i - last));
            last = i + 1;
        } else if (s[i] == '(' || s[i] == '<' || s[i] == '{') bracket++;
        else if (s[i] == ')' || s[i] == '>' || s[i] == '}') bracket--;
    }
    return res;
}

template <class... T>
void debug_out(vector <string> v, T&&... args) {
    int idx = 0;
    (..., (cerr << '[' << varName << v[idx++] << outer << " = " << varValue << args << outer << "] "));
    cerr << '\n' << white;
}
#define debug(...) { clog << outer << "L" << __LINE__ << outer << ": "; debug_out(vec_splitter(#__VA_ARGS__), __VA_ARGS__); }
// #define debug(...) { clog << outer << "L" << __LINE__ << " - " << green << __func__ << "()" << outer << ": "; debug_out(vec_splitter(#__VA_ARGS__), __VA_ARGS__); }