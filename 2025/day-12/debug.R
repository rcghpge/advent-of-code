# Advent of Code 2025 Day 12 - debugging in R
# run: Rscript debug.R input.txt

#!/usr/bin/env Rscript
args <- commandArgs(trailingOnly = TRUE)
if (length(args) != 1) stop("Usage: Rscript aoc_day12_fixed.R input.txt")
file_path <- args[1]

lines <- readLines(file_path, warn = FALSE)
lines <- lines[nzchar(lines)]  

# Parse shapes (0: to 5:)
shapes <- list()
i <- 1
while (i <= length(lines) && grepl("^\\d+:", lines[i])) {
  shape_id <- as.integer(gsub(":(.*)", "", lines[i]))
  shape_lines <- c()
  i <- i + 1
  while (i <= length(lines) && nchar(lines[i]) > 0 && 
         !grepl("^\\d+:", lines[i]) && !grepl("x", lines[i])) {
    shape_lines <- c(shape_lines, lines[i])
    i <- i + 1
  }
  
  if (length(shape_lines) == 0) next
  
  # Find all # positions with proper row/col indexing
  all_hash_pos <- c()
  for (row in 1:length(shape_lines)) {
    line_chars <- strsplit(shape_lines[row], "")[[1]]
    hash_cols <- which(line_chars == "#")
    if (length(hash_cols) > 0) {
      all_hash_pos <- rbind(all_hash_pos, cbind(row, hash_cols))
    }
  }
  
  if (nrow(all_hash_pos) > 0) {
    shapes[[as.character(shape_id)]] <- all_hash_pos
  }
}

# Parse regions (WxH: counts...)
regions <- list()
while (i <= length(lines)) {
  line <- lines[i]
  if (grepl("x", line)) {
    # Extract dimensions and counts
    dim_match <- regmatches(line, regexpr("(\\d+)x(\\d+)", line))[[1]]
    w <- as.integer(gsub("x.*", "", dim_match))
    h <- as.integer(gsub(".*x", "", dim_match))
    
    # Extract counts after dimensions
    count_str <- gsub(".*x\\d+\\s*:?", "", line)
    counts <- as.integer(strsplit(trimws(count_str), "\\s+")[[1]])
    names(counts) <- 0:(length(counts)-1)
    
    regions[[length(regions)+1]] <- list(w=w, h=h, counts=counts)
  }
  i <- i + 1
}

cat("Parsed", length(shapes), "shapes:\n")
for (id in names(shapes)) {
  cat(sprintf("  Shape %s: %d blocks\n", id, nrow(shapes[[id]])))
}

cat("and", length(regions), "regions\n")

# Calculate total area needed for each region
valid_regions <- 0
for (r in 1:length(regions)) {
  region <- regions[[r]]
  total_needed <- 0
  for (id in names(region$counts)) {
    if (region$counts[id] > 0 && id %in% names(shapes)) {
      total_needed <- total_needed + region$counts[id] * nrow(shapes[[id]])
    }
  }
  
  region_area <- region$w * region$h
  can_fit <- total_needed <= region_area
  
  status <- ifelse(can_fit, "✓", "✗")
  cat(sprintf("%s %dx%d (area:%d, needed:%d): ", status, region$w, region$h, 
              region_area, total_needed))
  cat(paste(region$counts[1:min(6,length(region$counts))], collapse=" "), "\n")
  
  if (can_fit) valid_regions <- valid_regions + 1
}

cat("\n**Answer:**", valid_regions, "regions can fit all presents\n")[file:45]
cat("\n**Debugging information:\n")
