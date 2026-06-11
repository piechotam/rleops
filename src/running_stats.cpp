#include <Rcpp.h>

#include <vector>

using namespace Rcpp;

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
//' @export
// [[Rcpp::export]]
List rle_cummin(List rle) {
    auto cummin = [](double agg, double val) {
        return (val < agg) ? val : agg;
    };
    return dot_rle_cumagg(rle, cummin, std::numeric_limits<double>::infinity());
}