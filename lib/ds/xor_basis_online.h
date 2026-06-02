namespace XOR_BASIS_ONLINE {
    template <typename T> bool umin(T &a, T b) { if (b < a) return a = b, true; return false; }
    template <typename T> bool umax(T &a, T b) { if (a < b) return a = b, true; return false; }
    template <class T> struct xor_basis_node {
        T val, mask;
        int id, high;
        xor_basis_node(T val, T mask, int id, int high) : val(val), mask(mask), id(id), high(high) {}
    };
    template <class T> void swap_bits(T &x, int a, int b) {
        if ((x >> a & 1) ^ (x >> b & 1)) x ^= (1 << a) ^ (1 << b);
    }
    template <class T> struct xor_basis {
        vector <xor_basis_node <T>> basis;
        bool add(T x, int id) {
            T mask = 0;
            for (auto &i: basis) if (umin(x, x ^ i.val)) mask ^= i.mask;
            if (x) {
                mask |= T(1) << basis.size();
                for (auto &i: basis) if (umin(i.val, i.val ^ x)) i.mask ^= mask;
                basis.emplace_back(x, mask, id, __lg(x));
                return true;
            }
            return false;
        }
        int get_the_same_high_bit(T bits, const vector <T> &val) {
            for (auto &i: basis) if (__builtin_popcount(val[i.id] & bits) & 1) return i.id;
            return -1;
        }
        T rebuild_and_delete(int id) {
            int pos = 0, mn = int(1e9), p2 = 0;
            T bits = 0;
            for (int i = 0; i < (int) basis.size(); ++i) if (basis[i].id == id) pos = i;
            for (int i = 0; i < (int) basis.size(); ++i) if (basis[i].mask >> pos & 1) {
                if (umin(mn, basis[i].high)) p2 = i;
                bits ^= T(1) << basis[i].high;
            }
            if (p2 != pos) {
                swap(basis[p2].id, basis[pos].id);
                for (auto &i: basis) swap_bits(i.mask, pos, p2);
                pos = p2;
            }
            for (int i = 0; i < (int) basis.size(); ++i) if (i != pos && basis[i].mask & (1 << pos)) {
                basis[i].val ^= basis[pos].val;
                basis[i].mask ^= basis[pos].mask;
            }
            T good = (1LL << pos) - 1;
            T other = ((1LL << basis.size()) - 1) ^ (good | (1LL << pos));
            basis.erase(basis.begin() + pos);
            for (auto &i: basis) i.mask = ((i.mask & other) >> 1) | (i.mask & good);
            return bits;
        }
    };
    template <class T, int max_bit> // not inclusive
    struct xor_basis_online {
        vector <xor_basis <T>> basises[max_bit + 1];
        int mx;
        vector <pair <int, int>> where;
        vector <T> val;
        xor_basis_online(): mx(0), cur_id(0) {}
        int cur_id;
        int add(T x) {
            val.push_back(x);
            where.emplace_back(-1, -1);
            int id = cur_id++;
            if (!x) return id;
            for (int i = mx; i >= 0; --i) if (!basises[i].empty() && basises[i].back().add(x, id)) {
                basises[i + 1].push_back(basises[i].back());
                basises[i].pop_back();
                umax(mx, i + 1);
                for (auto &j: basises[i + 1].back().basis)
                    where[j.id] = pair{i + 1, (int) basises[i + 1].size() - 1};
                return id;
            }
            basises[1].push_back(xor_basis<T>());
            basises[1].back().add(x, id);
            where.back() = make_pair(1, (int) (basises[1]).size() - 1);
            umax(mx, 1);
            return id;
        }
        int move_back(int sz, int pos) {
            int to = (int) basises[sz].size() - 1;
            if (to == pos) return pos;
            for (auto &i: basises[sz][pos].basis) where[i.id].second = to;
            for (auto &i: basises[sz][to].basis) where[i.id].second = pos;
            swap(basises[sz][pos], basises[sz][to]);
            return to;
        }
        void del(int id) {
            if (!val[id]) return;
            auto [sz, pos] = where[id];
            pos = move_back(sz, pos);
            while (true) {
                T bits = basises[sz].back().rebuild_and_delete(id);
                int i = sz - 1;
                while (i && basises[i].empty()) --i;
                int new_id = -1;
                if (i > 0) new_id = basises[i].back().get_the_same_high_bit(bits, val);
                if (new_id == -1) {
                    if (sz > 1) {
                        basises[sz - 1].push_back(basises[sz].back());
                        for (auto &j: basises[sz - 1].back().basis)
                            where[j.id] = pair{sz - 1, (int) basises[sz - 1].size() - 1};
                    }
                    basises[sz].pop_back();
                    if (mx > 0 && basises[mx].empty()) --mx;
                    return;
                }
                T cur = val[new_id];
                assert(basises[sz].back().add(cur, new_id));
                int new_sz = i, new_pos = (int) basises[i].size() - 1;
                where[new_id] = pair{sz, pos};
                id = new_id; sz = new_sz; pos = new_pos;
            }
        }
        int size() { return mx; }
    };
}
using XOR_BASIS_ONLINE::xor_basis_node;
using XOR_BASIS_ONLINE::xor_basis;
using XOR_BASIS_ONLINE::xor_basis_online;
