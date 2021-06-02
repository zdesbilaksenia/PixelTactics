#include <thread>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "GameTcpClient.h"
#include "GameMsgTypes.h"

#include "../Game/Card.h"


int main(int argc, char* argv[])
{
    GameTcpClient c;
    c.connect("10.147.17.200");
    c.incoming().wait();
    c.incoming().popFront();

    bool quit = false;

    while (!quit) {
        char choice = 0;

        std::cout << "1: Play\n4: Quit\n";
        std::cin >> choice;

        switch (choice) {
            case '1':
                c.connectToLobby();
                break;
            case '4':
                return 0;
                quit = true;
                break;
            case '5':
                c.waitForMessageOnTime(9);
                break;
            default:
                break;
        }
        if (c.isConnected()) {
            if (!c.incoming().empty()) {
                auto msg = c.incoming().popFront().msg;
                switch (msg.header.id) {
                    case GameMsgTypes::ServerAccept:
                        std::cout << "Server Accepted Connection\n";
                        break;
                    case GameMsgTypes::LobbyWaitingForPlayer:
                        std::cout << "Waiting for player...\n";
                        break;
                    case GameMsgTypes::LobbyGameStart:
                        std::cout << "GameStarted!!!\n";
                        quit = true;
                        break;
                    case GameMsgTypes::ErrorMessage: {
                        std::string err;
                        msg >> err;
                        std::cout << "Error: " << err;
                        break;
                    }
                    default:
                        break;
                }
            } else {
                std::cout << "No message))\n";
            }
        } else {
            std::cout << "Server Down\n";
            quit = true;
        }
    }

    // TIPA GAME
    bool gameRuns = true;
    while (gameRuns) {
        std::cout << "GAME LOBBY))\n";

        if (c.isConnected()) {
            if (!c.incoming().empty()) {
                auto msg = c.incoming().popFront().msg;
                switch (msg.header.id) {
                    case GameMsgTypes::LobbyGameOver: {
                        std::cout << "Game over!\n";
                        gameRuns = false;
                        break;
                    }
                }
            }
        } else {
            std::cout << "Server Down\n";
            gameRuns = false;
        }
    }

    return 0;
}
