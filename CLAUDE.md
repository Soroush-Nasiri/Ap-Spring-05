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
- **CA4** — Music player assignment using the `miniaudio.h` single-header audio library; assignment spec in `CA4/CA4.html`

## CA4 Assignment Detail (CA4/CA4.html)

The CA4 assignment is a fully designed A4-paged HTML document (13 sections). Key decisions already made:

### Structure
- **13 sections**: مقدمه, اهداف, نمای کلی, ساختار پوشه, مدل داده, معماری, جزئیات قابلیت‌ها, TUI, مدیریت خطا, OOP, Bonus, راهنمایی, miniaudio
- **Fixed folder structure** (no CLI args): app runs as `./music_player` and reads from:
  - `Data/library.csv` — song metadata
  - `Data/settings.cfg` — saved state (created by the app)
  - `Data/Playlists/*.m3u` — playlist files
  - `Data/Musics/*.mp3` — audio files
  - `src/miniaudio.h` — provided audio library (do not modify)

### Bonus Table (Part 11)
Raw Keyboard Input (+15), Full ncurses TUI (+20), Play History (+10), Favourites (+5), Add to Playlist (+10), **Seek ←→ (+10)**

### miniaudio (Part 13)
- Uses **high-level API** (`ma_engine` + `ma_sound`), NOT the low-level device/callback API
- Code example at top of Part 13 uses: `ma_sound_init_from_file` + `ma_sound_start` + `ma_sound_uninit`
- API table maps each function to its Player equivalent (play/pause/stop/resume/getCurrentTime)
- Seek subsection (13.4) is for the Bonus seek feature only

### Designers
طراح تمرین: سروش نصیری و ایلیا افضلی — بهار ۱۴۰۵

---

## Code Conventions

- Solutions use `using namespace std;` and standard C++ headers.
- Avoid STL algorithms (`<algorithm>`, `sort`, etc.) unless required — earlier CAs deliberately implement sorting manually (bubble sort in CA0).
- Input is read from `stdin`, output to `stdout`; no file I/O in solutions.
- Classes follow standard OOP patterns: private fields, public getters, constructor initialization.
