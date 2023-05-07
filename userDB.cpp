#include <iostream>
#include <mariadb/conncpp.hpp>
#include "userDB.h"
#include "userEntry.h"

using std::cerr;
using std::endl;

userDB::userDB() {
    // Instantiate Driver
    driver = sql::mariadb::get_driver_instance();
    
    // Configure Connection
    sql::SQLString url(db_url);

    // Use a properties map for the other connection options
    sql::Properties my_properties({{"user", user}, {"password", pass}});
    // Save properties in object
    properties = my_properties;

    // Establish Connection
    std::unique_ptr<sql::Connection> my_conn(driver->connect(db_url, properties));
    
    // Check success
    if (!my_conn) {
        cerr << "Invalid database connection" << endl;
        exit(EXIT_FAILURE);
    }
    
    // Save connection in object
    conn = std::move(my_conn);
}

bool userDB::validEmail(const string& email) {
    std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT COUNT(*) as count FROM users WHERE email = ?"));
    pstmt->setString(1, email);
    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (res->next()) {
        return res->getInt("count") == 0;
    }

    return false;
}

bool userDB::validName(const string& user) {
    std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT COUNT(*) as count FROM users WHERE username = ?"));
    pstmt->setString(1, user);
    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (res->next()) {
        return res->getInt("count") == 0;
    }

    return false;
}

void userDB::addEntry(const string& username, const string& email, const string& password) {
    if (!conn) {
        cerr << "Invalid database connection" << endl;
        exit(EXIT_FAILURE);
    }

    if (validEmail(email) && validName(username)) {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute("INSERT INTO users (username, email, password) VALUES ('" + username + "','" + email + "','" + password + "')");
    } else {
        cerr << "The email or username is not unique. Please try again with different values." << endl;
    }
}

vector<string> userDB::users() {
    vector<string> userList;

    if (!conn) {
        cerr << "Invalid database connection" << endl;
        exit(EXIT_FAILURE);
    }

    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT username FROM users"));

    while (res->next()) {
        userList.push_back(res->getString("username"));
    }

    return userList;
}
