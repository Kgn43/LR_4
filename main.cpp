#include "primitives.h"
#include "timeCheck.h"

counting_semaphore globSem{2};
mutex mut;
vector<int> someGeneralData{1,3,2};
random_device rd;
mt19937 gen(rd());
uniform_int_distribution distribution(0, 5);

class Thread {
protected:
    thread potok;

};


class Writer : public Thread{
    thread potok;
    uint32_t priority;

public:
    Writer (int data) : priority(distribution(gen)) {
        potok = thread(&Writer::write, this, data);
    }
    ~Writer() {
        if (potok.joinable()) {
            potok.join();
        }
    }
    void write(const int data) {
        while (true) {
            if (globSem.try_acquire()) {
                {

                    someGeneralData.push_back(data);
                }
                 sleep(2);
                return;
            }
        }

    }
};


class Reader : public Thread{
    thread potok;
public:
    Reader (vector<int> &vec) {
        potok = thread(&Reader::read, this, vec);
    }
    void read(vector<int> &vec) {
        globSem.acquire();
        cout << potok.get_id() << endl;
        for (auto i : someGeneralData) {
            cout << i << ' ';
        }
        cout << endl;
        sleep(1);
        globSem.release();
    }
    ~Reader() {
        if (potok.joinable()) {
            potok.join();
        }
    }
};


void taskThird() {
    vector<shared_ptr<Thread>> threads;
    for (int i = 0; i < 10; ++i) {
        Reader;
        Reader;
        Reader;
        //threads.emplace_back(make_shared<Writer>(distribution(gen)));
    }
}


int main() {
    // taskOne();
    // taskSecond();
    taskThird();
    return 0;
}