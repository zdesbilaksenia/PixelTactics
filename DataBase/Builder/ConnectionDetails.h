
#ifndef PIXELTACTICS_CONNECTIONDETAILS_H
#define PIXELTACTICS_CONNECTIONDETAILS_H

using std::string;

typedef struct ConnectionDetails {
    string server;
    string user;
    string password;
    string dataBase;
} ConnectionDetails;

#endif //PIXELTACTICS_CONNECTIONDETAILS_H
