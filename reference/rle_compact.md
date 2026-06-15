# Compact an rle object by merging identical runs

Compact an rle object by merging identical runs

## Usage

``` r
rle_compact(rle)
```

## Arguments

- rle:

  An R rle list object

## Value

An R rle list object with identical runs merged

## Examples

``` r
uncompacted <- list(lengths = c(2, 3, 1), values = c(5, 5, 10))
res1 <- rle_compact(uncompacted)
stopifnot(all.equal(res1, list(lengths = c(5, 1), values = c(5, 10))))

compacted <- list(lengths = c(5, 2), values = c(1, 2))
stopifnot(all.equal(rle_compact(compacted), compacted))

nas <- list(lengths = c(1, 2), values = c(NA_real_, NA_real_))
res2 <- rle_compact(nas)
stopifnot(all.equal(res2, list(lengths = 3, values = NA_real_)))
```
