#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <queue>
#include <string>
#include <condition_variable>

using namespace std;

mutex mtx;
mutex queueMtx;
condition_variable cv;
queue<string> messageQueue;
bool allTasksDone = false;

void sendMessage(const string& msg) {
    lock_guard<mutex> lock(queueMtx);
    messageQueue.push(msg);
    cv.notify_one();
}

// Simulated task
void taskFunction(int taskID) {
    for (int i = 0; i < 3; ++i) {
        {
            lock_guard<mutex> lock(mtx);
            cout << "[Core] Task " << taskID 
                 << " executing (Iteration " << i + 1 << ")..." << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(500));

        sendMessage("Task " + to_string(taskID) + 
                    " finished iteration " + to_string(i + 1) + "\n");
    }

    sendMessage("Task " + to_string(taskID) + " completed!\n");
}

// Scheduler (round-robin-ish, but here just launches all tasks)
void roundRobinScheduler(int numTasks) {
    vector<thread> taskThreads;

    for (int i = 0; i < numTasks; ++i) {
        taskThreads.emplace_back(taskFunction, i + 1);
    }

    for (auto& th : taskThreads) {
        th.join();
    }

    // All tasks done → notify reader
    {
        lock_guard<mutex> lock(queueMtx);
        allTasksDone = true;
    }
    cv.notify_all();
}

int main() {
    cout << "\n🚀 Simple Multithreaded Task Scheduler (Portable IPC Simulation)\n";

    int numTasks = 3;

    // Start scheduler
    thread scheduler(roundRobinScheduler, numTasks);

    // Reader thread (simulate IPC receiver)
    while (true) {
        unique_lock<mutex> lock(queueMtx);
        cv.wait(lock, [] { return !messageQueue.empty() || allTasksDone; });

        while (!messageQueue.empty()) {
            cout << "[IPC] " << messageQueue.front();
            messageQueue.pop();
        }

        if (allTasksDone && messageQueue.empty()) {
            break;
        }
    }

    scheduler.join();
    cout << "\n✅ All tasks completed. Scheduler exiting.\n";
    return 0;
}
