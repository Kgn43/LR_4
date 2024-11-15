#include "primitives.h"
#include "timeCheck.h"

counting_semaphore globSem{2};
mutex mut;
vector<int> someGeneralData{1,3,2};
random_device rd;
mt19937 gen(rd());
uniform_int_distribution distribution(0, 5);
void signal();

class Writer {
    thread potok;
    uint32_t priority;
public:
    binary_semaphore lock{1};
    Writer (int data) : priority(distribution(gen)) {
        potok = thread(&Writer::write, this, data);
        cout << "Writer " << this << " created" << endl;
    }
    ~Writer() {
        if (potok.joinable()) {
            potok.join();
        }
    }
    void write(const int &data) {
        while (true) {
            if (lock.try_acquire()) {
                {
                    lock_guard<mutex> lock(mut);
                    someGeneralData.push_back(data);
                }
                lock.release();
                sleep(2);
                return;
            }
        }
    }
    uint32_t getPriority() const {
        return priority;
    }
};


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
        //this_thread::sleep_for(chrono::seconds(1));
        signal();
        globSem.release();
    }
    ~Reader() {
        if (potok.joinable()) {
            potok.join();
        }
    }
};


vector<shared_ptr<Reader>> readers;
vector<shared_ptr<Writer>> writers;


void signal() {
    writers[0]->lock.acquire();
}

void taskThird() {
    for (int i = 0; i < 10; ++i) {
        // readers.emplace_back(make_shared<Reader>(someGeneralData));
        // readers.emplace_back(make_shared<Reader>(someGeneralData));
        // readers.emplace_back(make_shared<Reader>(someGeneralData));
        if (int randNum = distribution(gen); writers.empty()) {
            writers.emplace_back(make_shared<Writer>(randNum));
        }
        else {
            for (auto j = writers.begin(); j != writers.end(); ++j) {
                if (j->get()->getPriority() < randNum) {
                    writers.insert(j-1, make_shared<Writer>(randNum));
                }
                if (j + 1 == writers.end()) {
                    writers.emplace_back(make_shared<Writer>(randNum));
                }
            }
        }
    }
}

int main() {
    // taskOne();
    // taskSecond();
    taskThird();
    cout << someGeneralData.size() << endl;
    return 0;
}