# Advent of Code 2023 - Rust

This repository contains solutions to the **Advent of Code 2023** challenges, implemented in **Rust**. Each day's solution is modularized for clarity, scalability, and ease of navigation.

---

## Project Structure

```plaintext
~/advent-of-code/2023/
├── Cargo.toml      # Project configuration
├── Cargo.lock      # Dependency lock file
├── data/           # Input data for challenges
├── src/
│   ├── main.rs     # Main orchestrator for running solutions
│   ├── day_1.rs    # Solution for Day 1
│   └── day_2.rs    # Solution for Day 2 (and more to follow)
└── target/         # Build artifacts (ignored by Git)
```

### Key Files
- **`Cargo.toml`**: Contains project metadata and dependencies.
- **`src/main.rs`**: Central entry point that orchestrates the execution of day-specific solutions.
- **`src/day_n.rs`**: Individual solutions for each day's challenge.
- **`data/`**: Stores input data for challenges.

---

## Getting Started

### Prerequisites
- Install [Rust](https://www.rust-lang.org/). Ensure you have the latest stable version installed.

### Installation

Clone the repository:

```bash
git clone <repository-url>
cd advent-of-code/2023
```

Build the project:

```bash
cargo build
```

Run the solutions:

```bash
cargo run
```

---

## Running Specific Days

You can run solutions for specific days by modifying `main.rs` or using a CLI (if implemented). Example CLI usage:

```bash
cargo run -- 1  # Runs the Day 1 solution
cargo run -- 2  # Runs the Day 2 solution
```

---

## Adding New Solutions

1. Create a new file for the day in the `src/` directory:
   
   ```bash
   touch src/day_3.rs
   ```

2. Define the solution in the new file. Example:

   ```rust
   pub fn run() {
       println!("Solution for Day 3");
   }
   ```

3. Update `main.rs` to include the new module and call its `run()` function:

   ```rust
   mod day_3;

   fn main() {
       println!("Advent of Code 2023");

       println!("\n--- Day 1 ---");
       day_1::run();

       println!("\n--- Day 2 ---");
       day_2::run();

       println!("\n--- Day 3 ---");
       day_3::run();
   }
   ```

4. Build and test:

   ```bash
   cargo build
   cargo run
   ```

---

## Dependencies

This project currently has no external dependencies. To add dependencies, update the `Cargo.toml` file. Example:

```toml
[dependencies]
regex = "1.7.0"
```

Install and build the updated project:

```bash
cargo build
```

---

## Contributing

Contributions, suggestions, and feedback are welcome! See [aoc-proofing-repo](https://github.com/rcghpge/aoc-proofing-repo). This is the baseline repo for a robust AoC learning resource for anyone looking to 100x their programming, coding, and problem-solving skills.

## Acknowledgments

- The Rust community.
- [Advent of Code](https://adventofcode.com/) by Eric Wastl.
- [The Rust Programming Language @ Discord](https://discord.gg/Aa8N6py6sQ)
- [Rust Programming Language Community Discord](https://discord.gg/rust-lang-community).

---

**Happy coding!** 🎄
