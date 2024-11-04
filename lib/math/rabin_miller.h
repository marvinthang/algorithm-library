namespace RABIN_MILLER {
unsigned mod_mult(unsigned a, unsigned b, unsigned m) { return (uint64_t) a * b % m; }
uint64_t mod_mult64(uint64_t a, uint64_t b, uint64_t m) { return __int128_t(a) * b % m; }
unsigned mod_pow(unsigned a, uint64_t b, unsigned m) {
    unsigned ret = 1;
    for (; b; b >>= 1, a = mod_mult(a, a, m)) if (b & 1) ret = mod_mult(ret, a, m);
    return ret;
}
uint64_t mod_pow64(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t ret = 1;
    for (; b; b >>= 1, a = mod_mult64(a, a, m)) if (b & 1) ret = mod_mult64(ret, a, m);
    return ret;
}
bool is_prime32(unsigned n) {
    if (n <= 3) return (n >= 2);
    static const unsigned small[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
        71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139,
    };
    for (size_t i = 0; i < 34; ++i) if (n%small[i] == 0) return n == small[i];
    static const unsigned millerrabin[] = {2, 7, 61};
    unsigned s = n - 1, r = 0;
    while (!(s & 1)) s >>= 1, ++r;
    for (size_t i = 0, j; i < 3; ++i) {
        unsigned md = mod_pow(millerrabin[i], s, n);
        if (md == 1) continue;
        for (j = 1; j < r && md != n - 1; ++j) md = mod_mult(md, md, n);
        if (md != n - 1) return false;
    }
    return true;
}
bool is_prime64(uint64_t n) {
    if (n <= 3) return (n >= 2);
    static const uint64_t small[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
        71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139,
        149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199
    };
    for (size_t i = 0; i < 46; ++i) if (n % small[i] == 0) return n == small[i];
    static const uint64_t millerrabin[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    static const uint64_t A014233[] = {
        2047LL, 1373653LL, 25326001LL, 3215031751LL, 2152302898747LL,
        3474749660383LL, 341550071728321LL, 341550071728321LL,
        3825123056546413051LL, 3825123056546413051LL, 3825123056546413051LL, 0
    };
    uint64_t s = n - 1, r = 0;
    while (!(s & 1)) s >>= 1, ++r;
    for (size_t i = 0, j; i < 11; ++i) {
        uint64_t md = mod_pow64(millerrabin[i], s, n);
        if (md != 1) {
            for (j = 1; j < r && md != n - 1; ++j) md = mod_mult64(md, md, n);
            if (md != n - 1) return false;
        }
        if (n < A014233[i]) return true;
    }
    return true;
}
} using namespace RABIN_MILLER;
