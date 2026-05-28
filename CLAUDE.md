# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Context

This is the **Advanced Programming (AP) course repository** for Spring 1405 at the Department of Engineering Science. Soroush Nasiri is the Chief TA. The repo contains C++ solutions to computer assignments (CA0–CA4) and quiz materials.

## Building and Running

Each assignment solution is a standalone `.cpp` file. Compile with:

```bash
g++ -std=c++17 -o solution CAx/Qy.cpp
```

For CA3, a `Makefile` is expected (the judge script calls `make`). The compiled binary must be named `installer` in the CA3 directory.

## Testing

CA1 and CA3 include a local judge script and test cases under `testCases/in/` and `testCases/out/`.

**CA3 local judge** (handles build + test run automatically):
```bash
cd CA3 && bash localJudge.sh
```
The judge compiles via `make`, runs up to 100 test cases from `testCases/in/` and `testCases/out/`, enforces a 3-second time limit, and reports AC/WA/TLE/RE per test.

**Manual single test:**
```bash
./solution < testCases/in/in1.txt
```

**Test generators** (Python scripts like `Q1Gen.py`, `Q2Gen.py`, etc.) produce random test inputs for their corresponding question.

## Assignment Structure

- **CA0** — Word frequency counter (C++, no STL containers beyond `vector<string>`)
- **CA1** — Task manager with dependencies (OOP, `class Task`, `shared_ptr`)
- **CA2** — Three problems: Sudoku solver (Q1), Trie-based word search (Q2), N-Queens with fixed pieces (Q3)
- **CA3** — Competitive-style problem judged via `localJudge.sh`; solution compiled to binary named `installer`
- **CA4** — Music player assignment using the `miniaudio.h` single-header audio library; assignment spec in `CA4_MusicPlayer_Assignment-2 (1).html`

## Code Conventions

- Solutions use `using namespace std;` and standard C++ headers.
- Avoid STL algorithms (`<algorithm>`, `sort`, etc.) unless required — earlier CAs deliberately implement sorting manually (bubble sort in CA0).
- Input is read from `stdin`, output to `stdout`; no file I/O in solutions.
- Classes follow standard OOP patterns: private fields, public getters, constructor initialization.
