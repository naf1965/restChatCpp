#include "userEntry.h"

userEntry::userEntry() {
}

userEntry::userEntry(sql::SQLString u, sql::SQLString e, sql::SQLString p, sql::SQLString I) {
    username = u;
    email = e;
    password = p;
    ID=I;
  
}

string userEntry::text() {
	string result = ID + ". ";
	result += username + " ";
	result += email + " ";
	result += password;
	return result;

}

string userEntry::json() {
	string result = "{\"ID\":\"" + ID + "\",";
	result += "\"username\":\"" + username + "\",";
	result += "\"email\":\"" + email + "\",";
	result += "\"password\":\"" + password + "\"}";
	return result;

}

