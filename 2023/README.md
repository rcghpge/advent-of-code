# Advent of Code 2023 - Rust
**WIP**

This repository contains solutions to the **Advent of Code 2023** challenges, implemented in **Rust**. Each day's solution is modularized for clarity and ease of navigation.

## Project Structure

```
~/advent-of-code/2023/
├── Cargo.toml      # Project configuration
├── Cargo.lock      # Dependency lock file
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

---

## Getting Started

### Prerequisites
- [Rust](https://www.rust-lang.org/) (Ensure you have the latest stable version installed.)

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
To run a specific day, modify the `main.rs` file or use the CLI if configured. For example, if a CLI is implemented:
```bash
cargo run -- 1  # Runs Day 1 solution
cargo run -- 2  # Runs Day 2 solution
```

---

## Adding New Solutions
1. Create a new file in the `src/` directory for the new day:
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
This project currently has no external dependencies. To add dependencies, update the `Cargo.toml` file and rebuild the project:
```toml
[dependencies]
regex = "1.7.0"  # Example dependency
```

Install and build:
```bash
cargo build
```

---

## Contributing
Contributions are welcome! Please fork this repository, make your changes, and submit a pull request.

1. Fork the repository.
2. Create your feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add some feature"
   ```
4. Push to the branch:
   ```bash
   git push origin feature/your-feature-name
   ```
5. Open a pull request.

---

## Acknowledgments
- [Advent of Code](https://adventofcode.com/) by Eric Wastl.
- The Rust community for excellent documentation and tooling.
- The Rust Programming Language @ Discord: https://discord.gg/Aa8N6py6sQ
- Rust Progamming Language Community: https://discord.gg/rust-lang-community

Happy coding!


