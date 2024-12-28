// Day 20 Pulse Propagation
use crate::util::hash::*;
use std::fs;

type Input = [u32; 4];

/// Parse input into structured format.
pub fn parse(input: &str) -> Input {
    let mut node = FastMap::with_capacity(100);
    let mut kind = FastMap::with_capacity(100);

    for line in input.lines() {
        let mut tokens = line.split(|c: char| !c.is_ascii_lowercase()).filter(|s| !s.is_empty());

        let key = tokens.next().unwrap();
        let children: Vec<_> = tokens.collect();

        node.insert(key, children);
        kind.insert(key, !line.starts_with('&'));
    }

    let mut todo = Vec::new();
    let mut numbers = Vec::new();

    for &start in &node["broadcaster"] {
        todo.push((start, 0, 1));
    }

    while let Some((key, mut value, bit)) = todo.pop() {
        let children = &node[key];

        if let Some(next) = children.iter().find(|&&k| kind[k]) {
            if children.len() == 2 {
                value |= bit;
            }
            todo.push((next, value, bit << 1));
        } else {
            numbers.push(value | bit);
        }
    }

    numbers.try_into().unwrap()
}

/// Compute Part 1 using bitwise logic to count pulses.
pub fn part1(input: &Input) -> u32 {
    assert!(input.iter().all(|&n| n > 1000));

    let pairs: Vec<_> = input.iter().map(|n| (n, 13 - n.count_ones())).collect();

    let mut low = 5000;
    let mut high = 0;

    for n in 0..1000 {
        let rising: u32 = !n & (n + 1);
        high += 4 * rising.count_ones();

        let falling: u32 = n & !(n + 1);
        low += 4 * falling.count_ones();

        for &(number, feedback) in &pairs {
            let factor = (rising & number).count_ones();
            high += factor * (feedback + 3);
            low += factor;

            let factor = (falling & number).count_ones();
            high += factor * (feedback + 2);
            low += 2 * factor;
        }
    }

    low * high
}

/// Compute Part 2 assuming all numbers are prime or co-prime.
pub fn part2(input: &Input) -> u64 {
    input.iter().map(|&n| n as u64).product()
}

/// Main entry point for script
pub fn run() {
    // Load input data
    let input_file = "data/input20.txt";
    let input = fs::read_to_string(input_file).expect("Failed to read input file");

    // Parse input
    let parsed_input = parse(&input);

    // Compute results 
    let part1_result = part1(&parsed_input);
    let part2_result = part2(&parsed_input);

    // Results
    println!("Part 1: {}", part1_result);
    println!("Part 2: {}", part2_result);
}

