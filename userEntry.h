#ifndef CONTACTENTRY_H
#define CONTACTENTRY_H

#include <string>
#include <mariadb/conncpp.hpp>

using std::string;
using sql::SQLString;

class contactEntry {
public:
    contactEntry();
    contactEntry(SQLString u, SQLString e, SQLString p);
    string text();
    string json();

    SQLString username;
    SQLString email;
    SQLString password;
};

#endif
