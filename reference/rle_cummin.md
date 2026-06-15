# Cumulative Min of an rle object

Cumulative Min of an rle object

## Usage

``` r
rle_cummin(rle)
```

## Arguments

- rle:

  An R rle list object

## Value

An R rle list object

## Examples

``` r
rle_in <- list(lengths = c(2, 2, 2), values = c(5, 8, 2))
res1 <- rle_cummin(rle_in)
stopifnot(all.equal(res1, list(lengths = c(4, 2), values = c(5, 2))))

rle_na1 <- list(lengths = c(1, 2), values = c(NA_real_, -5))
res2 <- rle_cummin(rle_na1)
stopifnot(all.equal(res2, list(lengths = 3L, values = NA_real_)))
```
