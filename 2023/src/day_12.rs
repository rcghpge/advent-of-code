// Day 12 Hot Springs
use std::fs;
use crate::util::utils::parse_ints;

fn min_size(group_lengths: &[usize]) -> usize {
    group_lengths.iter().map(|len| len + 1).sum()
}

fn count_arrangements(spring_states: &[u8], group_lengths: &[usize]) -> u64 {
    if group_lengths.is_empty() {
        return if spring_states.contains(&b'#') { 0 } else { 1 };
    }

    let center_group_index = group_lengths.len() / 2;
    let center_group_length = group_lengths[center_group_index];
    let left_group_lengths = &group_lengths[..center_group_index];
    let right_group_lengths = &group_lengths[center_group_index + 1..];

    let left_min_size = min_size(left_group_lengths);
    let right_min_size = min_size(right_group_lengths);

    let mut num_arrangements = 0;
    let spring_states_len = spring_states.len();

    for start_index in left_min_size..=spring_states_len - right_min_size - center_group_length {
        if (start_index == 0 || b".?".contains(&spring_states[start_index - 1]))
            && !spring_states[start_index..start_index + center_group_length].contains(&b'.')
            && (start_index + center_group_length == spring_states_len
                || b".?".contains(&spring_states[start_index + center_group_length]))
        {
            let left_states = &spring_states[..start_index.max(1) - 1];
            let left_arrangements = count_arrangements(left_states, left_group_lengths);

            if left_arrangements > 0 {
                let right_start_index = (start_index + center_group_length + 1).min(spring_states_len);
                let right_states = &spring_states[right_start_index..];
                let right_arrangements = count_arrangements(right_states, right_group_lengths);

                num_arrangements += left_arrangements * right_arrangements;
            }
        }
    }

    num_arrangements
}

pub fn run() {
    // Read the input file
    let input_file = "data/input12.txt";
    let input = fs::read_to_string(input_file).expect("Failed to read input file");

    // Parse the input data
    let condition_records: Vec<_> = input
        .lines()
        .map(|line| {
            let mut parts = line.split_ascii_whitespace();
            let spring_states: &[u8] = parts.next().unwrap().as_bytes();
            let group_lengths: Vec<usize> = parse_ints(parts.next().unwrap(), false);
            (spring_states, group_lengths)
        })
        .collect();

    // Part 1: Number of arrangements for the original data
    let part_1_arrangements: u64 = condition_records
        .iter()
        .map(|(spring_states, group_lengths)| count_arrangements(spring_states, group_lengths))
        .sum();
    println!("Part 1: {}", part_1_arrangements);

    // Part 2: Number of arrangements for unfolded data
    let part_2_arrangements: u64 = condition_records
        .iter()
        .map(|(spring_states, group_lengths)| {
            let unfolded_spring_states = [std::str::from_utf8(spring_states).unwrap(); 5].join("?");
            let unfolded_group_lengths = group_lengths.repeat(5);
            count_arrangements(unfolded_spring_states.as_bytes(), &unfolded_group_lengths)
        })
        .sum();
    println!("Part 2: {}", part_2_arrangements);
}

