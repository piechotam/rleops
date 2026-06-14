#include <Rcpp.h>

#include <functional>

#include "common.hpp"

using namespace Rcpp;

template <typename F>
List dot_rle_binary_op(List rle_a, List rle_b, F binary_func) {
    NumericVector vals_a = rle_a["values"];
    IntegerVector lens_a = rle_a["lengths"];
    NumericVector vals_b = rle_b["values"];
    IntegerVector lens_b = rle_b["lengths"];

    size_t n_a = vals_a.size();
    size_t n_b = vals_b.size();

    std::vector<double> out_vals;
    std::vector<int> out_lens;

    size_t i = 0, j = 0;
    size_t rem_a = (n_a > 0) ? lens_a[0] : 0;
    size_t rem_b = (n_b > 0) ? lens_b[0] : 0;

    while (i < n_a && j < n_b) {
        double current_result = binary_func(vals_a[i], vals_b[j]);
        size_t step = (rem_a < rem_b) ? rem_a : rem_b;

        if (!out_vals.empty() &&
            dot_rle_is_same(out_vals.back(), current_result)) {
            out_lens.back() += step;
        } else {
            out_vals.push_back(current_result);
            out_lens.push_back(step);
        }
        rem_a -= step;
        rem_b -= step;

        if (rem_a == 0 && ++i < n_a) {
            rem_a = lens_a[i];
        }
        if (rem_b == 0 && ++j < n_b) {
            rem_b = lens_b[j];
        }
    }

    return List::create(Named("lengths") = wrap(out_lens),
                        Named("values") = wrap(out_vals));
}

//' Add two rle objects, element-wise
//'
//' @param rle_a An R rle list object
//' @param rle_b An R rle list object
//' @return An R rle list object containing the sum
//' @examples
//' rle_a <- list(lengths = c(2, 3), values = c(10, 20))
//' rle_b <- list(lengths = c(3, 2), values = c(5, 15))
//' result <- rle_add(rle_a, rle_b)
//' 
//' stopifnot(all.equal(result$lengths, c(2L, 1L, 2L)))
//' stopifnot(all.equal(result$values, c(15, 25, 35)))
//' @export
// [[Rcpp::export]]
List rle_add(List rle_a, List rle_b) {
    auto binary_func = [](double x, double y) { return x + y; };
    return dot_rle_binary_op(rle_a, rle_b, binary_func);
}

//' Multiply two rle objects, element-wise
//'
//' @param rle_a An R rle list object
//' @param rle_b An R rle list object
//' @return An R rle list object containing the product
//' @examples
//' rle_a <- list(lengths = c(2, 3), values = c(10, 20))
//' rle_b <- list(lengths = c(3, 2), values = c(5, 15))
//' result <- rle_multiply(rle_a, rle_b)
//' 
//' stopifnot(all.equal(result$lengths, c(2, 1, 2)))
//' stopifnot(all.equal(result$values, c(50, 100, 300)))
//' @export
// [[Rcpp::export]]
List rle_multiply(List rle_a, List rle_b) {
    auto binary_func = [](double x, double y) { return x * y; };
    return dot_rle_binary_op(rle_a, rle_b, binary_func);
}

//' Element-wise equality of two rle objects
//'
//' @param rle_a An R rle list object
//' @param rle_b An R rle list object
//' @return An R rle list object containing 1.0 (TRUE), 0.0 (FALSE) or NaN
//' @examples
//' a <- list(lengths = c(2, 2), values = c(10, 20))
//' b <- list(lengths = c(2, 2), values = c(10, 99))
//' res1 <- rle_eq(a, b)
//' stopifnot(all.equal(res1, list(lengths = c(2, 2), values = c(1.0, 0.0))))
//' 
//' c <- list(lengths = c(2), values = c(10))
//' d <- list(lengths = c(1, 1), values = c(NA_real_, 10))
//' res2 <- rle_eq(c, d)
//' stopifnot(all.equal(res2, list(lengths = c(1, 1), values = c(NA_real_, 1.0))))
//' @export
// [[Rcpp::export]]
List rle_eq(List rle_a, List rle_b) {
    auto binary_func = [](double x, double y) {
        if (NumericVector::is_na(x) || NumericVector::is_na(y)) return NA_REAL;
        return (x == y) ? 1.0 : 0.0;
    };
    return dot_rle_binary_op(rle_a, rle_b, binary_func);
}