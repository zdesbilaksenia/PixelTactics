#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_context& io_context)
  {
    return pointer(new tcp_connection(io_context));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void read() {
      memset(data, 0, 128);
      socket_.async_read_some(boost::asio::buffer(data, 128),
                              boost::bind(&tcp_connection::handle_read,
                                          shared_from_this(),
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
  }

  void write() {
      message_ = "";
      message_ = data;
      std::cout << "Message to write to client: " << message_ << std::endl;
      socket_.async_write_some(boost::asio::buffer(data, 128),
                               boost::bind(&tcp_connection::handle_write,
                                           shared_from_this(),
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
  }

  void start()
  {
      read();
      //for (;;) {
      // boost::asio::async_read_some(socket_, boost::asio::buffer(data, 128),
      //  boost::bind(&tcp_connection::handle_read, shared_from_this(),
      //    boost::asio::placeholders::error,
      //    boost::asio::placeholders::bytes_transferred));
      //std::cout << "Got data: " << message_.data() << std::endl;
    //message_ = make_daytime_string();

/*    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
      //}
*/  }

private:
  tcp_connection(boost::asio::io_context& io_context) : socket_(io_context) {}

  void handle_write(const boost::system::error_code& ec, size_t bytes_transferred) {
      if (ec) {
          std::cout << ec.message() << std::endl;
          socket_.close();
      }
      read();
  }

  void handle_read(const boost::system::error_code& ec, size_t bytes_transferred) {
      if (ec) {
          std::cout << ec.message() << std::endl;
          socket_.close();
          return;
      }
      std::cout << "Got froom client: "  << data << std::endl;
      write();
  }

  tcp::socket socket_;
  std::string message_;
  char data[128];
};

class Lobby {
public:
    Lobby() : lobby(2), data(""), current_player(0), code(SUCCESS) {}
    void start() {
        current_player = std::rand() % 2;
        while (code) {
            lobby[current_player]->read();
            parse();
            while (code == ERROR) {
                lobby[current_player]->write();
                lobby[current_player]->read();
            }
            lobby[current_player]->write();
            current_plaer = ++current_player % 2;
            lobby[current_player]->write();
        }
    }
private:
    void parse() {
        switch (data[0]) {
            case 'S':
                code = SUCCESS;
                break;
            case 'E':
                code = ERROR;
                break;
            default:
                code = QUIT;
                break;
        }
    }
    enum Code {
        QUIT,
        SUCCESS,
        ERROR
    }
    std::vector<tcp_connection::pointer> lobby;
    std::string data;
    size_t current_player;
    Code code;
}

class tcp_server
{
public:
  tcp_server(boost::asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), 2001)), playersCount(0)
      //acceptor_(io_context, tcp::endpoint(tcp::v4(), 2001)), t(boost::bind(&boost::asio::io_context::run, &io_context_))
  {
    start_accept();

  }

~tcp_server() {
    //t.join();
    }

private:
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
        std::cout << "New connection!\n";
        lobby[playersCount] = new_connection;
      new_connection->start();
      ++playersCount;
      if (playersCount == 2) {
          return;
      }
    }

    start_accept();
  }

  boost::asio::io_context& io_context_;
  tcp::acceptor acceptor_;
  char data[128];
  tcp_connection::pointer lobby[2];
  size_t playersCount;
};

int main()
{
  try
  {
    boost::asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
