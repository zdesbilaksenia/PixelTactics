#include "OwnedMessage.h"
#include "GameMsgTypes.h"
#include <iostream>

template <typename T>
std::ostream& operator << (std::ostream& os, const OwnedMessage<T>& msg) {
    os << msg.msg;
    return os;
}

template class OwnedMessage<GameMsgTypes>;
