🇵🇱 **Polski** | 🇬🇧 [English](README.en.md)

# 🍝 Problem ucztujących filozofów

![C++](https://img.shields.io/badge/C++-00599C?logo=cplusplus&logoColor=white) ![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)

> 🎓 Projekt na przedmiot **Systemy Operacyjne** (Politechnika Wrocławska)

Klasyczny problem synchronizacji wątków: filozofowie siedzą przy okrągłym stole, a żeby zjeść, każdy potrzebuje dwóch widelców, które dzieli z sąsiadami. Rozwiązanie jest wolne od zakleszczeń, a przebieg symulacji jest wizualizowany w terminalu.

## ✨ Co zrobiłem

- Każdy filozof to **osobny wątek** (`std::thread`)
- Widelce brane są **oba naraz albo wcale** (pod jednym `mutex`em), więc zakleszczenie jest niemożliwe
- Czekający filozof **śpi** na `condition_variable` zamiast aktywnie czekać w pętli, a po jedzeniu budzi tylko sąsiadów, którzy mogą już jeść
- **Wizualizacja w ncurses** — stan każdego filozofa (myśli / czeka / je), paski postępu jedzenia i stan widelców, odświeżane co 50 ms

## 🛠️ Technologie

C++17 · `std::thread` · `mutex` · `condition_variable` · `atomic` · ncurses

## 🚀 Uruchomienie

```bash
g++ -std=c++17 -O2 main.cpp -o filozofowie -lncurses -pthread
./filozofowie 5    # liczba filozofów (min. 5)
```
