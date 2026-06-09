#include <Rcpp.h>

#include <functional>

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

        if (!out_vals.empty() && out_vals.back() == current_result) {
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
//' @export
// [[Rcpp::export]]
List rle_multiply(List rle_a, List rle_b) {
    auto binary_func = [](double x, double y) { return x * y; };
    return dot_rle_binary_op(rle_a, rle_b, binary_func);
}