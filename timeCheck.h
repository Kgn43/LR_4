#ifndef TIMECHECK_H
#define TIMECHECK_H

#include <iostream>
#include <vector>
#include <thread>
#include <exception>
#include <chrono>

using std::vector, std::runtime_error, std::thread, std::cout, std::endl;


#define ITER_COUNT 10000000
#define THREAD_COUNT 8


struct Thing {
    int id;
    int number;
    int price;
    Thing () {}
    Thing (int ID, int NUMBER, int PRICE) : id(ID), number(NUMBER), price(PRICE) {};
};


void taskSecond();

#endif //TIMECHECK_H
