# Element-wise equality of two rle objects

Element-wise equality of two rle objects

## Usage

``` r
rle_eq(rle_a, rle_b, recycle = FALSE)
```

## Arguments

- rle_a:

  An R rle list object

- rle_b:

  An R rle list object

- recycle:

  If TRUE, shorter rle will be recycled. If FALSE, rle lengths must
  match.

## Value

An R rle list object containing 1.0 (TRUE), 0.0 (FALSE) or NaN

## Examples

``` r
a <- list(lengths = c(2, 2), values = c(10, 20))
b <- list(lengths = c(2, 2), values = c(10, 99))
res1 <- rle_eq(a, b)
stopifnot(all.equal(res1, list(lengths = c(2, 2), values = c(1.0, 0.0))))

c <- list(lengths = c(2), values = c(10))
d <- list(lengths = c(1, 1), values = c(NA_real_, 10))
res2 <- rle_eq(c, d)
stopifnot(all.equal(res2, list(lengths = c(1, 1), values = c(NA_real_, 1.0))))
```
