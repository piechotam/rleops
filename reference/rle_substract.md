# Substract two rle objects, element-wise

Substract two rle objects, element-wise

## Usage

``` r
rle_substract(rle_a, rle_b, recycle = FALSE)
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

An R rle list object containing the difference

## Examples

``` r
rle_a <- list(lengths = c(2, 3), values = c(10, 20))
rle_b <- list(lengths = c(3, 2), values = c(5, 15))
result <- rle_substract(rle_a, rle_b)

stopifnot(all.equal(result$lengths, c(2, 1, 2)))
stopifnot(all.equal(result$values, c(5, 15, 5)))
```
