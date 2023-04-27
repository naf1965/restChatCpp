//
//  namesAPI.cc - a microservice demo program
//
// James Skon
// Kenyon College, 2022
//

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include "httplib.h"
#include "UserManager.h"
#include "userEntry.h"
#include "userDB.h"



using namespace httplib;
using namespace std;

const int port = 5005;
UserManager userManager;
UserDB userDB;

void addMessage(string username, string message, map<string,vector<string>> &messageMap) {
    /* iterate through users adding message to each */
	cout << "Adding message from user: " << username << " with message: " << message << endl; // Add this line
    string jsonMessage = "{\"user\":\""+username+"\",\"message\":\""+message+"\"}";
    for (auto userMessagePair : messageMap) {
        string recipientUsername = userMessagePair.first;
        messageMap[recipientUsername].push_back(jsonMessage);
    }
	cout << "Message added: " << jsonMessage << endl;
}


void removeUser(string username, map<string, vector<string>> &messageMap) {
    if (messageMap.count(username) > 0) {
        messageMap.erase(username);
        cout << "User " << username << " has left the chat room." << endl;
    }
}

string getMessagesJSON(string username, map<string,vector<string>> &messageMap) {
	/* retrieve json list of messages for this user */
	bool first = true;
	string result = "{\"messages\":[";
	for (string message :  messageMap[username]) {
		if (not first) result += ",";
		result += message;
		first = false;
	}
	result += "]}";
	messageMap[username].clear();
	return result;
}

string getUsersJSON(UserManager &userManager) {
  bool first = true;
  string result = "[";
  for (const auto &user : userManager.getUsers()) {
    if (!first) result += ",";
    result += "\"" + user + "\"";
    first = false;
  }
  result += "]";
  return result;
}

string decodeURIComponent(const string &encoded) {
  string decoded;
  char ch;
  int i, j, ix;
  for (i = 0; i < encoded.length(); i++) {
    ch = encoded.at(i);
    if (ch == '+') {
      decoded += ' ';
    } else if (ch == '%') {
      sscanf(encoded.substr(i + 1, 2).c_str(), "%x", &ix);
      ch = static_cast<char>(ix);
      decoded += ch;
      i += 2;
    } else {
      decoded += ch;
    }
  }
  return decoded;
}




int main(void) {
  Server svr;
	userDB userObject;
  int nextUser=0;
  map<string,vector<string>> messageMap;
	vector<string> emailVector;
  vector<string> passwordVector;
  vector<string> usernameVector;
  bool registered = false;
	
  /* "/" just returnsAPI name */
  svr.Get("/", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("Chat API", "text/plain");
  });
svr.Get(R"(/chat/register/(.*)/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    string username = req.matches[1];
    string email = req.matches[2];
    string password = req.matches[3];
    string error;
	userObject.registerUser(username,email,password);
    bool registrationSuccessful = userManager.registerUser(username, email, password, error);
	

    // Return the appropriate response
    string result;
    if (registrationSuccessful) {
        userDB.addEntry(username, email, password); // Call addEntry function here
        result = "{\"status\":\"success\"}";
    } else {
        result = "{\"status\":\"fail\",\"error\":\"" + error + "\"}";
    }
    res.set_content(result, "text/json");
});

	
	svr.Get(R"(/chat/login/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    string username = req.matches[1];
    string password = req.matches[2];
    string error;
	string result;
	userObject.confirmUser(username,password);
    if (!userManager.userExists(username)) {
        result = "{\"status\":\"fail\",\"error\":\"User not registered\"}";
    } else {
        bool loginSuccessful = userManager.loginUser(username, password, error);

        // Return the appropriate response
        result;
        if (loginSuccessful) {
            result = "{\"status\":\"success\"}";
        } else {
            result = "{\"status\":\"fail\",\"error\":\"" + error + "\"}";
        }
    }

    res.set_content(result, "text/json");
});

  svr.Get(R"(/chat/join/(.*)/(.*))", [&](const Request& req, Response& res) {
  string username = req.matches[1];
  string password = req.matches[2];
  string error;
  string result;

  // Check if user with this name exists
  if (!userManager.userExists(username)) {
    result = "{\"status\":\"fail\",\"error\":\"User not registered\"}";
  } else {
    bool loginSuccessful = userManager.loginUser(username, password, error);

    // Return the appropriate response
    if (loginSuccessful) {
      // Add user to messages map
      vector<string> empty;
      cout << username << " joins" << endl;
      messageMap[username] = empty;
      result = "{\"status\":\"success\",\"user\":\"" + username + "\"}";
    } else {
      result = "{\"status\":\"fail\",\"error\":\"" + error + "\"}";
    }
  }

  res.set_header("Access-Control-Allow-Origin","*");
  res.set_content(result, "text/json");
});



 svr.Get(R"(/chat/send/(.*)/(.*))", [&](const Request& req, Response& res) {
  res.set_header("Access-Control-Allow-Origin", "*");
  string username = decodeURIComponent(req.matches[1]);
  string message = decodeURIComponent(req.matches[2]);
  string result;

  if (!messageMap.count(username)) {
    result = "{\"status\":\"baduser\"}";
  } else {
    addMessage(username, message, messageMap);
    result = "{\"status\":\"success\"}";
  }
  res.set_content(result, "text/json");
});

   svr.Get(R"(/chat/fetch/(.*))", [&](const Request& req, Response& res) {
  string username = req.matches[1];
  res.set_header("Access-Control-Allow-Origin", "*");
  string messagesJSON = getMessagesJSON(username, messageMap);
  string usersJSON = getUsersJSON(userManager);
  string resultJSON = "{\"messages\":" + messagesJSON + ",\"users\":" + usersJSON + "}";
  res.set_content(resultJSON, "text/json");
});
   svr.Get(R"(/chat/leave/(.*))", [&](const Request& req, Response& res) {
  string username = req.matches[1];
  removeUser(username, messageMap);
  res.set_header("Access-Control-Allow-Origin", "*");
  res.set_content("{\"status\":\"success\"}", "text/json");
});


  
  cout << "Server listening on port " << port << endl;
  svr.listen("0.0.0.0", port);

}
