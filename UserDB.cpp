#include <vector>
#include <iostream>
#include <mariadb/conncpp.hpp>
#include "contactDB.h"
#include "contactEntry.h"



contactDB::contactDB() {
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

vector<contactEntry> contactDB::find(string search) {

	vector<contactEntry> list;
    
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery(
			"SELECT * FROM contacts WHERE Last like '%"+search+"%' OR "+
    		 + "First like '%"+search+"%' OR " +
    		 + "Type like '%"+search+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	contactEntry entry(res->getString("First"),res->getString("Last"),
			res->getString("Phone"),res->getString("Type"),
	    	res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;

}

vector<contactEntry> contactDB::findByLast(string last) {

	vector<contactEntry> list;
    
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM contacts WHERE Last like '%"+last+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	contactEntry entry(res->getString("First"),res->getString("Last"),
			res->getString("Phone"),res->getString("Type"),
	    	res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;

}

vector<contactEntry> contactDB::findByFirst(string first) {

	vector<contactEntry> list;
	
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM contacts WHERE First like '%"+first+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	contactEntry entry(res->getString("First"),res->getString("Last"),
			res->getString("Phone"),res->getString("Type"),
	    	res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;
}

vector<contactEntry> contactDB::findByType(string type) {
	vector<contactEntry> list;
	
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM contacts WHERE Type like '%"+type+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	contactEntry entry(res->getString("First"),res->getString("Last"),
			res->getString("Phone"),res->getString("Type"),
	    	res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;

}

void contactDB::addEntry(string first,string last,string phone, string type){

	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	if (type != "Friend" && type != "Family" && type!="Business"){
     	 type="Other";
  	}
  	
  	stmnt->executeQuery("INSERT INTO contacts(First,Last,Phone,Type) VALUES ('"+first+"','"+last+"','"+phone+"','"+type+"')");
}

contactEntry contactDB::fetchEntry(string id){

	contactEntry entry;	
	
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM contacts WHERE ID = '"+id+"'");
    
    // Get first entry
    if (res->next()) {
    	entry = contactEntry(res->getString("First"),res->getString("Last"),
			res->getString("Phone"),res->getString("Type"),
	    	res->getString("ID"));
    }
    return entry;
}

void contactDB::editEntry(string idnum,string first,string last,string phone, string type){
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	if (type != "Friend" && type != "Family" && type!="Business"){
     	 type="Other";
  	}
  	
  	stmnt->executeQuery("UPDATE contacts SET First = '"+first+"', Last ='"+last+"', Phone ='"+phone+"', Type ='"+type+"' WHERE ID='"+idnum+"'");
  	
}


void contactDB::deleteEntry(string idnum){
  // Establish Connection
  std::unique_ptr<sql::Connection>  conn(driver->connect(db_url, properties));
    
  if (!conn) {
   	cerr << "Invalid database connection" << endl;
   	exit (EXIT_FAILURE);
  }

  std::auto_ptr<sql::Statement> stmt(conn->createStatement());

  stmt->execute("DELETE FROM contacts WHERE ID='"+idnum+"'");
}
