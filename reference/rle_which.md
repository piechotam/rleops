# Find indices of TRUE (1.0) values in an rle object

Find indices of TRUE (1.0) values in an rle object

## Usage

``` r
rle_which(rle)
```

## Arguments

- rle:

  An R rle list object (where 1.0 represents TRUE)

## Value

An integer vector of 1-based indices

## Examples

``` r
rle_in <- list(lengths = c(2, 1, 2), values = c(0, 1, 1))
res1 <- rle_which(rle_in)
stopifnot(all.equal(res1, c(3, 4, 5)))

rle_false <- list(lengths = c(5), values = c(0))
res2 <- rle_which(rle_false)
stopifnot(all.equal(res2, integer(0)))

rle_na <- list(lengths = c(2, 1), values = c(NA_real_, 1))
res3 <- rle_which(rle_na)
stopifnot(all.equal(res3, 3))
```
