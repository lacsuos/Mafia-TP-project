#pragma once

#include <queue>
#include <mutex>

template<typename T>
class ThreadQueue{
private:
    std::queue<T> queue;
    std::mutex mutex;

public:
    ThreadQueue() = default;
    ~ThreadQueue() {
        mutex.lock();
    }

    void Push(T &value){
        mutex.lock();
        queue.push(value);
        mutex.unlock();   
    }

    T Pop(){
        mutex.lock();
        auto result = queue.front();
        queue.pop();
        mutex.unlock();
        return result;
    }

    bool IsEmpty(){
        mutex.lock();
        bool result = queue.empty();
        mutex.unlock();
        return result;
    }
};