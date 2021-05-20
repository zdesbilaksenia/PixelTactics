#pragma once
#include <mutex>
#include <condition_variable>
#include <deque>

template <typename T>
class TsQueue {
public:
    TsQueue() = default;
    TsQueue(const TsQueue<T>& other);
    TsQueue<T>& operator= (const TsQueue<T>& other);
    virtual ~TsQueue();

    const T& front();
    const T& back();

    void pushFront(const T& item);
    void pushBack(const T& item);

    bool empty();
    size_t count();

    void clear();

    T popFront();
    T popBack();

    void wait();
protected:
    std::mutex mutex;
    std::deque<T> deq;

    std::condition_variable cvBlock;
    std::mutex mutexBlock;
};
