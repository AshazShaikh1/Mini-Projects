#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <chrono>
using namespace std;

// ------------------- IPC Message Queue -------------------
class MessageQueue {
private:
    queue<string> messages;
    mutex mtx;
    condition_variable cv;

public:
    void send(const string& msg) {
        unique_lock<mutex> lock(mtx);
        messages.push(msg);
        cv.notify_all();
    }

    string receive() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&] { return !messages.empty(); });
        string msg = messages.front();
        messages.pop();
        return msg;
    }
};

// ------------------- Process Simulation -------------------
class Process {
public:
    int id;
    int burstTime;

    Process(int pid, int bt) : id(pid), burstTime(bt) {}

    void run(MessageQueue &mq) {
        // Simulate doing some work
        for (int i = 0; i < burstTime; i++) {
            this_thread::sleep_for(chrono::milliseconds(500));
            cout << "Process " << id << " running step " << i + 1 << endl;
        }

        // After finishing, send message to other processes
        mq.send("Process " + to_string(id) + " has finished execution.");
    }
};

// ------------------- Round Robin Scheduler -------------------
class Scheduler {
private:
    vector<Process> processes;
    int timeQuantum;
    MessageQueue &mq;

public:
    Scheduler(vector<Process> p, int tq, MessageQueue &mqRef)
        : processes(p), timeQuantum(tq), mq(mqRef) {}

    void schedule() {
        vector<thread> threads;

        for (auto &p : processes) {
            threads.push_back(thread([&]() {
                int remainingTime = p.burstTime;
                while (remainingTime > 0) {
                    int execTime = min(timeQuantum, remainingTime);
                    cout << "Scheduling Process " << p.id << " for " << execTime << " units." << endl;
                    this_thread::sleep_for(chrono::seconds(execTime));
                    remainingTime -= execTime;
                }
                mq.send("Process " + to_string(p.id) + " completed!");
            }));
        }

        // Join all threads
        for (auto &t : threads) {
            if (t.joinable()) t.join();
        }
    }
};

// ------------------- Main -------------------
int main() {
    MessageQueue mq;

    // Example processes
    vector<Process> processList = {
        Process(1, 3),
        Process(2, 4),
        Process(3, 2)
    };

    // Create Scheduler (time quantum = 1)
    Scheduler scheduler(processList, 1, mq);

    // Run Scheduler on another thread (multi-core simulation)
    thread schedulerThread(&Scheduler::schedule, &scheduler);

    // Run IPC listener on another thread
    thread ipcListener([&]() {
        for (int i = 0; i < processList.size(); i++) {
            string msg = mq.receive();
            cout << "[IPC] Received: " << msg << endl;
        }
    });

    schedulerThread.join();
    ipcListener.join();

    cout << "All processes completed successfully!" << endl;
    return 0;
}