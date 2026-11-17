# SCOT — Scientific Conference Organization Tool

![C++](https://img.shields.io/badge/C%2B%2B17-00599C?logo=cplusplus&logoColor=white) ![CMake](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=white) ![Grade](https://img.shields.io/badge/Grade-19%2F20-brightgreen)

A tool that **assigns reviewers to scientific papers** automatically and optimally, respecting each reviewer's area of expertise, the minimum number of reviews per paper and the maximum workload per reviewer.

> Project 1 for **Algorithm Design (DA)** — BSc in Informatics and Computing Engineering (L.EIC), FEUP, 2025/2026. **Final grade: 19/20.**

## Approach
The problem is modelled as a **maximum-flow network**:

`source → papers → reviewers (matching expertise) → sink`

Edge capacities encode the constraints: *MinReviewsPerSubmission* on the paper edges and *MaxReviewsPerReviewer* on the reviewer edges. The tool solves it with the **Edmonds-Karp** algorithm (BFS-based augmenting paths). A full flow means a valid assignment exists; otherwise the tool reports which papers cannot be covered.

## Features
- Robust **CSV parser** that handles comments, quotes and multiple sections
- **Interactive CLI menu** and a **batch mode** for automated runs
- 14 test datasets with their expected outputs
- Code documented with **Doxygen**

## Structure
`Models` (data) · `Parser` (input) · `Graph/Vertex/Edge` (residual graph) · `App` (controller and algorithms) · `Menu` (CLI)

## ▶ How to run
```bash
cmake -S . -B build
cmake --build build
./build/myProg # interactive mode
```

## My contribution
- Project setup (CMake), the **interactive menu and batch CLI**
- **CSV parsing** without duplicates, and its integration with the graph
- Core application logic and the README

## Team
André Rocha and two colleagues (group T06_G)

 Original documentation (Portuguese): [README_PT.md](README_PT.md)
