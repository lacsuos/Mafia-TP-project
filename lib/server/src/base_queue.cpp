#include "base_queue.hpp"

namespace net {
    template<class T>
    BaseQueue<T>::~BaseQueue() {
        queue_mutex.lock();
    }

    template<class T>
    void BaseQueue<T>::Push(T &element) {
        queue_mutex.lock();
        queue_.push(element);
        queue_mutex.unlock();
    }

    template<class T>
    T BaseQueue<T>::Pop() {
        queue_mutex.lock();
        auto temp = queue_.front();
        queue_.pop();
        queue_mutex.unlock();
        return temp;
    }

    template<class T>
    bool BaseQueue<T>::IsEmpty() {
        queue_mutex.lock();
        bool result = queue_.empty();
        queue_mutex.unlock();
        return result;
    }
}