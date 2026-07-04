# Breaking the Sorting Barrier for SSSP - Implementation

This repository contains the implementation of the deterministic sub-linear time algorithm for the Single-Source Shortest Path (SSSP) problem.

## 🚀 Running Tests

You can easily run the tests using the provided `Makefile`:

* **Unit Tests:**
    `make run_unit_tests`
* **Correctness Tests** (small randomly generated graphs):
    `make run_correctness_test`
* **Performance/Speed Tests:**
    `make run_speed_test`

## 💻 Running the Algorithm (Custom Inputs)

To run the algorithm on your own graph data, compile and execute using:

`make run`

### Input Format (Standard Input)
The program expects data via standard input (stdin) in the following format:

1.  **First line:** 4 integers `n m s d`
    * `n`: Number of vertices 
    * `m`: Number of edges
    * `s`: Start vertex ID
    * `d`: Graph type (`1` for directed, `0` for undirected)
2.  **Next `m` lines:** 3 integers per line representing an edge: `u v weight`
    * *Note: Vertices should be labeled from 1 to `n`.*

### Output Format
* **Stdout:** `n` lines. The i-th line contains the vertex ID and its calculated shortest distance from the start vertex `s`.
* **Stderr (Standard Error):** Graph statistics (the printed edge count refers to directed edges) and the execution times of specific algorithm phases (init, solve, get, etc.).