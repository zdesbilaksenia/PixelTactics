#include "TcpConnection.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "TsQueue.h"
#include "OwnedMessage.h"
#include "Message.h"
#include "TcpServer.h"
#include "GameMsgTypes.h"


template <typename T>
TcpConnection<T>::TcpConnection(owner parent, boost::asio::io_context& ioContext_, boost::asio::ip::tcp::socket sock_, TsQueue<OwnedMessage<T>>& qMsgIn_)
    : ioContext(ioContext_), sock(std::move(sock_)), qMsgIn(qMsgIn_) {
    ownerType = parent;

    if (ownerType == owner::server) {
        handshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
        handshakeCheck = scramble(handshakeOut);
    } else {
        handshakeOut = 0;
        handshakeIn = 0;
    }
}
template <typename T>
TcpConnection<T>::~TcpConnection() {}

template <typename T>
uint32_t TcpConnection<T>::getID() const {
    return id;
}

template <typename T>
void TcpConnection<T>::connectToClient(TcpServer<T>* server, uint32_t uid) {
    if (ownerType == owner::server) {
        if (sock.is_open()) {
            id = uid;
            // readHeader();
            writeValidation();
            readValidation(server);
        }
    }
}
template <typename T>
void TcpConnection<T>::connectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
    if (ownerType == owner::client) {
        boost::asio::async_connect(sock, endpoints,
                [this](const boost::system::error_code& ec, boost::asio::ip::tcp::endpoint endpoint) {
                    if (!ec) {
                        // readHeader();
                        readValidation();
                    }
                });
    }
}
template <typename T>
void TcpConnection<T>::disconnect() {
    if (isConnected()) {
        boost::asio::post(ioContext, [this]() { sock.close(); });
    }
}
template <typename T>
bool TcpConnection<T>::isConnected() const {
    return sock.is_open();
}

template <typename T>
void TcpConnection<T>::send(const Message<T>& msg) {
    boost::asio::post(ioContext,
                      [this, msg]() {
                          bool writingMsg = !qMsgOut.empty();
                          qMsgOut.pushBack(msg);
                          if (!writingMsg) {
                              writeHeader();
                          }
                      });
}

template <typename T>
void TcpConnection<T>::readHeader() {
    boost::asio::async_read(sock, boost::asio::buffer(&tempMsg.header, sizeof(MessageHeader<T>)),
            [this](const boost::system::error_code& ec, std::size_t length) {
                if (!ec) {
                    if (tempMsg.header.size > 0) {
                        tempMsg.body.resize(tempMsg.header.size);
                        readBody();
                    } else {
                        addToMsgQueue();
                    }
                } else {
                    std::cout << "[" << id << "] Read Header Fail.\n";
                    sock.close();
                }
            });
}

template <typename T>
void TcpConnection<T>::readBody() {
    boost::asio::async_read(sock, boost::asio::buffer(tempMsg.body.data(), tempMsg.body.size()),
            [this](const boost::system::error_code& ec, std::size_t length) {
                if (!ec) {
                    addToMsgQueue();
                } else {
                    std::cout << "[" << id << "] Read Body Fail.\n";
                    sock.close();
                }
            });
}

template <typename T>
void TcpConnection<T>::writeHeader() {
    async_write(sock, boost::asio::buffer(&qMsgOut.front().header, sizeof(MessageHeader<T>)),
            [this](const boost::system::error_code& ec, std::size_t length) {
                if (!ec) {
                    if (qMsgOut.front().body.size() > 0) {
                        writeBody();
                    } else {
                        qMsgOut.popFront();
                        if (!qMsgOut.empty()) {
                            writeHeader();
                        }
                    }
                } else {
                    std::cout << "[" << id << "] Write Header Fail.\n";
                    sock.close();
                }
            });
}

template <typename T>
void TcpConnection<T>::writeBody() {
    async_write(sock, boost::asio::buffer(qMsgOut.front().body.data(), qMsgOut.front().body.size()),
            [this](const boost::system::error_code& ec, std::size_t length) {
                if (!ec) {
                    qMsgOut.popFront();

                    if (!qMsgOut.empty()) {
                        writeHeader();
                    }
                } else {
                    std::cout << "[" << id << "] Write Body Fail.\n";
                    sock.close();
                }
            });
}

template <typename T>
void TcpConnection<T>::addToMsgQueue() {
    if (ownerType == owner::server) {
        qMsgIn.pushBack({ this->shared_from_this(), tempMsg });
    } else {
        qMsgIn.pushBack({ nullptr, tempMsg });
    }

    readHeader();
}

template <typename T>
uint64_t TcpConnection<T>::scramble(uint64_t input) {
    uint64_t output = input ^ 0xDEADBEFFAC0DECFA;
    output = (output & 0xF0F0F0F0F0F0F0F0) >> 4 | (output & 0x0F0F0F0F0F0F0F0F) << 4;
    return output ^ 0xC0132596CAD450FF;
}

template <typename T>
void TcpConnection<T>::writeValidation() {
    boost::asio::async_write(sock, boost::asio::buffer(&handshakeOut, sizeof(uint64_t)),
            [this](const boost::system::error_code& ec, std::size_t length) {
                if (!ec) {
                    if (ownerType == owner::client) {
                        readHeader();
                    }
                } else {
                    sock.close();
                }
            });
}

template <typename T>
void TcpConnection<T>::readValidation(TcpServer<T>* server) {
    boost::asio::async_read(sock, boost::asio::buffer(&handshakeIn, sizeof(uint64_t)),
            [this, server](const boost::system::error_code& ec, std::size_t length) {
                if (!ec) {
                    if (ownerType == owner::server) {
                        if (handshakeIn == handshakeCheck) {
                            std::cout << "Client Validated\n";
                            server->onClientValidated(this->shared_from_this());

                            readHeader();
                        } else {
                            std::cout << "Client Disconnected (Fail Validation)\n";
                            sock.close();
                        }
                    } else {
                        handshakeOut = scramble(handshakeIn);

                        writeValidation();
                    }
                } else {
                    std::cout << "Client Disconnected (Read Validation)\n";
                    std::cout << "New Error: " << ec.message() << std::endl;
                    sock.close();
                }
            });
}

template class TcpConnection<GameMsgTypes>;
