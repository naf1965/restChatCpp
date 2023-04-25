#include <mariadb/conncpp.hpp>
#include <string>
using namespace std;

#ifndef USERENTRY_H
#define USERENTRY_H

class userEntry {
public:
     userEntry();
     userEntry(sql::SQLString n, sql::SQLString e, sql::SQLString p, sql::SQLString I);
     string text();
     string json();
     
     string name;
     string ID;
     string email;
     string password;

private:

};

#endif /* USERENTRY_H */

