#include <Rcpp.h>

using namespace Rcpp;

inline bool dot_rle_is_same(double a, double b) {
    bool na_a = NumericVector::is_na(a);
    bool na_b = NumericVector::is_na(b);
    if (na_a && na_b) return true;
    if (na_a || na_b) return false;
    return a == b;
}