#include "grader/generator_pch.h"

using namespace std;

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string program_name = "generator";
    string seed = to_string(chrono::steady_clock::now().time_since_epoch().count());
    char *default_argv[] = {program_name.data(), seed.data()};
    if (argc == 1) {
        registerGen(2, default_argv, 1);
    } else {
        registerGen(argc, argv, 1);
    }

    freopen("${file_name}.inp", "w", stdout);

    return 0;
}
