template <class A, class B>   inline A ceil(A a, B b) { return a / b + ((a ^ b) > 0 && a % b); }
template <class A, class B>  inline A floor(A a, B b) { return a / b - ((a ^ b) < 0 && a % b); }
