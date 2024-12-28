// Day 15 Lens Library
use std::fs;

/// Represents an item in the lens library.
struct Item<'a> {
    label: &'a [u8],
    lens: usize,
}

/// Parses the input and calculates Part 1 and Part 2 simultaneously.
fn parse(input: &str) -> (usize, usize) {
    let mut part_one = 0;
    let mut part_two = 0;

    // Initialize the boxes vector
    let mut boxes: Vec<Vec<Item<'_>>> = (0..256).map(|_| Vec::new()).collect();

    for step in input.trim().as_bytes().split(|&b| b == b',') {
        let size = step.len();
        part_one += hash(step);

        if step[size - 1] == b'-' {
            // Handle deletion case
            let label = &step[..size - 1];
            let hash = hash(label);
            let slot = &mut boxes[hash];

            // Remove the item if it exists
            if let Some(i) = slot.iter().position(|item| item.label == label) {
                slot.remove(i);
            }
        } else {
            // Handle addition or update case
            let label = &step[..size - 2];
            let hash = hash(label);
            let slot = &mut boxes[hash];
            let lens = (step[size - 1] - b'0') as usize;

            // Replace or append the lens
            if let Some(i) = slot.iter_mut().find(|item| item.label == label) {
                i.lens = lens;
            } else {
                slot.push(Item { label, lens });
            }
        }
    }

    // Calculate Part 2: Total focusing power
    for (i, slot) in boxes.iter().enumerate() {
        for (j, item) in slot.iter().enumerate() {
            part_two += (i + 1) * (j + 1) * item.lens;
        }
    }

    (part_one, part_two)
}

/// Custom hash function.
#[inline]
fn hash(slice: &[u8]) -> usize {
    slice.iter().fold(0, |acc, &b| ((acc + b as usize) * 17) & 0xff)
}

/// Run the program with input from `data/input15.txt`.
pub fn run() {
    // Load input data from file
    let input_file = "data/input15.txt";
    let input = fs::read_to_string(input_file).expect("Failed to read input file");

    // Parse input and calculate results
    let (part_one, part_two) = parse(&input);

    // Print results
    println!("Part 1: {}", part_one);
    println!("Part 2: {}", part_two);
}

