use std::fs;

/// Represents the parsed input structure.
pub struct Input {
    seeds: Vec<u64>,
    stages: Vec<Vec<[u64; 3]>>,
}

/// Parse the input string into the `Input` struct.
pub fn parse(input: &str) -> Input {
    let sections: Vec<_> = input
        .split("\n\n")
        .filter(|chunk| !chunk.trim().is_empty())
        .collect();

    if sections.len() < 2 {
        panic!("Input must contain seeds and at least one stage.");
    }

    // Parse seeds (ignore non-numeric labels)
    let seeds_section = sections[0].trim();
    let seeds = seeds_section
        .split_whitespace()
        .filter_map(|s| s.parse::<u64>().ok()) // Only keep valid numbers
        .collect();

    // Parse stages
    let stages = sections[1..]
        .iter()
        .map(|chunk| {
            let chunk = chunk.trim();

            chunk
                .split_whitespace()
                .filter_map(|s| s.parse::<u64>().ok()) // Only keep valid numbers
                .collect::<Vec<u64>>()
                .chunks(3)
                .map(|chunk| {
                    if chunk.len() == 3 {
                        [chunk[0], chunk[1], chunk[1] + chunk[2]] // Convert length to end
                    } else {
                        panic!("Invalid stage chunk: {:?}", chunk); // Ensure valid triples
                    }
                })
                .collect()
        })
        .collect();

    Input { seeds, stages }
}

/// Process each seed individually (Part 1).
pub fn part1(input: &Input) -> u64 {
    let mut seeds = input.seeds.clone();

    for stage in &input.stages {
        for seed in &mut seeds {
            for &[dest, start, end] in stage {
                if start <= *seed && *seed < end {
                    *seed = *seed - start + dest;
                    break;
                }
            }
        }
    }

    // Return the smallest seed value after processing.
    *seeds.iter().min().unwrap()
}

/// Process ranges for better performance (Part 2).
pub fn part2(input: &Input) -> u64 {
    let mut current_ranges: Vec<[u64; 2]> = input
        .seeds
        .chunks(2)
        .map(|chunk| [chunk[0], chunk[0] + chunk[1]]) // Convert seeds to ranges
        .collect();

    let mut next_ranges = Vec::new();
    let mut next_stage = Vec::new();

    for stage in &input.stages {
        for &[dest, start, end] in stage {
            for [s1, e1] in current_ranges.drain(..) {
                // Calculate overlap range.
                let overlap_start = s1.max(start);
                let overlap_end = e1.min(end);

                if overlap_start >= overlap_end {
                    // No overlap; keep the original range.
                    next_ranges.push([s1, e1]);
                } else {
                    // Add the overlap to the destination range.
                    next_stage.push([overlap_start - start + dest, overlap_end - start + dest]);

                    // Handle ranges outside of the overlap.
                    if s1 < overlap_start {
                        next_ranges.push([s1, overlap_start]);
                    }
                    if overlap_end < e1 {
                        next_ranges.push([overlap_end, e1]);
                    }
                }
            }

            std::mem::swap(&mut current_ranges, &mut next_ranges);
        }

        // Merge the current stage ranges for the next stage.
        current_ranges.append(&mut next_stage);
    }

    // Return the smallest starting point from the resulting ranges.
    current_ranges.iter().map(|range| range[0]).min().unwrap()
}

/// Main function to run the solution.
pub fn run() {
    // Load the input file.
    let input_data = fs::read_to_string("data/input5.txt").expect("Failed to read input file");

    // Parse the input.
    let input = parse(&input_data);

    // Solve parts 1 and 2.
    let result1 = part1(&input);
    let result2 = part2(&input);

    // Print results.
    println!("Part 1: {}", result1);
    println!("Part 2: {}", result2);
}



