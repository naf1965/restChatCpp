#include "userEntry.h"

userEntry::userEntry() {
}

contactEntry::contactEntry(sql::SQLString n, sql::SQLString e, sql::SQLString p, sql::SQLString I) {
    name = n;
    email = e;
    password = p;
    ID=I;
  
}

string userEntry::text() {
	string result = ID + ". ";
	result += name + " ";
	result += email + " ";
	result += password;
	return result;

}

string contactEntry::json() {
	string result = "{\"ID\":\"" + ID + "\",";
	result += "\"name\":\"" + name + "\",";
	result += "\"email\":\"" + email + "\",";
	result += "\"password\":\"" + password + "\"}";
	return result;

}

