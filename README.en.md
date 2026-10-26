🇵🇱 [Polski](README.md) | 🇬🇧 **English**

# 🍝 Dining Philosophers Problem

![C++](https://img.shields.io/badge/C++-00599C?logo=cplusplus&logoColor=white) ![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)

> 🎓 Project for the **Operating Systems** course (Wrocław University of Science and Technology)

The classic thread-synchronisation problem: philosophers sit at a round table and each needs two forks — shared with their neighbours — to eat. The solution is deadlock-free, and the simulation is visualised in the terminal.

## ✨ What I did

- Every philosopher is a **separate thread** (`std::thread`)
- Forks are taken **both at once or not at all** (under one `mutex`), so deadlock can't happen
- A waiting philosopher **sleeps** on a `condition_variable` instead of busy-waiting, and after eating wakes only the neighbours who can now eat
- **ncurses visualisation** — each philosopher's state (thinking / waiting / eating), eating progress bars and fork states, refreshed every 50 ms

## 🛠️ Tech

C++17 · `std::thread` · `mutex` · `condition_variable` · `atomic` · ncurses

## 🚀 Run it

```bash
g++ -std=c++17 -O2 main.cpp -o filozofowie -lncurses -pthread
./filozofowie 5    # number of philosophers (at least 5)
```
