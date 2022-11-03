#include "Server.h"
#include <iostream>
#include <thread>
#include <vector>
#include "ServerFrame.h"
#include <inaddr.h>
#include <ws2tcpip.h>
#include <string> 
#pragma warning (disable:4996)


Server::Server()
{
}

std::string Server::Start()
{
	int idsGiven = 0;

	WORD wVersionNeeded = MAKEWORD(2, 2); //Make word finds the appropriate dll to use for networking

	serversocket = INVALID_SOCKET;

	int wsaerr = WSAStartup(wVersionNeeded, &wsaData);

	serversocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (online) return "Server is already online";

	if (wsaerr != 0) return "dll could not be found by program.";
	if (serversocket == INVALID_SOCKET) {
		WSACleanup(); return "Error with initialising socket";
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(25565);

	if (bind(serversocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		closesocket(serversocket);
		WSACleanup();
		return "bind() failed: " + WSAGetLastError();
	}

	if (listen(serversocket, 0) == SOCKET_ERROR)
	{
		closesocket(serversocket);
		WSACleanup();
		return "bind() failed: " + WSAGetLastError();
	};

	unsigned long noBlocking = 1;
	ioctlsocket(serversocket, FIONBIO, &noBlocking);

	online = true;

	listenThread = std::thread([this] {this->Listen(); });
	disconnectThread = std::thread([this] {this->CheckDisconnect(); });
	receiveAndSend = std::thread([this] {this->ReceiveAndSend(); });
	return "-_- it works";

}

void Server::Listen()
{


	while (online == true)
	{

		SOCKET acceptSocket = INVALID_SOCKET;
		sockaddr_in addr = { 0 };
		int addrlen = sizeof(addr);

		acceptSocket = accept(serversocket, (sockaddr*)&addr, &addrlen);

		if (acceptSocket == INVALID_SOCKET)
		{
			continue;
		}
		else //User joined successfully
		{
			User user;
			user.Id = userList.size();
			char nameBuffer[200] = "";
			int byteAmount = recv(acceptSocket, nameBuffer, 200, 0);
			user.name = nameBuffer;
			user.name += " (User " + std::to_string(user.Id) + ")";
			bool exists = false;
			std::string newIP = inet_ntoa(addr.sin_addr);
			user.ip = newIP;
			user.joined = true;
			user.userSocket = acceptSocket;

			for (int i = 0; i < userList.size(); i++)
			{
				User s = userList[i];

				if (s.ip == user.ip)
				{
					user.Id = s.Id;
					user.name = s.name;
					user.joined = true;
					exists = true;
					userList[i] = user;
					ProcessUser(user);
					continue;
				}
			}

			if (exists) continue;

			userList.push_back(user);
			ProcessUser(user);


		}
	}
}

void Server::AssignFrame(ServerFrame* serverFrame)
{
	this->serverFrame = serverFrame;
}


void Server::ProcessUser(User user)
{
	std::string getname = user.name + ": has joined! \n";
	const char* sendbuf = getname.c_str();

	int sendL = strlen(sendbuf);

	for (int i = 0; i < userList.size(); i++)
	{
		send(userList[i].userSocket, sendbuf, sendL, 0);
	}

}

void Server::CheckDisconnect()
{
	while (online == true)
	{

		if (userList.size() == 0)
		{
			continue;
		}

		else
		{
			for (int i = 0; i < userList.size(); i++)
			{
				const char* sendbuf = "";
				int sendL = strlen(sendbuf);

				if (!userList[i].joined) continue;

				int failCheck = send(userList[i].userSocket, sendbuf, sendL, 0);


				if (failCheck == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
				{

					std::string userLeftName = userList[i].name;
					userList[i].joined = false;

					for (int j = 0; j < userList.size(); j++)
					{

						std::string getname = userLeftName + ": has left!";

						const char* sendbuf = getname.c_str();

						int sendL = strlen(sendbuf);

						int r = send(userList[j].userSocket, sendbuf, sendL, 0);

					}
				}
			}
		}
	}

}


void Server::ReceiveAndSend()
{
	while (online == true)
	{
		if (userList.size() == 0)
		{
			continue;
		}

		for (int i = 0; i < userList.size(); i++)
		{
			char receiveBuffer[200] = "";
			int byteAmount = recv(userList[i].userSocket, receiveBuffer, 200, 0);
			
			if (byteAmount > 0)
			{
				for (int j = 0; j < userList.size(); j++)
				{
					std::string getname = userList[i].name;
					std::string message = getname+ ": " + receiveBuffer + "\n";
					int messageL = strlen(message.c_str());
					int sendMessage = send(userList[j].userSocket, message.c_str(), messageL, 0);

				}
			}

		}

	}
}

std::string Server::Stop()
{
	if (!online) {return "Server already stopped";}
	userList.clear();
	listenThread.detach();
	disconnectThread.detach();
	receiveAndSend.detach();
	online = false;
	closesocket(this->serversocket);
	WSACleanup();
	return "Server Stopped";
}