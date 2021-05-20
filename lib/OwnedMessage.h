#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "Message.h"

template <typename T>
class TcpConnection;

template <typename T>
struct OwnedMessage {
    boost::shared_ptr<TcpConnection<T>> remote = nullptr;
    Message<T> msg;

    template <typename U>
    friend std::ostream& operator << (std::ostream& os, const OwnedMessage<U>& msg);
};

