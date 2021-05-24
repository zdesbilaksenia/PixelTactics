#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <cstring>

#include "CardInfo.h"

template <typename T>
struct MessageHeader {
    T id{};
    uint32_t size = 0; MessageHeader() = default; MessageHeader(const T& id_);
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

    template <typename U, typename DT>
    friend Message<U>& operator << (Message<U>& msg, const std::vector<DT>& data) {
        for (int i = data.size() - 1; i >= 0; --i) {
            DT elem = data[i];
            msg << elem;
        }
        msg << data.size();
    }

    template <typename U, typename DT>
    friend Message<U>& operator >> (Message<U>& msg, std::vector<DT>& data) {
        size_t len = 0;
        msg >> len;
        for (size_t i = 0; i < len; ++i) {
            DT elem;
            msg >> elem;
            data.push_back(elem);
        }
    }

    template <typename U>
    friend Message<U>& operator << (Message<U>& msg, const std::string& data) {
        for (int i = data.length(); i >= 0; --i) {
            char elem = data[i];
            msg << elem;
        }
        msg << size_t(data.length() + 1);
    }

    template <typename U>
    friend Message<U>& operator >> (Message<U>& msg, std::string& data) {
        size_t len = 0;
        msg >> len;
        for (size_t i = 0; i < len; ++i) {
            char elem = 0;
            msg >> elem;
            data.push_back(elem);
        }
    }

    template <typename U>
    friend Message<U>& operator << (Message<U>& msg, CardInfo& data) {
        msg << data.backLinePower;
        msg << data.middleLinePower;
        msg << data.frontLinePower;
        msg << data.strength;
        msg << data.HP;
        msg << data.name;
        msg << data.textureId;
        msg << data.ID;
    }

    template <typename U>
    friend Message<U>& operator >> (Message<U>& msg, CardInfo& data) {
        msg >> data.ID;
        msg >> data.textureId;
        msg >> data.name;
        msg >> data.HP;
        msg >> data.strength;
        msg >> data.frontLinePower;
        msg >> data.middleLinePower;
        msg >> data.backLinePower;
    }
};

// Have to keep overload of operators << & >> because "undefined reference to"
// error occurs on every single type in the world
