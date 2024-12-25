// Day 9 Mirage Maintenance
use std::fs;
use crate::util::utils::parse_int_vecs;

fn extrapolate(readings: &[i64], is_future: bool) -> i64 {
    if readings.iter().all(|&r| r == 0) {
        return 0;
    }
    let differences: Vec<_> = readings[1..]
        .iter()
        .zip(readings[..(readings.len() - 1)].iter())
        .map(|(a, b)| a - b)
        .collect();
    if is_future {
        readings.last().unwrap() + extrapolate(&differences, is_future)
    } else {
        readings.first().unwrap() - extrapolate(&differences, is_future)
    }
}

pub fn run() {
    // Read the input file
    let input = fs::read_to_string("data/input9.txt")
        .expect("Failed to read the input file");
    
    // Parse the input data
    let data: Vec<Vec<i64>> = parse_int_vecs(&input, true);

    // Calculate results for Part 1 and Part 2
    let part_1: i64 = data
        .iter()
        .map(|readings| extrapolate(readings, true)) // is_future = true
        .sum();
    
    let part_2: i64 = data
        .iter()
        .map(|readings| extrapolate(readings, false)) // is_future = false
        .sum();

    // Print results for Part 1 and Part 2
    println!("Part 1: {}", part_1);
    println!("Part 2: {}", part_2);
}

