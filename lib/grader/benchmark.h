struct Benchmark {
    unsigned long long tim, newtim;

    unsigned long long get_cur_time(){
        return chrono::steady_clock::now().time_since_epoch().count();
    }

    Benchmark() {
        tim = newtim = get_cur_time();
    }

    string section(string name) {
        newtim = get_cur_time();
        stringstream ss;
        ss << fixed << setprecision(9);
        ss << name << ": " << (long double) (newtim - tim) / 1e9;
        tim = newtim;
        string ans;
        getline(ss, ans);
        return ans;
    }
};
