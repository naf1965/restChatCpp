# MakeFile for restChat
# server using C++ Microservice
# sudo mkdir /var/www/html/restChat
# sudo chown ubuntu /var/www/html/restChat

CXXFLAGS = -Wall -std=c++11

all: PutHTML compile_restChat

PutHTML:
	cp restChat.html /var/www/html/restChat/
	cp restChat.css /var/www/html/restChat/
	cp restChat.js /var/www/html/restChat/
	@echo "Current contents of your HTML directory: "
	@ls -l /var/www/html/restChat/

compile_restChat: restChat

restChat : restChat.cpp httplib.h UserManager.cpp UserManager.h
	$(CXX) -o restChat $(CXXFLAGS) restChat.cpp UserManager.cpp

clean:
	-rm -f restChat *.o


.PHONY: all PutHTML compile_restChat clean
