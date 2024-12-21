// Day 6: Wait For It

use std::fs;

/// Parse the input from a string into a vector of lines.
pub fn parse(input: &str) -> Vec<&str> {
    input.lines().collect()
}

/// Solve Part 1: Use the race function on the raw input.
pub fn part1(input: &[&str]) -> u128 {
    race(input[0], input[1])
}

/// Solve Part 2: Use the race function on merged (numeric-only) input.
pub fn part2(input: &[&str]) -> u128 {
    race(&merge(input[0]), &merge(input[1]))
}

/// Filter numeric characters from a line and merge them into a single string.
fn merge(line: &str) -> String {
    line.chars().filter(char::is_ascii_digit).collect()
}

/// Perform the race calculation based on the quadratic formula.
fn race(first: &str, second: &str) -> u128 {
    // Parse the times and distances as unsigned integers.
    let times = first
        .split_whitespace()
        .filter_map(|s| s.parse::<u128>().ok()); // Skip invalid entries.
    let distances = second
        .split_whitespace()
        .filter_map(|s| s.parse::<u128>().ok()); // Skip invalid entries.

    let mut result = 1;

    for (time, distance) in times.zip(distances) {
        // Use the quadratic formula to calculate roots.
        if time * time < 4 * distance {
            panic!(
                "Invalid input: time² must be greater than or equal to 4 * distance. Found time={} and distance={}",
                time, distance
            );
        }

        let root = ((time * time - 4 * distance) as f64).sqrt() as u128;

        let mut start = (time - root + 1) / 2; // Using integer math with div_ceil.
        let mut end = (time + root) / 2;

        // Adjust the bounds as needed.
        if start * (time - start) > distance {
            start -= 1;
        }
        if end * (time - end) > distance {
            end += 1;
        }

        // Multiply the range to the result.
        result *= end - start - 1;
    }

    result
}

/// Main function to read the input and solve both parts.
pub fn run() {
    // Read the input from `data/input6.txt`.
    let input_data = fs::read_to_string("data/input6.txt").expect("Failed to read input file");

    // Parse the input into lines.
    let input = parse(&input_data);

    // Validate the input format.
    if input.len() < 2 {
        panic!("Input must contain at least two lines: times and distances.");
    }

    // Solve parts 1 and 2.
    let result1 = part1(&input);
    let result2 = part2(&input);

    // Print the results.
    println!("Part 1: {}", result1);
    println!("Part 2: {}", result2);
}

