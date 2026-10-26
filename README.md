**Polski** | [English](README.en.md)

# Problem ucztujących filozofów

Implementacja klasycznego problemu synchronizacji — **ucztujących filozofów** — w C++, z wizualizacją na żywo w terminalu (**ncurses**).

Każdy filozof działa w osobnym wątku. Myśli przez 1–3 s, czeka na oba widelce, je (ok. 4 s, widoczne jako pasek postępu), odkłada widelce i kończy. Program zamyka się kilka sekund po tym, jak zjedzą wszyscy filozofowie.

## Jak uniknięto zakleszczenia i zagłodzenia

- Filozof bierze **oba widelce naraz albo żadnego** — sprawdzenie i podniesienie widelców odbywa się pod jednym wspólnym `std::mutex`, więc nie powstaje cykliczne oczekiwanie prowadzące do zakleszczenia.
- Każdy filozof ma własną `std::condition_variable`. Filozof czekający na widelce śpi na niej zamiast aktywnie czekać.
- Po jedzeniu filozof budzi tylko **lewego i prawego sąsiada** i tylko wtedy, gdy ci mogą już jeść.
- `std::atomic<bool> running` pozwala poprawnie zakończyć wszystkie wątki.

## Wizualizacja

Osobny wątek odświeża ekran co 50 ms i pokazuje:

- stan każdego filozofa — *Filozofuje*, *Czeka*, *Je*,
- pasek postępu jedzenia każdego filozofa,
- stan każdego widelca — *wolny* / *zajęty*.

## Kompilacja i uruchomienie

Wymagany Linux/macOS z biblioteką ncurses (`sudo apt install libncurses-dev` na Debianie/Ubuntu).

```bash
g++ -std=c++17 -O2 main.cpp -o filozofowie -lncurses -pthread
./filozofowie 5
```

Argument to liczba filozofów (co najmniej 5).
