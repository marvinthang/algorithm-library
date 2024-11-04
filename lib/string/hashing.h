namespace HASHING {

    const int NMOD = 2;
    const int MOD[] = {(int) 1e9 + 7, (int) 1e9 + 2277, (int) 1e9 + 5277, 
                            (int) 1e9 + 8277, (int) 1e9 + 9277};
    const int BASE = 227; // 311
    const int capital = 0;
    const int MAX_SIZE = 1e6 + 6;

    int pw[NMOD][MAX_SIZE];

    void prepareHashing(void) {
        for (int j = 0; j < NMOD; ++j) {
            pw[j][0] = 1;
            for (int i = 1; i < MAX_SIZE; ++i)
                pw[j][i] = 1LL * pw[j][i - 1] * BASE % MOD[j];
        }
    }

    struct Value {

            int val[NMOD];
        
            Value(int x = 0) { for (int i = 0; i < NMOD; ++i) val[i] = x; }

            Value(char c, int pos) {
                for (int i = 0; i < NMOD; ++i) 
                    val[i] = 1LL * c * pw[i][pos] % MOD[i];
            }

            Value shift(int step) const {
                Value res;
                for (int i = 0; i < NMOD; ++i)
                    res.val[i] = 1LL * val[i] * pw[i][step] % MOD[i];
                return res;
            }

            Value operator + (const Value &other) const {
                Value res;
                for (int i = 0; i < NMOD; ++i) {
                    res.val[i] = val[i] + other.val[i];
                    if (res.val[i] >= MOD[i]) res.val[i] -= MOD[i];
                }
                return res;
            }

            Value operator - (const Value &other) const {
                Value res;
                for (int i = 0; i < NMOD; ++i) {
                    res.val[i] = val[i] - other.val[i];
                    if (res.val[i] < 0) res.val[i] += MOD[i];
                }
                return res;
            }

            bool operator < (const Value& other) const { 
                for (int i = 0; i < NMOD; ++i)
                    if (val[i] != other.val[i]) return val[i] < other.val[i];
                return false;
            }

            bool operator == (const Value& other) const {
                for (int i = 0; i < NMOD; ++i)
                    if (val[i] != other.val[i]) return false;
                return true;
            }
        
            bool operator != (const Value& other) const {
                for (int i = 0; i < NMOD; ++i) 
                    if (val[i] != other.val[i]) return true;
                return false;
            }
    
    };

    struct Hash {

        string S;
        int len;
        Value *code, *rev_code;

        void Init_Hash(bool palin = false) {
            len = S.size();
            S = ' ' + S;
            code = new Value[len + 1];
            code[0] = 0;
            if (palin) {
                rev_code = new Value[len + 1];
                rev_code[0] = 0;
            }
            for (int i = 1; i <= len; ++i) {
                code[i] = code[i - 1] + Value(S[i] - capital + 1, i);
                if (palin) rev_code[i] = rev_code[i - 1] + Value(S[len - i + 1] - capital + 1, i);
            }
        }

        Hash() {}

        Hash(string s, bool palin = false) {
            S = s;
            Init_Hash(palin);
        }

        int getHash(int l, int r, int mod) const {
            assert(l <= r && l > 0 && r <= len);
            int res = code[r].val[mod] - code[l - 1].val[mod];
            if (res < 0) res += MOD[mod];
            res = 1LL * res * pw[mod][MAX_SIZE - l] % MOD[mod];
            return res;
        }

        Value getHash(int l, int r) const {
            assert(l <= r && l > 0 && r <= len);
            return (code[r] - code[l - 1]).shift(MAX_SIZE - l);
        }

        Value getRevHash(int l, int r) {
            assert(l <= r && l > 0 && r <= len);
            return (rev_code[r] - rev_code[l - 1]).shift(MAX_SIZE - l);
        }

        bool isPalin(int l, int r) {
            return getHash(l, r) == getRevHash(len - r + 1, len - l + 1);
        }

        Value getHash(void) const {
            return getHash(1, len);
        }

        bool operator == (Hash& other) const {
            return code[len] == other.code[other.len];
        }

        bool operator < (Hash& other) const{    
            return code[len] < other.code[other.len];
        }

        bool operator > (Hash& other) const{
            return !(code[len] < other.code[other.len] || code[len] == other.code[other.len]);
        }

        bool cmpSubstr(int l, int r, int u, int v) {
            assert(l <= r && u <= v); assert(l > 0 && u > 0); assert(r <= len && v <= len);
            int len = min(r - l + 1, v - u + 1);
            int max_same = 0;
            int Min = 1, Max = len;
            while (Min <= Max) {
                int mid = Min + Max >> 1;
                Value F1 = getHash(l, l + mid - 1);
                Value F2 = getHash(u, u + mid - 1);
                if (F1 == F2) {
                    max_same = mid;
                    Min = mid + 1;
                } else Max = mid - 1;
            }
            if (max_same == len) return r - l < v - u;
            return S[l + max_same] < S[u + max_same];
        }

    };

    bool cmpStr(Hash a, Hash b) {
        int len = min(a.len, b.len);
        int max_same = 0;
        int l = 1, r = len - 1;
        while (l <= r) {
            int mid = l + r >> 1;
            Value F1 = a.getHash(1, mid);
            Value F2 = b.getHash(1, mid);
            if (F1 == F2) {
                max_same = mid;
                l = mid + 1;
            } else r = mid - 1;
        }
        if (max_same == len) return a.len < b.len;
        return a.S[max_same + 1] < b.S[max_same + 1];
    }

}

using namespace HASHING;
