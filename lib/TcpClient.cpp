#include "TcpClient.h"

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/move/unique_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/move/make_unique.hpp>
#include <boost/thread/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "TcpConnection.h"
#include "Message.h"
#include "OwnedMessage.h"
#include "TsQueue.h"
#include "GameMsgTypes.h"


template <typename T>
TcpClient<T>::TcpClient() {}
template <typename T>
TcpClient<T>::~TcpClient() { disconnect(); }

template <typename T>
bool TcpClient<T>::connect(const std::string& host, const uint16_t port) {
    try {
        boost::asio::ip::tcp::resolver resolver(ioContext);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
        connection = boost::movelib::make_unique<TcpConnection<T>>(
                TcpConnection<T>::owner::client,
                ioContext,
                boost::asio::ip::tcp::socket(ioContext), qMsgIn);
        connection->connectToServer(endpoints);
        thrContext = std::thread([this]() { ioContext.run(); });
    } catch(std::exception& e) {
        std::cerr << "Client Exception: " << e.what() << "\n";
        return false;
    }
    return true;
}
template <typename T>
void TcpClient<T>::disconnect() {
    if (isConnected()) {
        connection->disconnect();
    }

    ioContext.stop();
    if (thrContext.joinable())
        thrContext.join();

    connection.release();
}
template <typename T>
bool TcpClient<T>::isConnected() const {
    if (connection)
        return connection->isConnected();

    return false;
}

template <typename T>
TsQueue<OwnedMessage<T>>& TcpClient<T>::incoming() {
    return qMsgIn;
}
template <typename T>
void TcpClient<T>::send(const Message<T>& msg) const {
    if (isConnected()) {
        connection->send(msg);
    }
}

template class TcpClient<GameMsgTypes>;
