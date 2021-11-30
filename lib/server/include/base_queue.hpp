#pragma once

#include <queue>
#include <mutex>

namespace net {
    template<class T>
    class BaseQueue {
    public:
        BaseQueue() = default;

        ~BaseQueue();

        void Push(T &element);

        T Pop();

        bool IsEmpty();

    private:
        std::queue<T> queue_;
        std::mutex queue_mutex;
    };
}

