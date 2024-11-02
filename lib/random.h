mt19937 rng(chrono::steady_clock::now().time_since_epoch().count()); 
template <class T> T rand(T l, T h) { return uniform_int_distribution <T> (l, h) (rng); } 
template <class T> T rand(T h) { return uniform_int_distribution <T> (0, h - 1) (rng); } 
