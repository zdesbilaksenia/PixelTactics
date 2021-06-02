#include "Message.h"
#include "GameMsgTypes.h"
#include <cstdint>
#include <iostream>
#include <vector>
#include <cstring>

template <typename T>
MessageHeader<T>::MessageHeader(const T& id_) : id(id_) {}

template class MessageHeader<GameMsgTypes>;


template <typename T>
Message<T>::Message(const T& id_) : header(id_) {}

template <typename T>
size_t Message<T>::size() const {
    return body.size();
}

template class Message<GameMsgTypes>;
