#pragma once
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <vector>

struct User
{
	int Id;
	std::string ip;
	SOCKET userSocket = INVALID_SOCKET;
	std::string name;
	bool joined = false;
};

class ServerFrame;

class Server
{
public:
	Server();
	std::string Start();
	void Listen();
	void AssignFrame(ServerFrame* serverFrame);
	void ProcessUser(User user);
	void CheckDisconnect();
	void ReceiveAndSend();
	std::string Stop();
private:
	bool online = false;
	ServerFrame* serverFrame;
	SOCKET serversocket;
	WORD wVersionNeeded;
	WSADATA wsaData;
	int wsaerr;
	sockaddr_in service;
	
	int idsGiven;
	std::vector<User> userList;
	std::thread listenThread;
	std::thread disconnectThread;
	std::thread receiveAndSend;

	char receiveBuffer;
	int byteAmount;
};
