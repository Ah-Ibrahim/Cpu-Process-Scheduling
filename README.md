# Process Scheduler (C++ Implementation)

## Overview

This project implements a **process scheduling simulator** in C++ that supports multiple scheduling algorithms. The program reads scheduling parameters from standard input, simulates process execution, and prints either a trace of execution or statistics based on user input.

## Features

- **Supports Multiple Scheduling Algorithms:**
  - First Come First Serve (FCFS)
  - Round Robin (RR)
  - Shortest Process Next (SPN)
  - Shortest Remaining Time (SRT)
  - Highest Response Ratio Next (HRRN)
  - Feedback Queues (FB-1, FB-2i)
  - Aging
- **Trace Mode:** Displays a step-by-step execution of processes.
- **Statistics Mode:** Computes process completion times, turnaround times, and normalized turnaround times.

## Compilation & Execution

### Compile the Program:

```sh
make # using Makefile or
g++ -g scheduler.cpp -o scheduler
```

### Run the Program:

```sh
./scheduler < input.txt
```

The input is read from standard input (`stdin`). You can redirect a file containing scheduling parameters.

## Scheduling Algorithms

### 1) First Come First Serve (FCFS)

- Processes are scheduled in the order of their arrival.

### 2) Round Robin (RR-x)

- Each process is given a fixed time slice (quantum `x`).
- If unfinished, the process is moved to the end of the queue.

### 3) Shortest Process Next (SPN)

- Selects the process with the shortest service time.

### 4) Shortest Remaining Time (SRT)

- Preemptive version of SPN, choosing the process with the least remaining time.

### 5) Highest Response Ratio Next (HRRN)

- Prioritizes processes with a high response ratio.

### 6) Feedback Queues (FB-1, FB-2i)

- Uses multiple queues with different priority levels.

### 7) Aging

- Prevents starvation by increasing priority over time.

## Code Structure

- **`scheduler.h`**: Header file defining process structures and scheduling logic.
- **`scheduler.cpp`**: Implements parsing, scheduling algorithms, and result output.

## Input Format

1. **Trace or Statistics Mode:** (`trace` or `stats`)
2. **Algorithms:** Comma-separated list of algorithm numbers (e.g., `1,2-3,4` indicates FCFC, RR-3 and SRT).
3. **Last Instant:** Integer defining the maximum simulation time.
4. **Number of Processes:** Integer representing the total number of processes.
5. **Process List:**
   - Each process is defined in a new line as `name,arrival_time,service_time`

## Output Format

### 1) trace mode

A time figure is shown for all processes.
(\*) -> indicates the process is active (running on cpu).
(.) -> indicates the process is blocked (waiting for cpu).

### 2) stats mode

Shows different stats (arrival time, service time (burst time), etc.) about processes.

## Sample Input and Output

Input (trace mode for first scheduling algorithm):

```
trace
1
20
5
A,0,3
B,2,6
C,4,4
D,6,5
E,8,2
```

Output:

```
FCFS  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
------------------------------------------------
A     |*|*|*| | | | | | | | | | | | | | | | | |
B     | | |.|*|*|*|*|*|*| | | | | | | | | | | |
C     | | | | |.|.|.|.|.|*|*|*|*| | | | | | | |
D     | | | | | | |.|.|.|.|.|.|.|*|*|*|*|*| | |
E     | | | | | | | | |.|.|.|.|.|.|.|.|.|.|*|*|
------------------------------------------------


```

Input (stats mode for first scheduling algorithm):

```
stats
1
20
5
A,0,3
B,2,6
C,4,4
D,6,5
E,8,2
```

Output:

```
FCFS
Process    |  A  |  B  |  C  |  D  |  E  |
Arrival    |  0  |  2  |  4  |  6  |  8  |
Service    |  3  |  6  |  4  |  5  |  2  | Mean|
Finish     |  3  |  9  | 13  | 18  | 20  |-----|
Turnaround |  3  |  7  |  9  | 12  | 12  | 8.60|
NormTurn   | 1.00| 1.17| 2.25| 2.40| 6.00| 2.56|
```

## Author

Developed by Ahmed Ibrahim. Contributions and feedback are welcome!
