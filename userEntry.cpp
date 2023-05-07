#include "userEntry.h"

userEntry::userEntry() {
}

userEntry::userEntry(sql::SQLString u, sql::SQLString e, sql::SQLString p) {
    username = u;
    email = e;
    password = p;
  
}

string userEntry::text() {
	string result;
	result += username + " ";
	result += email + " ";
	result += password;
	return result;

}

string userEntry::json() {
	string result;
	result += "\"username\":\"" + username + "\",";
	result += "\"email\":\"" + email + "\",";
	result += "\"password\":\"" + password + "\"}";
	return result;

}

