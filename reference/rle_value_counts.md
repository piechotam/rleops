# Unique values and their frequencies of an rle object

Unique values and their frequencies of an rle object

## Usage

``` r
rle_value_counts(rle)
```

## Arguments

- rle:

  An R rle list object

## Value

A list containing unique values and their total counts

## Examples

``` r
rle_in <- list(lengths = c(2, 3, 4), values = c(10, 20, 10))
res1 <- rle_value_counts(rle_in)
# No guarantee on the order in output
stopifnot(all.equal(res1$counts[res1$values == 10], 6))
stopifnot(all.equal(res1$counts[res1$values == 20], 3))

empty <- list(lengths = integer(0), values = numeric(0))
res3 <- rle_value_counts(empty)
stopifnot(all.equal(length(res3$values), 0))
```
