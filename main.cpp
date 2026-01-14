#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <ctime>

#include <ncurses.h>
#include <clocale>

using namespace std;

enum class State {
    THINKING, // Myśli
    EATING, // Je
    WAITING // Czeka
};

int N; // Liczba filozofów

vector<State> philosophers;
vector<bool> forks;
vector<int> eating_progress;
vector<bool> finished;


mutex mtx;
vector<unique_ptr<condition_variable>> cvs;
atomic<bool> running(true);

int left(int i) {
    return (i + N - 1) % N;
}

int right(int i) {
    return (i + 1) % N;
}

bool can_eat(int i) {
    return philosophers[i] == State::WAITING && forks[i] && forks[right(i)];
}

void take_forks(int i) {
    unique_lock<mutex> lock(mtx);

    philosophers[i] = State::WAITING;

    while (running && !can_eat(i)) {
        cvs[i]->wait(lock);
    }

    if (!running) return;

    forks[i] = false;
    forks[right(i)] = false;
    philosophers[i] = State::EATING;
}


void put_forks(int i) {
    lock_guard<mutex> lock(mtx);

    forks[i] = true;
    forks[right(i)] = true;
    philosophers[i] = State::THINKING;

    int l = left(i);
    int r = right(i);

    if (philosophers[l] == State::WAITING && can_eat(l)) {
        cvs[l]->notify_one();
    }
    if (philosophers[r] == State::WAITING && can_eat(r)) {
        cvs[r]->notify_one();
    }
}

void philosopher_thread(int id) {
    while (running && !finished[id]) {
        // Myślenie
        {
            this_thread::sleep_for(
                chrono::seconds(1 + rand() % 3)
            );
        }


        take_forks(id);

        if (!running) break;

        // Jedzenie
        int duration = 20;
        for (int step = 1; step <= duration && running; step++) 
        {
            {
                lock_guard<mutex> lock(mtx);
                eating_progress[id] = step * 5;
            }
        this_thread::sleep_for(chrono::milliseconds(200));
        }

        {
            lock_guard<mutex> lock(mtx);
            eating_progress[id] = 100;
            finished[id] = true;
        }        
        put_forks(id);
    };
}

void initialize_ncurses() {
    initscr();
    noecho();
    curs_set(0);
    timeout(0);
}

void shutdown_ncurses() {
    endwin();
}

string progress_bar(int percent) {
    const int width = 20;
    int filled = (percent * width) / 100;
    return "[" + string(filled, '#') + string(width - filled, '-') + "]";
} 

bool all_finished() {
    for (bool f : finished) {
        if (!f) return false;
    }
    return true;
}

void visualization_thread() {
    initialize_ncurses();

    while (running) {
        {
        unique_lock<mutex> lock(mtx);
        erase();
        
        mvprintw(0, 0, "Problem ucztujących filozofów");
        mvprintw(1, 0, "--------------------------------");

        for (int i = 0; i < N; i++) {
            const char* state_str;

            switch (philosophers[i]) {
                case State::THINKING:
                    state_str = "Filozofuje";
                    break;
                case State::EATING:
                    state_str = "Je";
                    break;
                case State::WAITING:
                    state_str = "Czeka";
                    break;
                default:
                    state_str = "Nieznany";
            }
            string bar = progress_bar(eating_progress[i]);
            mvprintw(3 + i, 0, "Filozof %2d: %-10s %s", i + 1, state_str, bar.c_str());
        }

        mvprintw(4 + N, 0, "Widelec:");
        for (int i = 0; i < N; i++) {
            mvprintw(5 + N + i, 0, " %2d: %s", i + 1, forks[i] ? "wolny" : "zajęty");
        }
        clrtobot();

        refresh();
    }
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    shutdown_ncurses();
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");
    if (argc != 2) {
        cerr << "Użyj: " << argv[0] << " <liczba_filozofów>\n";
        return 1;
    }

    N = atoi(argv[1]);
    if (N < 5) {
        cerr << "Liczba filozofów musi wynosić conajmniej 5.\n";
        return 1;
    }

    srand(time(nullptr));

    philosophers.resize(N, State::THINKING);
    forks.resize(N, true);
    eating_progress.resize(N, 0);
    finished.resize(N, false);

    cvs.resize(N);
    for (int i = 0; i < N; i++) {
        cvs[i] = make_unique<condition_variable>();
    }

   vector<thread> threads;

    // wątek wizualizacji
    thread vis_thread(visualization_thread);

    // wątki filozofów
    for (int i = 0; i < N; i++) {
        threads.emplace_back(philosopher_thread, i);
    }

    while (!all_finished()) {
        this_thread::sleep_for(chrono::milliseconds(200));
    }

    this_thread::sleep_for(chrono::seconds(5));

    running = false;
    for (int i = 0; i < N; i++) {
        cvs[i]->notify_one();
    }


    for (auto& t : threads) {
        t.join();
    }

    vis_thread.join();

    return 0;
}
