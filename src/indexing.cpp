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