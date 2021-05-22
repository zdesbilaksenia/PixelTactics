#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "TsQueue.h"
#include "OwnedMessage.h"
#include "Message.h"
// #include "TcpServer.h"

template <typename T>
class TcpServer;

template <typename T>
class TcpConnection : public boost::enable_shared_from_this<TcpConnection<T>> {
public:
     enum class owner {
        server,
        client
    };

   TcpConnection(owner parent, boost::asio::io_context& ioContext_, boost::asio::ip::tcp::socket sock_, TsQueue<OwnedMessage<T>>& qMsgIn_);
    virtual ~TcpConnection();

    uint32_t getID() const;

    void connectToClient(TcpServer<T>* server, uint32_t uid = 0);
    void connectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints);
    void disconnect();
    bool isConnected() const;
    void send(const Message<T>& msg);
protected:
    boost::asio::ip::tcp::socket sock;
    boost::asio::io_context& ioContext;
    TsQueue<Message<T>> qMsgOut;
    TsQueue<OwnedMessage<T>>& qMsgIn;
    Message<T> tempMsg;

    owner ownerType = owner::server;
    uint32_t id = 0;

    uint64_t handshakeOut = 0;
    uint64_t handshakeIn = 0;
    uint64_t handshakeCheck = 0;

private:
    void readHeader();
    void readBody();
    void writeHeader();
    void writeBody();

    void addToMsgQueue();
    uint64_t scramble(uint64_t input);
    void writeValidation();
    void readValidation(TcpServer<T>* server = nullptr);
};
