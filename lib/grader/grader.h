#include <bits/stdc++.h>

using namespace std;

struct Benchmark {
    unsigned long long last;

    static unsigned long long now() {
        return chrono::steady_clock::now().time_since_epoch().count();
    }

    Benchmark(): last(now()) {}

    pair <string, long double> section(const string &name) {
        unsigned long long current = now();
        long double elapsed = (long double) (current - last) / 1e9;
        stringstream ss;
        ss << fixed << setprecision(9) << name << ": " << elapsed << "s.";
        last = current;
        return {ss.str(), elapsed};
    }
};

const string NAME = "${file_name}";
const int NUM_TEST = 1000;
const long double TIME_LIMIT = 10;

#ifdef _WIN32
const string EXECUTABLE_SUFFIX = ".exe";
const string CLEAR_COMMAND = "cls";
#else
const string EXECUTABLE_SUFFIX = "";
const string CLEAR_COMMAND = "clear";
#endif

int run_command(const string &command) {
    return system(command.c_str());
}

void check_status(int status, const string &where) {
    if (!status) return;
    cout << where << " -> program exited abnormally\n";
    exit(0);
}

int main() {
    for (int iTest = 1; iTest <= NUM_TEST; ++iTest) {
        cout << "Running on test " << iTest;
        check_status(run_command("./gen" + EXECUTABLE_SUFFIX), "\t./generator");

        Benchmark bm;
        check_status(run_command("./" + NAME + EXECUTABLE_SUFFIX), "\t./" + NAME);
        auto bms = bm.section("\ttime");
        // cout << bms.first;
        cout << '\n';

        if (bms.second > TIME_LIMIT) {
            cout << "Time limit exceeded on test " << iTest << '\n';
            return 0;
        }

        check_status(run_command("./" + NAME + "_bf" + EXECUTABLE_SUFFIX), "\t./" + NAME + "_bf");
        if (run_command("diff -w " + NAME + ".out " + NAME + ".ans")) {
            cout << "Wrong answer on test " << iTest << '\n';
            return 0;
        }
        if (iTest % 30 == 0) run_command(CLEAR_COMMAND);
    }
    cout << "Accepted!\n";
    return 0;
}
