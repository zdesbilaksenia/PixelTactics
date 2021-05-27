#include <utility>


#ifndef PIXELTACTICS_CONNECTIONDETAILS_H
#define PIXELTACTICS_CONNECTIONDETAILS_H

using std::string;
using std::move;

typedef struct ConnectionDetails {

    string server;
    string user;
    string password;
    string dataBase;

    ConnectionDetails(string _server, string _user, string _password, string _dataBase) : server(move(_server)),
                                                                                          user(move(_user)),
                                                                                          password(move(_password)),
                                                                                          dataBase(move(_dataBase)) {};

    ConnectionDetails(const ConnectionDetails &_details) : server(_details.server), user(_details.user),
                                                           password(_details.password), dataBase(_details.dataBase) {};
} ConnectionDetails;

#endif //PIXELTACTICS_CONNECTIONDETAILS_H
