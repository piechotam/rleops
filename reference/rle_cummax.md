# Cumulative Max of an rle object

Cumulative Max of an rle object

## Usage

``` r
rle_cummax(rle)
```

## Arguments

- rle:

  An R rle list object

## Value

An R rle list object

## Examples

``` r
rle_in <- list(lengths = c(2, 2, 2), values = c(5, 3, 7))
res1 <- rle_cummax(rle_in)
stopifnot(all.equal(res1, list(lengths = c(4, 2), values = c(5, 7))))

rle_na1 <- list(lengths = c(2, 2), values = c(NA_real_, 5))
res2 <- rle_cummax(rle_na1)
stopifnot(all.equal(res2, list(lengths = 4L, values = NA_real_)))

rle_na2 <- list(lengths = c(2, 2, 2), values = c(5, NA_real_, 10))
res3 <- rle_cummax(rle_na2)
stopifnot(all.equal(res3, list(lengths = c(2L, 4L), values = c(5, NA_real_))))
```
