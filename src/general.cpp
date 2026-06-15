#include <Rcpp.h>

#include "common.h"

using namespace Rcpp;

//' Compact an rle object by merging identical runs
//'
//' @param rle An R rle list object
//' @return An R rle list object with identical runs merged
//' @examples
//' uncompacted <- list(lengths = c(2, 3, 1), values = c(5, 5, 10))
//' res1 <- rle_compact(uncompacted)
//' stopifnot(all.equal(res1, list(lengths = c(5, 1), values = c(5, 10))))
//' 
//' compacted <- list(lengths = c(5, 2), values = c(1, 2))
//' stopifnot(all.equal(rle_compact(compacted), compacted))
//' 
//' nas <- list(lengths = c(1, 2), values = c(NA_real_, NA_real_))
//' res2 <- rle_compact(nas)
//' stopifnot(all.equal(res2, list(lengths = 3, values = NA_real_)))
//' @export
// [[Rcpp::export]]
List rle_compact(List rle) {
    NumericVector vals = rle["values"];
    IntegerVector lens = rle["lengths"];
    size_t n = vals.size();

    std::vector<double> out_vals;
    std::vector<size_t> out_lens;

    for (size_t i = 0; i < n; ++i) {
        // this handles case when pushed value is the same as current last
        dot_push_rle(out_vals, out_lens, vals[i], lens[i]);
    }

    return List::create(Named("lengths") = wrap(out_lens),
                        Named("values") = wrap(out_vals));
}

//' Unique values and their frequencies of an rle object
//'
//' @param rle An R rle list object
//' @return A list containing unique values and their total counts
//' @examples
//' rle_in <- list(lengths = c(2, 3, 4), values = c(10, 20, 10))
//' res1 <- rle_value_counts(rle_in)
//' # No guarantee on the order in output
//' stopifnot(all.equal(res1$counts[res1$values == 10], 6))
//' stopifnot(all.equal(res1$counts[res1$values == 20], 3))
//' 
//' empty <- list(lengths = integer(0), values = numeric(0))
//' res3 <- rle_value_counts(empty)
//' stopifnot(all.equal(length(res3$values), 0))
//' @export
// [[Rcpp::export]]
List rle_value_counts(List rle) {
    NumericVector vals = rle["values"];
    IntegerVector lens = rle["lengths"];
    size_t n = vals.size();

    std::unordered_map<double, size_t> counts;
    size_t na_count = 0;

    for (size_t i = 0; i < n; ++i) {
        if (NumericVector::is_na(vals[i])) {
            na_count += lens[i];
        } else {
            counts[vals[i]] += lens[i];
        }
    }

    size_t unique_size = counts.size() + (na_count > 0 ? 1 : 0);
    NumericVector out_vals(unique_size);
    NumericVector out_counts(unique_size);

    size_t idx = 0;
    for (auto const& [val, count] : counts) {
        out_vals[idx] = val;
        out_counts[idx] = count;
        idx++;
    }

    if (na_count > 0) {
        out_vals[idx] = NA_REAL;
        out_counts[idx] = na_count;
    }

    return List::create(Named("values") = out_vals,
                        Named("counts") = out_counts);
}

//' Filter an rle vector by another logical rle vector
//'
//' @param rle_data An R rle list object with data to filter
//' @param rle_mask An R rle list object with logical mask
//' @return A filtered rle list object
//' @export
// [[Rcpp::export]]
List rle_filter(List rle_data, List rle_mask) {
    NumericVector vals_d = rle_data["values"];
    IntegerVector lens_d = rle_data["lengths"];
    NumericVector vals_m = rle_mask["values"];
    IntegerVector lens_m = rle_mask["lengths"];

    size_t n_d = vals_d.size();
    size_t n_m = vals_m.size();

    std::vector<double> out_vals;
    std::vector<size_t> out_lens;

    size_t i = 0, j = 0;
    size_t rem_d = (n_d > 0) ? lens_d[0] : 0;
    size_t rem_m = (n_m > 0) ? lens_m[0] : 0;

    while (i < n_d && j < n_m) {
        size_t step = std::min(rem_d, rem_m);

        if (!NumericVector::is_na(vals_m[j]) && vals_m[j] == 1.0) {
            dot_push_rle(out_vals, out_lens, vals_d[i], step);
        }

        rem_d -= step;
        rem_m -= step;

        if (rem_d == 0 && ++i < n_d) rem_d = lens_d[i];
        if (rem_m == 0 && ++j < n_m) rem_m = lens_m[j];
    }

    return List::create(Named("lengths") = wrap(out_lens),
                        Named("values") = wrap(out_vals));
}