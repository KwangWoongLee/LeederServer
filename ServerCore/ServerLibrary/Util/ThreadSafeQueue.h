#pragma once
#include "stdafx.h"

namespace leeder {

    template<typename T>
    class ThreadSafeQueue {
    public:
        ThreadSafeQueue() {}
        ~ThreadSafeQueue() {}

        void Push(const T& element) {
            std::unique_lock<std::mutex> lock(mQueueMutex);
            mQueue.push(element);
            mQueueEmptyCV.notify_one();
        }

        T Pop() {
            std::unique_lock<std::mutex> lock(mQueueMutex);
            mQueueEmptyCV.wait(lock, [this]() { return !mQueue.empty(); });

            
            auto ret = std::move(mQueue.front());
            mQueue.pop();

            return std::move(ret);
        }

        void Clear() {
            std::unique_lock<std::mutex> lock(mQueueMutex);
            while (!mQueue.empty()) {
                mQueue.pop();
            }
        }

        uint32_t Size() {
            std::unique_lock<std::mutex> lock(mQueueMutex);
            return mQueue.size();
        }

        bool Empty() {
            std::unique_lock<std::mutex> lock(mQueueMutex);
            return mQueue.empty();
        }

    private:
        std::mutex    mQueueMutex;
        std::queue<T> mQueue;
        std::condition_variable mQueueEmptyCV;
    };

}