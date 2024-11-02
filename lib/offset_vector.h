template <typename V> struct OffsetVector {
    // Index should be in [minIndex, maxIndex].
    // minIndex and maxIndex can be negative.
    OffsetVector(int minIndex, int maxIndex): x(maxIndex - minIndex + 1), offset(minIndex) {}
    V & operator [] (int index) { return x[index - offset]; }
    auto begin() { return x.begin(); }
    auto end() { return x.end(); }
    auto size() { return x.size(); }
private:
    vector <V> x;
    int offset;
};
