#include <thread>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "GameTcpClient.h"
#include "GameMsgTypes.h"


int main(int argc, char* argv[])
{
    GameTcpClient c;
    c.connect("10.147.17.33");
    c.incoming().wait();
    c.incoming().popFront();

    // c.getHeroesStats();
    bool quit = false;

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
                        std::cout << "Hello from [" << uid << "]\n"; break;
                    case GameMsgTypes::LobbyWaitingForPlayer:
                        std::cout << "Waiting for player...\n";
                        break;
                    case GameMsgTypes::LobbyGameStart:
                        std::cout << "GameStarted!!!\n";
                        quit = true;
                        break;
                    case GameMsgTypes::ErrorMessage: { // TODO errors not working yet!!
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
        std::cout << "GAME LOBBY))\n1: Greet ur opponent\n2: Choose Leader\n";
        char choice = 0;
        std::cin >> choice;
        switch (choice) {
            case '1': {
                Message<GameMsgTypes> msg(GameMsgTypes::GameHelloToPlayer);
                c.send(msg);
                break;
            }
            case '2': {
                int choice = 0;
                std::cin >> choice;
                Message<GameMsgTypes> msg(GameMsgTypes::GameLeaderChoice);
                msg << choice;
                c.send(msg);
                break;
            }
        }

        if (c.isConnected()) {
            if (!c.incoming().empty()) {
                auto msg = c.incoming().popFront().msg;
                switch (msg.header.id) {
                    case GameMsgTypes::GameHelloToPlayer:
                        std::cout << "Hello from ur opponent!\n";
                        break;
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
