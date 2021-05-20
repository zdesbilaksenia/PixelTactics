// #include <boost/asio.hpp>

#include "GameTcpClient.h"
#include "GameMsgTypes.h"


int main(int argc, char* argv[])
{
    GameTcpClient c;
    c.connect("10.147.17.33");
    // c.getHeroesStats();
    bool quit = false;

    // boost::asio::io_context io;

    /*boost::asio::steady_timer timer(io, std::chrono::steady_clock::now() + std::chrono::seconds(2));
    timer.async_wait(
            [&](const boost::system::error_code& ec) {
                if (ec) {
                    std::cout << "Timer Error!\n";
                } else {
                    std::cout << "Trying to get stats\n";
                    c.getHeroesStats();
                }
            });*/

    while (!quit) {
        char choice = 0;

        std::cout << "0: Join Lobby\n1: Make Lobby\n2: Message All\n3: Get Heroes Stats\n4: Quit\n";
        std::cin >> choice;

        switch (choice) {
            case '0': {
                uint16_t lobbyID = 0;
                std::cout << "Type lobby ID: ";
                std::cin >> lobbyID;
                c.joinLobby(lobbyID);
                std::cout << "Trying to join lobby#" << lobbyID << "\n";
                break;
            }
            case '1':
                c.makeLobby();
                std::cout << "Trying to make a lobby\n";
                break;
            case '2':
                c.messageAll();
                break;
            case '3':
                c.getHeroesStats();
                while (c.incoming().empty()) {
                }
                break;
            case '4':
                quit = true;
                break;
            default:
                break;
        }
        if (c.isConnected()) {
            if (!c.incoming().empty()) {
                auto msg = c.incoming().popFront().msg;
                switch (msg.header.id) {
                    case GameMsgTypes::HeroesStats:
                        std::cout << "Got stats:\n";
                        uint8_t stats[36];
                        msg >> stats;
                        for (int i = 0; i < 36; ++i) {
                            std::cout << int(stats[i]) << " ";
                        }
                        break;
                    case GameMsgTypes::ServerAccept:
                        std::cout << "Server Accepted Connection\n";
                        break;
                    case GameMsgTypes::ServerPing:
                        int inc;
                        msg >> inc;
                        std::cout << inc << " times" << std::endl;
                        break;
                    case GameMsgTypes::ServerMessage:
                        uint32_t uid;
                        msg >> uid;
                        std::cout << "Hello from [" << uid << "]\n";
                        break;
                    case GameMsgTypes::LobbyWaitingForPlayer:
                        std::cout << "Waiting for player...\n";
                        break;
                    case GameMsgTypes::LobbyGameStart:
                        std::cout << "GameStarted!!!\n";
                    default:
                        break;
                }
            }
        } else {
            std::cout << "Server Down\n";
            quit = true;
        }
    }

    return 0;
}
