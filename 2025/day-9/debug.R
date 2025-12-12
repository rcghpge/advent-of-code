#!/usr/bin/env Rscript

suppressPackageStartupMessages({
  library(stringr)
  library(dplyr)
  library(purrr)
  library(collections)
})

# -------------------------------------------------------------------
# Load Data (uses input.txt in dir)
# -------------------------------------------------------------------
s <- Sys.time()

data_name <- "input"
in_data <- readLines(paste0(data_name, ".txt"), warn = FALSE)

split_nums <- do.call(rbind, strsplit(in_data, ","))

total_points <- length(in_data) |> as.numeric()

delta_mat_create <- function(vec) {
  mat <- replicate(length(vec), vec)
  t_mat <- t(mat)
  abs(mat - t_mat) + 1
}

xdelta <- delta_mat_create(as.numeric(split_nums[, 1]))
ydelta <- delta_mat_create(as.numeric(split_nums[, 2]))

area_mat <- xdelta * ydelta
area_vec <- area_mat[upper.tri(area_mat)]

cat(paste0("Part 1 Answer: ", max(area_vec), "\n"))
cat("Part 1 runtime: ", Sys.time() - s, " secs\n")

# -------------------------------------------------------------------
# Part 2
# -------------------------------------------------------------------
s <- Sys.time()

point_data_create <- function(point_char_df) {
  point_df <- tibble(
    x1 = as.numeric(point_char_df[, 1]),
    y1 = as.numeric(point_char_df[, 2])
  )
  
  point_df_shift <- tibble(
    x2 = c(dplyr::pull(point_df, x1)[-1], dplyr::pull(point_df, x1)[1]),
    y2 = c(dplyr::pull(point_df, y1)[-1], dplyr::pull(point_df, y1)[1])
  )
  
  point_df[, c("x2", "y2")] <- point_df_shift
  point_df
}

points <- point_data_create(split_nums)

# -------------------------------------------------------------------
# Create blank grid
# -------------------------------------------------------------------
x_cols <- sort(unique(points$x1))
y_rows <- sort(unique(points$y1))

x_dict <- dict()
y_dict <- dict()

i <- 1
x_cols_char <- character(length(x_cols))

for (x in x_cols) {
  name <- paste0("C", (i * 2) - 1)
  
  x_dict$set(name, x)
  x_cols_char[i] <- name
  
  points[which(points$x1 == x), "x1_norm"] <- (i * 2) - 1
  points[which(points$x2 == x), "x2_norm"] <- (i * 2) - 1
  
  i <- i + 1
}

i <- 1
y_rows_char <- character(length(y_rows))

for (y in y_rows) {
  name <- paste0("R", (i * 2) - 1)
  
  y_dict$set(name, y)
  y_rows_char[i] <- name
  
  points[which(points$y1 == y), "y1_norm"] <- (i * 2) - 1
  points[which(points$y2 == y), "y2_norm"] <- (i * 2) - 1
  
  i <- i + 1
}

max_x <- length(x_cols) * 2 - 1
max_y <- length(y_rows) * 2 - 1

points_norm <- points |>
  dplyr::select(x1_norm, y1_norm, x2_norm, y2_norm)

# -------------------------------------------------------------------
# Map a grid
# -------------------------------------------------------------------
point_mat <- matrix(0, nrow = max_y, ncol = max_x)

point_fill <- function(x1_norm, x2_norm, y1_norm, y2_norm) {
  if (x1_norm == x2_norm) {
    min_y <- min(y1_norm, y2_norm)
    max_y <- max(y1_norm, y2_norm)
    point_mat[min_y:max_y, x1_norm] <<- 1
  } else if (y1_norm == y2_norm) {
    min_x <- min(x1_norm, x2_norm)
    max_x <- max(x1_norm, x2_norm)
    point_mat[y1_norm, min_x:max_x] <<- 1
  }
}

purrr::pwalk(points_norm, point_fill)

# Fill outside from left
for (row in 1:nrow(point_mat)) {
  for (col in 1:ncol(point_mat)) {
    if (point_mat[row, col] == 1) break
    point_mat[row, col] <- -1
  }
}

# Fill outside from right
for (row in 1:nrow(point_mat)) {
  for (col in ncol(point_mat):1) {
    if (point_mat[row, col] == 1) break
    point_mat[row, col] <- -1
  }
}

point_mat[which(point_mat == 0)] <- 1

# -------------------------------------------------------------------
# Priority queue 
# -------------------------------------------------------------------
q <- priority_queue()

x_key_get <- function(val) {
  key <- unlist(x_dict$keys()[which(sapply(x_dict$values(), function(x) val == x))])
  as.numeric(substr(key, 2, nchar(key)))
}

y_key_get <- function(val) {
  key <- unlist(y_dict$keys()[which(sapply(y_dict$values(), function(x) val == x))])
  as.numeric(substr(key, 2, nchar(key)))
}

row_i <- 1
col_i <- 2

for (i in 1:length(area_vec)) {
  point1 <- as.numeric(split_nums[row_i, ])
  point2 <- as.numeric(split_nums[col_i, ])
  
  q$push(
    list(
      point1 = point1,
      point2 = point2,
      area   = area_vec[i]
    ),
    priority = area_vec[i]
  )
  
  row_i <- row_i + 1
  
  if (row_i == col_i) {
    col_i <- col_i + 1
    row_i <- 1
  }
}

rect_check <- function(cur_max) {
  x1 <- x_key_get(cur_max$point1[1])
  y1 <- y_key_get(cur_max$point1[2])
  
  x2 <- x_key_get(cur_max$point2[1])
  y2 <- y_key_get(cur_max$point2[2])
  
  min_x <- min(x1, x2)
  min_y <- min(y1, y2)
  max_x <- max(x1, x2)
  max_y <- max(y1, y2)
  
  rect_mat <- point_mat[min_y:max_y, min_x:max_x]
  
  !(-1 %in% rect_mat)
}

in_polygon <- FALSE
i <- 1

while (!in_polygon) {
  cat(paste0("Iteration: ", i, "\n"))
  next_val <- q$pop()
  
  in_polygon <- rect_check(next_val)
  
  if (in_polygon) {
    cat(paste0("Part 2 Answer: ", next_val$area, "\n"))
  } else {
    i <- i + 1
  }
}

cat("Part 2 runtime: ", Sys.time() - s, " secs\n")
