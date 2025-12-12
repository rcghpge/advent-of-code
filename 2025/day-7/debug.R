#!/usr/bin/env Rscript

cat("AoC 2025 Day 7 - Laboratories\n\n")

input_file <- "input.txt"
input <- readLines(input_file)
len <- nchar(input[[1]])
input <- input |>
  strsplit(split = "") |>
  unlist() |>
  matrix(ncol = len, byrow = TRUE)

start <- which(input == "S")
input[start] <- "|"
dims <- dim(input)
visited <- rep(0, length(input))

next_line <- function(x, y) {
  for (i in seq_along(x)) {
    if (identical(c(x[i], y[i]), c("|", "."))) {
      y[i] <- "|"
    } else if (identical(c(x[i], y[i]), c("|", "^"))) {
      y[c(i - 1, i + 1)] <- "|"
    }
  }
  y
}

# PART 1: Propagate beams row-by-row, count splitters hit
for (i in 1:(dims[1] - 1)) {
  input[i + 1, ] <- next_line(input[i, ], input[i + 1, ])
}

splitters <- which(input == "^")
part1 <- sum(input[splitters - 1] == "|")
cat(sprintf("Part 1: %d splitters hit\n", part1))

# PART 2: Build DAG of splitters + bottom row, count paths
nodes <- c(splitters, seq(nrow(input), length(input), by = nrow(input)))
node_inds <- arrayInd(nodes, dims)
node_inds <- node_inds[order(node_inds[, 1]), ]
num_nodes <- nrow(node_inds)

adj_mat <- matrix(0, nrow = num_nodes, ncol = num_nodes)

for (i in seq_along(node_inds[, 1])) {
  below_cond <- node_inds[, 1] > node_inds[i, 1]
  left_cond <- node_inds[, 2] == node_inds[i, 2] - 1
  right_cond <- node_inds[, 2] == node_inds[i, 2] + 1
  below_l <- which(below_cond & left_cond)
  below_r <- which(below_cond & right_cond)
  if (length(below_l) > 0) {
    adj_mat[i, below_l[1]] <- 1
  }
  if (length(below_r) > 0) {
    adj_mat[i, below_r[1]] <- 1
  }
}

num_paths <- rep(0, num_nodes)

path_counter <- function(node) {
  paths <- 0
  if (node > (length(nodes) - nrow(input) + 1)) {
    paths <- paths + 1
  } else if (num_paths[node] > 0) {
    paths <- paths + num_paths[node]
  } else {
    neighbors <- which(adj_mat[node, ] == 1)
    for (i in neighbors) {
      paths <- paths + path_counter(i)
    }
  }
  num_paths[node] <<- paths
  paths
}

part2 <- path_counter(1)
cat(sprintf("Part 2: %.0f paths\n", part2))

cat("\n✅ Both parts complete!\n")
