#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "GameTcpClient.h"
#include "GameMsgTypes.h"
#include <boost/log/trivial.hpp>
using namespace boost::log;
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Card.h"

void setData(GameTcpClient &client, vector<Unit> &playerUnits, vector<Unit> &opponentUnits, vector<Card> &cards, vector<Texture> &textures);

bool menu(RenderWindow &window,
          Mouse &mouse,
          Event &event,
          GameTcpClient &client);

bool connectToServer(GameTcpClient &client, const string& host);