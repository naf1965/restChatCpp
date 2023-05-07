#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <mariadb++/account.hpp>
#include <mariadb++/connection.hpp>
#include "userEntry.h"

#ifndef USERDB_H
#define USERDB_H

#define DB_URL "localhost"
#define USER "root"
#define PASS "newpassword"
#define DB_NAME "userInfo"

using namespace std;

class userDB {
public:
    userDB();
    userEntry fetchEntryByUsername(string username);
    bool confirmUser(string user, string pass);
    void addEntry(string username,string email,string password);
    bool validEmail(string email);
    bool validName(string user);
    vector<string> users();
private:
    const string db_url = DB_URL;
    const string user = USER;
    const string pass = PASS;
    const string db_name = DB_NAME;
    mariadb::connection_ref conn;
};

#endif /* USERDB_H */
