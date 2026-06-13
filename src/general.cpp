#include <Rcpp.h>

#include "common.hpp"

using namespace Rcpp;

//' Compact an RLE object by merging identical runs
//'
//' @param rle An R rle list object
//' @return An R rle list object with identical runs merged
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

//' Unique values and their frequencies of an RLE object
//'
//' @param rle An R rle list object
//' @return A list containing unique values and their total counts
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