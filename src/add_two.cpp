#include <Rcpp.h>
using namespace Rcpp;

//' Add Two Numbers
//' 
//' A simple function to test Rcpp setup.
//' @param x A numeric vector
//' @export
// [[Rcpp::export]]
NumericVector add_two(NumericVector x) {
  return x + 2;
}
