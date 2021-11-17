//
// Created by ivan on 16.11.2021.
//

#ifndef MAFIA_TP_PROJECT_QUEUE_H
#define MAFIA_TP_PROJECT_QUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class Queue {
private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
public:
    Queue(): q(), m(), c() {}

    ~SafeQueue() {}

    // Add an element to the queue.
    void push(T t) {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T pop() {
        std::unique_lock<std::mutex> lock(m);
        while(q.empty()) {
            c.wait(lock);
        }
        T val = q.front();
        q.pop();
        return val;
    }
};
#endif //MAFIA_TP_PROJECT_QUEUE_H
