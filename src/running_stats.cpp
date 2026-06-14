#include <Rcpp.h>

#include <vector>

#include "common.hpp"

using namespace Rcpp;

template <typename F>
List dot_rle_cumagg(List rle, F agg_func, double agg_init) {
    NumericVector vals = rle["values"];
    IntegerVector lens = rle["lengths"];
    size_t n = vals.size();

    std::vector<double> out_vals;
    std::vector<size_t> out_lens;

    bool has_na = false;
    double agg = agg_init;
    for (size_t i = 0; i < n; ++i) {
        // TODO: add skip_na parametr to change this behaviour
        if (has_na || NumericVector::is_na(vals[i])) {
            has_na = true;
            dot_push_rle(out_vals, out_lens, NA_REAL, lens[i]);
        } else {
            agg = agg_func(agg, vals[i]);
            dot_push_rle(out_vals, out_lens, agg, lens[i]);
        }
    }
    return List::create(Named("lengths") = wrap(out_lens),
                        Named("values") = wrap(out_vals));
}

//' Cumulative Max of an rle object
//'
//' @param rle An R rle list object
//' @return An R rle list object
//' @examples
//' rle_in <- list(lengths = c(2, 2, 2), values = c(5, 3, 7))
//' res1 <- rle_cummax(rle_in)
//' stopifnot(all.equal(res1, list(lengths = c(4, 2), values = c(5, 7))))
//' 
//' rle_na1 <- list(lengths = c(2, 2), values = c(NA_real_, 5))
//' res2 <- rle_cummax(rle_na1)
//' stopifnot(all.equal(res2, list(lengths = 4L, values = NA_real_)))
//' 
//' rle_na2 <- list(lengths = c(2, 2, 2), values = c(5, NA_real_, 10))
//' res3 <- rle_cummax(rle_na2)
//' stopifnot(all.equal(res3, list(lengths = c(2L, 4L), values = c(5, NA_real_))))
//' @export
// [[Rcpp::export]]
List rle_cummax(List rle) {
    auto cummax = [](double agg, double val) {
        return (val > agg) ? val : agg;
    };
    return dot_rle_cumagg(rle, cummax,
                          -std::numeric_limits<double>::infinity());
}

//' Cumulative Min of an rle object
//'
//' @param rle An R rle list object
//' @return An R rle list object
//' @examples
//' rle_in <- list(lengths = c(2, 2, 2), values = c(5, 8, 2))
//' res1 <- rle_cummin(rle_in)
//' stopifnot(all.equal(res1, list(lengths = c(4, 2), values = c(5, 2))))
//' 
//' rle_na1 <- list(lengths = c(1, 2), values = c(NA_real_, -5))
//' res2 <- rle_cummin(rle_na1)
//' stopifnot(all.equal(res2, list(lengths = 3L, values = NA_real_)))
//' @export
// [[Rcpp::export]]
List rle_cummin(List rle) {
    auto cummin = [](double agg, double val) {
        return (val < agg) ? val : agg;
    };
    return dot_rle_cumagg(rle, cummin, std::numeric_limits<double>::infinity());
}