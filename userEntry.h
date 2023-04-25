#include <mariadb/conncpp.hpp>
#include <string>
using namespace std;

#ifndef CONTACTENTRY_H
#define CONTACTENTRY_H

class contactEntry {
public:
     contactEntry();
     contactEntry(sql::SQLString f, sql::SQLString l, sql::SQLString p, sql::SQLString t,sql::SQLString I,sql::SQLString e,sql::SQLString s);
     string text();
     string json();
     
     string first;
     string last;
     string phone;
     string type;
     string ID;
     string email;
     string social;

private:

};

#endif /* CONTACTENTRY_H */

