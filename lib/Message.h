#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <cstring>

template <typename T>
struct MessageHeader {
    T id{};
    uint32_t size = 0;
    MessageHeader() = default;
    MessageHeader(const T& id_);
};

template <typename T>
struct Message {
    MessageHeader<T> header{};
    std::vector<uint8_t> body;

    Message() = default;
    Message(const T& id_);

    size_t size() const ;

    template <typename U>
    friend std::ostream& operator << (std::ostream& os, const Message<U>& msg) {
        os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
        return os;
    }

    template <typename U, typename DT>
    friend Message<U>& operator << (Message<U>& msg, const DT& data) {
        static_assert(std::is_standard_layout<DT>::value, "Data is too complex to push into vector");

        size_t cache = msg.body.size();
        msg.body.resize(msg.body.size() + sizeof(DT));
        std::memcpy(msg.body.data() + cache, &data, sizeof(DT));
        msg.header.size = msg.size();

        return msg;
    }

    template <typename U, typename DT>
    friend Message<U>& operator >> (Message<U>& msg, DT& data) {
        static_assert(std::is_standard_layout<DT>::value, "Data is too complex to push into vector");

        size_t cache = msg.body.size() - sizeof(DT);
        std::memcpy(&data, msg.body.data() + cache, sizeof(DT));
        msg.body.resize(cache);
        msg.header.size = msg.size();

        return msg;
    }
};

// Have to keep overload of operators << & >> because "undefined reference to"
// error occurs on every single type in the world
