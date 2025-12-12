#!/usr/bin/env Rscript

# AOC 2025 Day 10 Part 2 - Auto-installs lpSolve
cat("=== AOC 2025 Day 10 Part 2 ===\n")

# Auto-install lpSolve if missing
if (!require(lpSolve, quietly = TRUE)) {
  cat("Installing lpSolve...\n")
  install.packages("lpSolve", repos = "https://cran.rstudio.com/")
  library(lpSolve)
} else {
  library(lpSolve)
}

lines <- readLines("input.txt")
total_presses <- 0
machine_num <- 0

solve_machine <- function(line, machine_num) {
  target_match <- regmatches(line, regexpr("\\{[^}]+\\}", line))[[1]]
  if (length(target_match) == 0) return(999)
  
  targets <- as.integer(unlist(strsplit(gsub("[{}]", "", target_match), ",")))
  n_counters <- length(targets)
  
  button_strs <- regmatches(line, gregexpr("\\([^)]+\\)", line))[[1]]
  n_buttons <- length(button_strs)
  
  if (n_buttons == 0 || n_counters == 0) return(999)
  
  buttons <- matrix(0, nrow = n_counters, ncol = n_buttons)
  for (b in 1:n_buttons) {
    btn_list <- gsub("[() ]", "", button_strs[b])  
    if (nchar(btn_list) == 0) next
    
    btn_indices <- as.integer(unlist(strsplit(btn_list, ",")))
    for (idx in btn_indices) {
      if (!is.na(idx) && idx >= 0 && idx < n_counters) {
        buttons[idx + 1, b] <- 1
      }
    }
  }
  
  obj <- rep(1, n_buttons)
  const.mat <- buttons
  const.dir <- rep("==", n_counters)
  const.rhs <- targets
  result <- lp(direction = "min",
               objective.in = obj,
               const.mat = const.mat,
               const.dir = const.dir,
               const.rhs = const.rhs,
               all.int = TRUE,
               transpose.constraints = TRUE)
  
  if (result$status == 0) {
    presses <- round(result$objval)
    if (presses == Inf) presses <- 999
    return(presses)
  } else {
    return(999)
  }
}

cat("Processing", length(lines), "machines...\n")
for (i in seq_along(lines)) {
  line <- lines[i]
  if (nchar(trimws(line)) == 0) next
  
  machine_num <- machine_num + 1
  presses <- solve_machine(line, machine_num)
  total_presses <- total_presses + presses
  
  if (machine_num %% 20 == 0 || machine_num <= 5) {
    cat(sprintf("Machine %3d: %5d presses (Running total: %d)\n", 
                machine_num, presses, total_presses))
  }
}

cat(sprintf("\n🎉 Part 2 Total Count: %d 🎉\n", total_presses))
cat("\n=== Example Verification ===\n")
example <- c(
  "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}",
  "[...#.] (0,2,3
