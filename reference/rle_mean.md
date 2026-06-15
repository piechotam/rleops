# Mean of all values in an rle object

Mean of all values in an rle object

## Usage

``` r
rle_mean(rle, na_rm = FALSE)
```

## Arguments

- rle:

  An R rle list object

- na_rm:

  If TRUE, NA values are ignored. Default FALSE.

## Value

A numeric scalar

## Examples

``` r
rle_a <- list(lengths = c(2, 2), values = c(0.0, 4.0))
stopifnot(rle_mean(rle_a) == 2.0)

rle_na <- list(lengths = c(3, 1), values = c(6.0, NA_real_))
stopifnot(is.na(rle_mean(rle_na)))
stopifnot(rle_mean(rle_na, na_rm = TRUE) == 6.0)
```
