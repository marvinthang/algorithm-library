template <size_t bitset_size> void solve(int n) {

}

template <size_t bit = 0> void try_solve(int n) {
    if constexpr(bit < 30) {
        if (n <= (1u << bit)) solve<(1u << bit)>(n);
        else try_solve<bit + 1>(n);
    }
}
