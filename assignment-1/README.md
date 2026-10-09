# Data Structures: Assignment 1

Four C++ programs, each built on a hand-written data structure. No STL containers (`vector`, `list`, `map`, etc.) are used anywhere.

## Contents

| File | Problem | Data structure |
|------|---------|----------------|
| `q1.cpp` | **Grayscale Pixel Grid**: image toolkit with bounds-checked pixel access, 90° rotation, horizontal flip, histogram and 3x3 box blur | Fixed-size 2D array (raw `int**`) |
| `q2.cpp` | **Growable Embedding Store**: vector store with append, remove, and brute-force nearest-neighbor search by Euclidean distance | Dynamic array that doubles when full |
| `q3.cpp` | **Round-Robin GPU Job Scheduler**: time-sliced scheduling with job removal and per-job completion times | Singly circular linked list |
| `q4.cpp` | **Chat Conversation History**: add, delete, edit and evict old messages from a conversation | Doubly linked list |

## Build and run

Needs `g++` (any version with C++11 or later).

```bash
g++ -Wall -o q1 q1.cpp && ./q1
g++ -Wall -o q2 q2.cpp && ./q2
g++ -Wall -o q3 q3.cpp && ./q3
g++ -Wall -o q4 q4.cpp && ./q4
```

Each program reads from standard input. You can pipe a file in:

```bash
./q1 < input.txt
```

## Input formats

**Q1:** `rows cols`, then `rows*cols` integers (0 to 255) in row-major order, then commands: `ROTATE`, `FLIP`, `HIST`, `BLUR`, `PRINT`.

**Q2:** dimension `D`, then commands: `ADD v1 ... vD`, `REMOVE index`, `QUERY v1 ... vD`, `STATE`. Size and capacity are printed after every `ADD` and `REMOVE`, so resize events are visible.

**Q3:** number of jobs, then `jobId burstTime` pairs, then the quantum. Prints each time slice, the total simulation time and each job's completion time.

**Q4:** commands: `ADD id role text`, `DELETE id`, `EDIT id newText`, `LIMIT n`, `PRINT`.

## Notes

- Q1 rotates by allocating only the new `cols x rows` grid, with no oversized helper buffers.
- Q2 doubles capacity when full, so appends are amortized O(1).
- Q3 handles the edge case where the last remaining job finishes and the circle becomes empty.
- Q4 handles removal at the head, tail and middle.

Coursework for FAST-NUCES Islamabad.
