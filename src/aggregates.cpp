#include <Rcpp.h>

#include <limits>
#include <vector>

#include "common.h"

using namespace Rcpp;

//' Total length of the rle object (number of elements before encoding)
//'
//' @param rle An R rle list object
//' @return Total length
//' @examples
//' rle_a <- list(lengths = c(2, 3, 1), values = c(1, 2, 3))
//' stopifnot(rle_length(rle_a) == 6)
//'
//' empty <- list(lengths = integer(0), values = numeric(0))
//' stopifnot(rle_length(empty) == 0)
//' @export
// [[Rcpp::export]]
size_t rle_length(List rle) {
    IntegerVector lens = rle["lengths"];
    size_t total = 0;
    for (size_t l : lens) total += l;
    return total;
}

template <typename F>
double dot_scalar_agg(List rle, F agg_func, double agg_init) {
    NumericVector vals = rle["values"];
    IntegerVector lens = rle["lengths"];
    size_t n = vals.size();

    double agg = agg_init;
    for (size_t i = 0; i < n; ++i) {
        agg = agg_func(vals[i], lens[i], agg);
        // if agg ever becomes NA, we can stop immediately
        if (NumericVector::is_na(agg)) {
            return NA_REAL;
        }
    }
    return agg;
}

//' Sum of all values in an rle object
//'
//' @param rle An R rle list object
//' @param na_rm If TRUE, NA values are ignored.
//' @return Sum of values, or NA if any value is NA and na_rm is FALSE
//' @examples
//' rle_a <- list(lengths = c(2, 3), values = c(4.0, 2.0))
//' stopifnot(rle_sum(rle_a) == 14.0)
//'
//' rle_na <- list(lengths = c(2, 1), values = c(3.0, NA_real_))
//' stopifnot(is.na(rle_sum(rle_na)))
//' stopifnot(rle_sum(rle_na, na_rm = TRUE) == 6.0)
//' @export
// [[Rcpp::export]]
double rle_sum(List rle, bool na_rm = false) {
    auto sum_agg = [na_rm](double val, size_t len, double agg) {
        if (NumericVector::is_na(val)) {
            // if na_rm is true we just skip
            return na_rm ? agg : NA_REAL;
        } else {
            return agg + val * len;
        }
    };
    return dot_scalar_agg(rle, sum_agg, 0.0);
}

//' Mean of all values in an rle object
//'
//' @param rle An R rle list object
//' @param na_rm If TRUE, NA values are ignored. Default FALSE.
//' @return A numeric scalar
//' @examples
//' rle_a <- list(lengths = c(2, 2), values = c(0.0, 4.0))
//' stopifnot(rle_mean(rle_a) == 2.0)
//'
//' rle_na <- list(lengths = c(3, 1), values = c(6.0, NA_real_))
//' stopifnot(is.na(rle_mean(rle_na)))
//' stopifnot(rle_mean(rle_na, na_rm = TRUE) == 6.0)
//' @export
// [[Rcpp::export]]
double rle_mean(List rle, bool na_rm = false) {
    size_t current_len = 0;
    auto mean_agg = [na_rm, &current_len](double val, size_t len, double agg) {
        if (NumericVector::is_na(val)) {
            return na_rm ? agg : NA_REAL;
        } else {
            size_t previous_len = current_len;
            current_len += len;
            return (agg * previous_len + val * len) / current_len;
        }
    };

    return dot_scalar_agg(rle, mean_agg, 0.0);
}
