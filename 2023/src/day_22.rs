// Day 22 Sand Slabs
use crate::util::iter::*;
use crate::util::parse::*;

type Input = (usize, usize);

/// Parse input and compute results for Part 1 and Part 2 simultaneously.
pub fn parse(input: &str) -> Input {
    let mut bricks: Vec<_> = input.iter_unsigned::<usize>().chunk::<6>().collect();
    let mut heights = [0; 100];
    let mut indices = [usize::MAX; 100];

    let mut safe = vec![true; bricks.len()];
    let mut dominator: Vec<(usize, usize)> = Vec::with_capacity(bricks.len());

    bricks.sort_unstable_by_key(|b| b[2]); // Sort ascending by lowest z coordinate.

    for (i, &[x1, y1, z1, x2, y2, z2]) in bricks.iter().enumerate() {
        let start = 10 * y1 + x1;
        let end = 10 * y2 + x2;
        let step = if y2 > y1 { 10 } else { 1 };
        let height = z2 - z1 + 1;

        let mut top = 0;
        let mut previous = usize::MAX;
        let mut underneath = 0;
        let mut parent = 0;
        let mut depth = 0;

        // Find highest z-coordinate underneath brick
        for j in (start..=end).step_by(step) {
            top = top.max(heights[j]);
        }

        for j in (start..=end).step_by(step) {
            if heights[j] == top {
                let index = indices[j];
                if index != previous {
                    previous = index;
                    underneath += 1;

                    if underneath == 1 {
                        (parent, depth) = dominator[previous];
                    } else {
                        // Find common ancestor
                        let (mut a, mut b) = (parent, depth);
                        let (mut x, mut y) = dominator[previous];

                        while b > y {
                            (a, b) = dominator[a];
                        }
                        while y > b {
                            (x, y) = dominator[x];
                        }
                        while a != x {
                            (a, b) = dominator[a];
                            (x, _) = dominator[x];
                        }

                        (parent, depth) = (a, b);
                    }
                }
            }

            heights[j] = top + height; // Update height of grid cell.
            indices[j] = i; // Update index of grid cell.
        }

        if underneath == 1 {
            safe[previous] = false; // Mark as unsafe.
            parent = previous;
            depth = dominator[previous].1 + 1;
        }

        dominator.push((parent, depth));
    }

    let part_one = safe.iter().filter(|&&b| b).count();
    let part_two = dominator.iter().map(|(_, d)| d).sum();
    (part_one, part_two)
}

/// Return result for Part 1
pub fn part1(input: &Input) -> usize {
    input.0
}

/// Return result for Part 2
pub fn part2(input: &Input) -> usize {
    input.1
}

/// Run solution
pub fn run() {
    // Load input from file.
    let input_file = "data/input22.txt";
    let input = std::fs::read_to_string(input_file).expect("Failed to read input file");

    // Parse input and compute results.
    let parsed_input = parse(&input);

    // Results
    println!("Part 1: {}", part1(&parsed_input));
    println!("Part 2: {}", part2(&parsed_input));
}

