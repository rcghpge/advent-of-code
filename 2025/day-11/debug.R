#!/usr/bin/env Rscript

# Day 11: Reactor - Part 2 

parse_input <- function(filename) {
  lines <- readLines(filename)
  graph <- list()
  
  for (line in lines) {
    if (grepl(":", line, fixed = TRUE)) {
      parts <- strsplit(line, ":", fixed = TRUE)[[1]]
      node <- trimws(parts[1])
      outputs <- trimws(strsplit(parts[2], " ")[[1]])
      outputs <- outputs[outputs != ""]
      graph[[node]] <- outputs
    }
  }
  
  if (is.null(graph[["out"]])) {
    graph[["out"]] <- character(0)
  }
  
  return(graph)
}

count_paths_dp <- function(graph, start, end, must_visit = character()) {
  
  memo <- new.env()
  must_visit_set <- sort(must_visit)
  
  encode_set <- function(char_vec) {
    if (length(char_vec) == 0) return("")
    paste(sort(char_vec), collapse="|")
  }
  
  dfs_memo <- function(current, remaining) {
    # Base case: reached end node
    if (current == end) {
      # Only count if all required nodes visited
      return(if (length(remaining) == 0) 1 else 0)
    }
    
    remaining_key <- encode_set(remaining)
    memo_key <- paste(current, remaining_key, sep="||")
    
    if (exists(memo_key, envir = memo)) {
      return(get(memo_key, envir = memo))
    }
    
    neighbors <- graph[[current]]
    
    if (is.null(neighbors) || length(neighbors) == 0) {
      # Dead end - no path to 'end'
      assign(memo_key, 0, envir = memo)
      return(0)
    }
    
    total <- 0
    for (neighbor in neighbors) {
      # Remove neighbor from remaining if it's there
      new_remaining <- remaining[remaining != neighbor]
      
      total <- total + dfs_memo(neighbor, new_remaining)
    }
    
    assign(memo_key, total, envir = memo)
    return(total)
  }
  
  return(dfs_memo(start, must_visit_set))
}

# ============================================================================
# Main
# ============================================================================

cat("Day 11: Reactor - Part 2\n")
cat("=========================\n\n")

cat("Parsing input.txt...\n")
graph <- parse_input("input.txt")

cat(sprintf("Loaded %d nodes\n", length(graph)))
cat(sprintf("  svr: %s\n", if("svr" %in% names(graph)) "YES" else "NO"))
cat(sprintf("  out: %s\n", if("out" %in% names(graph)) "YES" else "NO"))
cat(sprintf("  dac: %s\n", if("dac" %in% names(graph)) "YES" else "NO"))
cat(sprintf("  fft: %s\n", if("fft" %in% names(graph)) "YES" else "NO"))
cat("\n")

cat("Computing paths from 'svr' to 'out' visiting both 'dac' and 'fft'...\n")
cat("(Dynamic Programming on DAG)\n\n")

start_time <- Sys.time()
result <- count_paths_dp(graph, "svr", "out", c("dac", "fft"))
end_time <- Sys.time()

elapsed <- difftime(end_time, start_time, units = "secs")

cat("====================================\n")
cat(sprintf("Total Path Count: %.0f\n", result))
cat(sprintf("Time: %.4f seconds\n", elapsed))
cat("====================================\n")