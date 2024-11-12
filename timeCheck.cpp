#include "timeCheck.h"


int getPrice(const int id, const vector<Thing> &market) {
    for (auto thing : market) {
        if (thing.id == id) return thing.price;
    }
    throw runtime_error("there no thing with this id");
}


void singleThread(const vector<Thing> &market, const int iterCount = ITER_COUNT) {
    for (int price, i = 0; i < iterCount; ++i) {
        price = getPrice(3, market);
    }
}



void multyThread(const vector<Thing> &market) {
    vector<thread> threads;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back(singleThread, market, ITER_COUNT/THREAD_COUNT);
    }
    for (auto& th : threads) {
        th.join();
    }
}


void taskSecond() {
    const vector<Thing> market{
            {1, 40, 50},
            {2, 4, 100},
            {3, 8, 52}
    };
    auto start = std::chrono::high_resolution_clock::now();
    singleThread(market);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    cout << "Single thread time = " << duration << endl;
    start = std::chrono::high_resolution_clock::now();
    multyThread(market);
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "multy thread time = " << duration << endl;
}