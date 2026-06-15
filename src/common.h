#include <Rcpp.h>

using namespace Rcpp;

inline bool dot_rle_is_same(double a, double b) {
    bool na_a = NumericVector::is_na(a);
    bool na_b = NumericVector::is_na(b);
    if (na_a && na_b) return true;
    if (na_a || na_b) return false;
    return a == b;
}

inline void dot_push_rle(std::vector<double>& vals, std::vector<size_t>& lens,
                         double val, size_t len) {
    if (len <= 0) return;
    bool na_val = NumericVector::is_na(val);

    if (!vals.empty()) {
        bool na_back = NumericVector::is_na(vals.back());
        // if value matches the last, just increase last length
        if ((na_val && na_back) ||
            (!na_val && !na_back && vals.back() == val)) {
            lens.back() += len;
            return;
        }
    }
    vals.push_back(val);
    lens.push_back(len);
}