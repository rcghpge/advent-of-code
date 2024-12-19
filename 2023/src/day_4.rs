/// Day 4: Scratchcards
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

/// Parses the input file into a vector of usize values.
pub fn parse_input_file(file_path: &str) -> io::Result<Vec<usize>> {
    let path = Path::new(file_path);
    let file = File::open(&path)?;
    let reader = io::BufReader::new(file);

    let mut results = Vec::new();

    for line in reader.lines() {
        let line = line?;
        let mut found = [false; 100]; // Numbers are at most 99

        if let Some((win, have)) = line.split_once('|') {
            // Parse winning numbers
            win.split_whitespace()
                .skip(1) // Assuming the first element is not a number
                .filter_map(|num_str| num_str.parse::<usize>().ok())
                .for_each(|num| found[num] = true);

            // Count valid "have" numbers
            let count = have
                .split_whitespace()
                .filter_map(|num_str| num_str.parse::<usize>().ok())
                .filter(|&num| found[num])
                .count();

            results.push(count);
        }
    }

    Ok(results)
}

/// Part 1 solution
pub fn part1(input: &[usize]) -> u32 {
    input.iter().map(|&n| (1 << n) >> 1).sum()
}

/// Part 2 solution
pub fn part2(input: &[usize]) -> u32 {
    let mut copies = vec![1; input.len()]; // Start with a single copy of each card

    for (i, &n) in input.iter().enumerate() {
        (1..=n).for_each(|j| {
            if i + j < copies.len() {
                copies[i + j] += copies[i];
            }
        });
    }

    copies.iter().sum()
}

/// Run solution
pub fn run() {
    let input_file = "data/input4.txt";
    match parse_input_file(input_file) {
        Ok(data) => {
            println!("Part 1: {}", part1(&data));
            println!("Part 2: {}", part2(&data));
        }
        Err(_) => eprintln!("Error: Failed to parse input data."),
    }
}



