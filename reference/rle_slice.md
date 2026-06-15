# Extract a slice of an rle vector

Extract a slice of an rle vector

## Usage

``` r
rle_slice(rle, start, end)
```

## Arguments

- rle:

  An R rle list object

- start:

  The 1-based starting index

- end:

  The 1-based ending index

## Value

A sliced R rle list object

## Examples

``` r
rle_in <- list(lengths = c(3, 3, 3), values = c(10, 20, 30))

res1 <- rle_slice(rle_in, 2, 7)
stopifnot((all.equal(res1, list(lengths = c(2, 3, 1), values = c(10, 20, 30)))))

res2 <- rle_slice(rle_in, 1, 9)
stopifnot(all.equal(res2, rle_in))

res3 <- rle_slice(rle_in, 5, 5)
stopifnot(all.equal(res3, list(lengths = 1, values = 20)))
res4 <- rle_slice(rle_in, 8, 20)
stopifnot(all.equal(res4, list(lengths = 2, values = 30)))
```
