#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
/* uncomment for applications that use vectors */
#include <vector>

#include <mariadb/conncpp.hpp>

#include "UserManager.h"

#ifndef REGISTRYSQL_H
#define REGISTRYSQL_H

#define DB_URL "jdbc:mariadb://localhost:3306/userInfo"
#define USER "root"
#define PASS "newpassword"

using namespace std;

class UserDB {
public:
    UserDB();
    vector<UserEntry> find(string search);
    UserEntry fetchEntry(string idnum);
    void addEntry(string username, string email, string password);
    void UserVerification(string username, string email,string password);
    void UserExists(string username, string email);
private:
    const string db_url=DB_URL;
    const string user=USER;
    const string pass=PASS;
    sql::Driver* driver;
    sql::Properties properties;
    std::unique_ptr<sql::Connection> conn;

};

#endif /* REGISTRYSQL_H */
