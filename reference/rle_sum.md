# Sum of all values in an rle object

Sum of all values in an rle object

## Usage

``` r
rle_sum(rle, na_rm = FALSE)
```

## Arguments

- rle:

  An R rle list object

- na_rm:

  If TRUE, NA values are ignored.

## Value

Sum of values, or NA if any value is NA and na_rm is FALSE

## Examples

``` r
rle_a <- list(lengths = c(2, 3), values = c(4.0, 2.0))
stopifnot(rle_sum(rle_a) == 14.0)

rle_na <- list(lengths = c(2, 1), values = c(3.0, NA_real_))
stopifnot(is.na(rle_sum(rle_na)))
stopifnot(rle_sum(rle_na, na_rm = TRUE) == 6.0)
```
