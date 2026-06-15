# Total length of the rle object (number of elements before encoding)

Total length of the rle object (number of elements before encoding)

## Usage

``` r
rle_length(rle)
```

## Arguments

- rle:

  An R rle list object

## Value

Total length

## Examples

``` r
rle_a <- list(lengths = c(2, 3, 1), values = c(1, 2, 3))
stopifnot(rle_length(rle_a) == 6)

empty <- list(lengths = integer(0), values = numeric(0))
stopifnot(rle_length(empty) == 0)
```
