#include <Rcpp.h>

#include <vector>

#include "common.hpp"

using namespace Rcpp;

//' Extract a slice of an rle vector
//'
//' @param rle An R rle list object
//' @param start The 1-based starting index
//' @param end The 1-based ending index
//' @return A sliced R rle list object
//' @examples
//' rle_in <- list(lengths = c(3, 3, 3), values = c(10, 20, 30))
//' 
//' res1 <- rle_slice(rle_in, 2, 7)
//' stopifnot((all.equal(res1, list(lengths = c(2, 3, 1), values = c(10, 20, 30))))
//' 
//' res2 <- rle_slice(rle_in, 1, 9)
//' stopifnot(all.equal(res2, rle_in))
//' 
//' res3 <- rle_slice(rle_in, 5, 5)
//' stopifnot(all.equal(res3, list(lengths = 1, values = 20)))
//' res4 <- rle_slice(rle_in, 8, 20)
//' stopifnot(all.equal(res4, list(lengths = 2, values = 30)))
//' @export
// [[Rcpp::export]]
List rle_slice(List rle, size_t start, size_t end) {
    NumericVector vals = rle["values"];
    IntegerVector lens = rle["lengths"];
    size_t n = vals.size();

    std::vector<double> out_vals;
    std::vector<size_t> out_lens;

    size_t current_pos = 1;
    for (size_t i = 0; i < n; ++i) {
        size_t run_end = current_pos + lens[i] - 1;

        if (run_end >= start && current_pos <= end) {
            size_t overlap_start = (current_pos > start) ? current_pos : start;
            size_t overlap_end = (run_end < end) ? run_end : end;
            size_t overlap_len = overlap_end - overlap_start + 1;

            if (!out_vals.empty() &&
                dot_rle_is_same(out_vals.back(), vals[i])) {
                out_lens.back() += overlap_len;
            } else {
                out_vals.push_back(vals[i]);
                out_lens.push_back(overlap_len);
            }
        }
        current_pos += lens[i];
        if (current_pos > end) break;
    }

    return List::create(Named("lengths") = wrap(out_lens),
                        Named("values") = wrap(out_vals));
}

//' Find indices of TRUE (1.0) values in an RLE object
//'
//' @param rle An R rle list object (where 1.0 represents TRUE)
//' @return An integer vector of 1-based indices
//' @examples
//' rle_in <- list(lengths = c(2, 1, 2), values = c(0, 1, 1))
//' res1 <- rle_which(rle_in)
//' stopifnot(all.equal(res1, c(3, 4, 5)))
//' 
//' rle_false <- list(lengths = c(5), values = c(0))
//' res2 <- rle_which(rle_false)
//' stopifnot(all.equal(res2, integer(0)))
//' 
//' rle_na <- list(lengths = c(2, 1), values = c(NA_real_, 1))
//' res3 <- rle_which(rle_na)
//' stopifnot(all.equal(res3, 3))
//' @export
// [[Rcpp::export]]
IntegerVector rle_which(List rle) {
    NumericVector vals = rle["values"];
    IntegerVector lens = rle["lengths"];
    int n = vals.size();

    int true_count = 0;
    for (int i = 0; i < n; ++i) {
        if (!NumericVector::is_na(vals[i]) && vals[i] == 1.0) {
            true_count += lens[i];
        }
    }

    IntegerVector out(true_count);
    int idx = 0;
    int current_pos = 1;

    for (int i = 0; i < n; ++i) {
        if (!NumericVector::is_na(vals[i]) && vals[i] == 1.0) {
            for (int j = 0; j < lens[i]; ++j) {
                out[idx++] = current_pos + j;
            }
        }
        current_pos += lens[i];
    }

    return out;
}