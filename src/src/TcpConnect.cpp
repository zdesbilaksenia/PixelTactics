#include <iostream>

#include "TcpConnection.h"

template <typename T>
class TcpConnection : public boost::enable_shared_from_this<TcpConnection<T>> {
public:
    enum class owner {
        server,
        client
    };
    // TcpConnection(owner parent, boost::asio::io_context& ioContext_, boost::asio::ip::tcp::socket sock_, TsQueue<OwnedMessage<T>>& qMsgIn_)
    // : ioContext(ioContext_), sock(std::move(sock_)), qMsgIn(qMsgIn_) {
    TcpConnection(owner parent, boost::asio::io_context& ioContext_, boost::asio::ip::tcp::socket sock_, TsQueue<OwnedMessage<T>>& qMsgIn_)
    : ioContext(ioContext_), sock(std::move(sock_)), qMsgIn(qMsgIn_) {
        ownerType = parent;
    }
    ~TcpConnection() {}

    uint32_t getID() const {
        return id;
    }

    void connectToClient(uint32_t uid = 0) {
        if (ownerType == owner::server) {
            if (sock.is_open()) {
                id = uid;
                readHeader();
            }
        }
    }
    void connectToServer(boost::asio::ip::tcp::resolver::results_type endpoints) {
        if (ownerType == owner::client) {
            boost::asio::async_connect(sock, endpoints,
                    [this](const boost::system::error_code& ec, boost::asio::ip::tcp::endpoint endpoint) {
                        if (!ec) {
                            readHeader();
                        }
                    });
        }
    }
    bool disconnect() {
        if (isConnected()) {
            boost::asio::post(ioContext, [this]() { sock.close(); });
        }
    }
    bool isConnected() const;

    bool send(const Message<T>& msg) {
        boost::asio::post(ioContext,
                          [this, msg]() {
                              bool writingMsg = !qMsgOut.empty();
                              qMsgOut.pushBack(msg);
                              if (!writingMsg) {
                                  writeHeader();
                              }
                          });
    }
protected:
    boost::asio::ip::tcp::socket sock;
    boost::asio::io_context& ioContext;
    TsQueue<Message<T>> qMsgOut;
    TsQueue<OwnedMessage<T>>& qMsgIn;
    Message<T> tempMsg;

    owner ownerType = owner::server;
    uint32_t id = 0;

private:
    void readHeader() {
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

    void readBody() {
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

    void writeHeader() {
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

    void writeBody() {
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

    void addToMsgQueue() {
        if (ownerType == owner::server) {
            qMsgIn.pushBack({ this->shared_from_this(), tempMsg });
        } else {
            qMsgIn.pushBack({ nullptr, tempMsg });
        }

        readHeader();
    }
};


