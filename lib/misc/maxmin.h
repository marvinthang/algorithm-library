template <class A, class B> bool minimize(A &a, B b)  { return a > b ? a = b, true : false; }
template <class A, class B> bool maximize(A &a, B b)  { return a < b ? a = b, true : false; }
