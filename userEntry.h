#include <mariadb/conncpp.hpp>
#include <string>
using namespace std;

#ifndef USERENTRY_H
#define USERENTRY_H

class userEntry {
public:
     userEntry();
     userEntry(sql::SQLString f, sql::SQLString l, sql::SQLString p, sql::SQLString t,sql::SQLString I,sql::SQLString e,sql::SQLString s);
     string text();
     string json();
     
     string name;
     string ID;
     string email;
     string password;

private:

};

#endif /* USERENTRY_H */

