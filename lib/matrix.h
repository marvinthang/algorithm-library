template <class T> struct Matrix {
    int numRow, numCol; vector <T> val;
    // accessors
    typename vector<T>::iterator operator [] (int r) { return val.begin() + r * numCol; }
    inline T & at(int r, int c) { return val[r * numCol + c]; }
    inline T get(int r, int c) const { return val[r * numCol + c]; }
    // constructors
    Matrix() {}
    Matrix(int r, int c): numRow(r), numCol(c), val(r * c) {}
    Matrix(const vector <vector <T>> &d) {
        numRow = d.size();
        numCol = numRow ? d[0].size() : 0;
        for (int i = 0; i < numRow; ++i) {
            assert((int) d[i].size() == numCol);
            copy(d[i].begin(), d[i].end(), back_inserter(val));
        }
    }
    Matrix & set_value(T v) {
        for (int i = 0; i < numRow * numCol; ++i) val[i] = v;
        return *this;
    }
    // convert to 2D vector
    vector <vector <T>> vecvec(void) const {
        vector <vector <T>> res(numRow);
        for (int i = 0; i < numRow; ++i)
            copy(val.begin() + i * numCol, val.begin() + (i + 1) * numCol, back_inserter(res[i]));
        return res;
    }
    operator vector <vector <T>> () const { return vecvec(); }
    static Matrix identity(int n) {
        Matrix res(n, n);
        for (int i = 0; i < n; ++i) res.at(i, i) = T(1);
        return res;
    }
    friend istream & operator >> (istream &in, Matrix &res) {
        for (T &x: res.val) in >> x;
        return in;
    }
    friend ostream & operator << (ostream &out, const Matrix &res) {
        for (int i = 0; i < res.numRow * res.numCol; ++i)
            cout << res.val[i] << " \n"[i % res.numCol == res.numCol - 1];
        return out;
    }
    Matrix operator - (void) {
        Matrix res(numRow, numCol);
        for (int i = 0; i < numRow * numCol; ++i) res.val[i] = -val[i];
        return res;
    }
    Matrix operator * (const T &v) {
        Matrix res = *this;
        for (T &x: res.val) x *= v;
        return res;
    } 
    Matrix operator / (const T &v) {
        Matrix res = *this;
        const T inv = T(1) / v;
        for (T &x: res.val) x *= inv;
        return res;
    }
    Matrix operator + (const Matrix &other) const {
        int M = numRow, N = numCol;
        assert(M == other.numRow); assert(N == other.numCol);
        Matrix res = *this;
        for (int i = 0; i < numRow * numCol; ++i) res.val[i] += other.val[i];
        return res;
    }
    Matrix operator - (const Matrix &other) const {
        int M = numRow, N = numCol;
        assert(M == other.numRow); assert(N == other.numCol);
        Matrix res = *this;
        for (int i = 0; i < numRow * numCol; ++i) res.val[i] -= other.val[i];
        return res;
    }
    Matrix operator * (const Matrix &other) const {
        int M = numRow, N = numCol, P = other.numCol;
        assert(N == other.numRow);
        Matrix t_other = other.transpose();
        Matrix res(M, P);
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < P; ++j)
                res.at(i, j) = inner_product(this->val.begin() + N * i, this->val.begin() + N * (i + 1), t_other.val.begin() + t_other.numCol * j, T(0));
        return res;
    }
    Matrix & operator *= (const T &v) { return *this = *this * v; }
    Matrix & operator /= (const T &v) { return *this = *this / v; }
    Matrix & operator += (const Matrix &other) { return *this = *this + other; }
    Matrix & operator -= (const Matrix &other) { return *this = *this - other; }
    Matrix & operator *= (const Matrix &other) { return *this = *this * other; }
    Matrix pow(long long Exp) const {
        int M = numRow;
        assert(M == numCol); assert(Exp >= 0);
        Matrix res = identity(M);
        if (!Exp) return res;
        bool is_id = true;
        for (int i = 63 - __builtin_clzll(Exp); i >= 0; --i) {
            if (!is_id) res *= res;
            if (Exp >> i & 1) res *= *this, is_id = false;
        }
        return res;
    }
    Matrix transpose(void) const {
        Matrix res(numCol, numRow);
        for (int i = 0; i < numRow; ++i)
            for (int j = 0; j < numCol; ++j)
                res.at(j, i) = this->get(i, j);
        return res;
    }
};
