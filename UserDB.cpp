#include <vector>
#include <iostream>
#include <mariadb/conncpp.hpp>
#include "UserDB.h"
#include "userEntry.h"



UserDB::UserDB() {
  	// Instantiate Driver
  	driver = sql::mariadb::get_driver_instance();
  	
 	// Configure Connection
  	// The URL or TCP connection string format is
  	// ``jdbc:mariadb://host:port/database``.
  	sql::SQLString url(db_url);

    // Use a properties map for the other connection options
  	sql::Properties my_properties({{"user", user}, {"password",pass}});
  	// Save properties in object
  	properties = my_properties;

    // Establish Connection
  	std::unique_ptr<sql::Connection> my_conn(driver->connect(db_url, properties));
    
    // Check success
    if (!my_conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
   	
   	// Save connection in object
   	conn = std::move(my_conn);
   	
}

vector<userEntry> UserDB::find(string search) {

	vector<userEntry> list;
    
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery(
			"SELECT * FROM userInfo WHERE Username like '%"+search+"%' OR "+
    		 + "Password like '%"+search+"%' OR " +
    		 + "Email like '%"+search+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	contactEntry entry(res->getString("Username"),res->getString("Password"),
		res->getString("Email"),res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;

}

vector<userEntry> UserDB::findByEmail(string email) {
	vector<userEntry> list;
	
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM userInfo WHERE Email like '%"+email+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	userEntry entry(res->getString("Username"),res->getString("Email"),
			res->getString("Password"),res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;

}

void UserDB::addEntry(string username,string email,string password){

	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	stmnt->executeQuery("INSERT INTO userInfo(Username,Email,Password) VALUES ('"+username+"','"+email+"','"+password+"')");
}

userEntry UserDB::fetchEntry(string id){

	contactEntry entry;	
	
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM userInfo WHERE ID = '"+id+"'");
    
    // Get first entry
    if (res->next()) {
    	entry = userEntry(res->getString("Username"),res->getString("Email"),
			res->getString("Password"),res->getString("ID"));
    }
    return entry;
}
