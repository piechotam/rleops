# rleops

Operations on Run-Length-Encoded vectors, that preserve the
encoding of inputs and outputs. The library features basic binary operations,
aggregations, running statistics and other handful utils.

## Installation

### R

Install the development version from GitHub:

```r
install.packages("devtools")
devtools::install_github("piechotam/rleops")
```

### From source

```bash
git clone https://github.com/piechotam/rleops.git
cd rleops

R CMD INSTALL .
```

## Basic usage

```R
a <- c(rep(5, 10), rep(-5, 2), rep(3, 7))
b <- c(rep(2, 3),  rep(10, 9), rep(-3, 7))

rle_a <- rle(a)
rle_b <- rle(b)

rle_add(rle_a, rle_b)
#> $lengths
#> [1] 3 7 2 7
#> 
#> $values
#> [1]  7 15  5  0

rle_a$values <- abs(rle_a$values)
rle_a
#> Run Length Encoding
#>   lengths: int [1:3] 10 2 7
#>   values : num [1:3] 5 5 3   <-- duplicates

rle_compact(rle_a)
#> $lengths
#> [1] 12  7
#> 
#> $values
#> [1] 5 3

rle_cummax(rle_b)
#> $lengths
#> [1]  3 16
#> 
#> $values
#> [1]  2 10

rle_sum(rle_a)
#> [1] 81
```