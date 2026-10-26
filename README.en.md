[Polski](README.md) | **English**

# Dining Philosophers Problem

A C++ implementation of the classic **dining philosophers** synchronisation problem, with a live terminal visualisation in **ncurses**.

Every philosopher runs in its own thread. Each one thinks for 1–3 s, waits for both forks, eats (about 4 s, shown as a progress bar), puts the forks down and finishes. The program ends a few seconds after every philosopher has eaten.

## How deadlock and starvation are avoided

- A philosopher picks up **both forks at once, or none** — the check and the pick-up happen under one shared `std::mutex`, so the circular wait that causes deadlock cannot occur.
- Each philosopher has their own `std::condition_variable`. A philosopher waiting for forks sleeps on it instead of busy-waiting.
- After eating, a philosopher wakes only their **left and right neighbours**, and only if those neighbours can actually eat now.
- `std::atomic<bool> running` lets all threads shut down cleanly.

## Visualisation

A separate thread redraws the screen every 50 ms and shows:

- the state of each philosopher — *Filozofuje* (thinking), *Czeka* (waiting), *Je* (eating),
- an eating progress bar for each philosopher,
- the state of each fork — *wolny* (free) / *zajęty* (taken).

The interface text is in Polish.

## Build and run

Requires Linux/macOS with the ncurses library (`sudo apt install libncurses-dev` on Debian/Ubuntu).

```bash
g++ -std=c++17 -O2 main.cpp -o filozofowie -lncurses -pthread
./filozofowie 5
```

The argument is the number of philosophers (at least 5).
