#include "Readers_Writers.h"


counting_semaphore globSem{2};
mutex mut;
vector<int> someGeneralData{1,3,2};
random_device rd;
mt19937 gen(rd());
uniform_int_distribution distribution(1, 5);


class Reader {
    thread potok;
public:
    Reader (vector<int> &vec) {
        potok = thread(&Reader::read, this, ref(vec));
    }
    void read(const vector<int> &vec) const {
        globSem.acquire();
        cout << "Reader " << this_thread::get_id() << " reading: ";
        for (const auto &i : vec) {
            cout << i << ' ';
        }
        cout << endl;
        this_thread::sleep_for(chrono::seconds(1));
        globSem.release();
    }
    ~Reader() {
        if (potok.joinable()) {
            potok.join();
        }
    }
};



class Writer {
    thread potok;
    uint32_t priority;
    bool existed;
public:
    Writer (const vector<shared_ptr<Writer>> &vec, int data) : priority(distribution(gen)) {
        potok = thread(&Writer::write, this, ref(vec), data);
        existed = false;
        cout << "Writer " << this->potok.get_id() << " created with priority " << priority << " and value " << data << endl;
    }
    ~Writer() {
        if (potok.joinable()) {
            potok.join();
        }
    }
    bool isThisTreadWithMaxPriority (const vector<shared_ptr<Writer>> &vec) const {
        this_thread::sleep_for(chrono::seconds(1));
        uint32_t maxPriority = 0;
        for (const auto &i : vec) {
            if (i->getPriority() > maxPriority && i->getExisted() == false) {
                maxPriority = i->getPriority();
            }
        }
        return maxPriority == this->priority;
    }
    void write(const vector<shared_ptr<Writer>> &vec, const int &data) {
        while (true) {
            if (isThisTreadWithMaxPriority(vec)) {
                lock_guard<mutex> lock(mut);
                someGeneralData.push_back(data);
                //this_thread::sleep_for(chrono::seconds(4));
                existed = true;
                return;
            }
        }
    }
    uint32_t getPriority() const {
        return priority;
    }
    bool getExisted() const {
        return existed;
    }
};

void taskThird() {
    vector<shared_ptr<Reader>> readers;
    vector<shared_ptr<Writer>> writers;
    readers.reserve(10);
    writers.reserve(10);
    for (int i = 0; i < 4; ++i) {
        readers.emplace_back(make_shared<Reader>(someGeneralData));
        readers.emplace_back(make_shared<Reader>(someGeneralData));
        int randNum = distribution(gen);
        writers.emplace_back(make_unique<Writer>(ref(writers), randNum));
    }
}