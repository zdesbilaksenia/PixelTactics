#include "TsQueue.h"
#include "GameMsgTypes.h"
#include "OwnedMessage.h"
#include "Message.h"
#include <mutex>
#include <condition_variable>
#include <deque>

template <typename T>
TsQueue<T>::TsQueue(const TsQueue<T>& other) : deq(other.deq) {
}
template <typename T>
TsQueue<T>& TsQueue<T>::operator= (const TsQueue<T>& other) {
    // if (deq != other.deq) {
        deq = other.deq;
    // }
    return *this;
}
template <typename T>
TsQueue<T>::~TsQueue() { clear(); };

template <typename T>
const T& TsQueue<T>::front() {
    std::scoped_lock lock(mutex);
    return deq.front();
}

template <typename T>
const T& TsQueue<T>::back() {
    std::scoped_lock lock(mutex);
    return deq.back();
}

template <typename T>
void TsQueue<T>::pushFront(const T& item) {
    std::scoped_lock lock(mutex);
    deq.emplace_front(item);

    std::unique_lock<std::mutex> ul(mutexBlock);
    cvBlock.notify_one();
}

template <typename T>
void TsQueue<T>::pushBack(const T& item) {
    std::scoped_lock lock(mutex);
    deq.emplace_back(item);

    std::unique_lock<std::mutex> ul(mutexBlock);
    cvBlock.notify_one();
}

template <typename T>
bool TsQueue<T>::empty() {
    std::scoped_lock lock(mutex);
    return deq.empty();
}

template <typename T>
size_t TsQueue<T>::count() {
    std::scoped_lock lock(mutex);
    return deq.size();
}

template <typename T>
void TsQueue<T>::clear() {
    std::scoped_lock lock(mutex);
    deq.clear();
}

template <typename T>
T TsQueue<T>::popFront() {
    std::scoped_lock lock(mutex);
    T cache = std::move(deq.front());
    deq.pop_front();
    return cache;
}

template <typename T>
T TsQueue<T>::popBack() {
    std::scoped_lock lock(mutex);
    T cache = std::move(deq.back());
    deq.pop_back();
    return cache;
}

template <typename T>
void TsQueue<T>::wait() {
    while (empty()) {
        std::unique_lock<std::mutex> ul(mutexBlock);
        cvBlock.wait(ul);
    }
}

template class TsQueue<Message<GameMsgTypes>>;
template class TsQueue<OwnedMessage<GameMsgTypes>>;
