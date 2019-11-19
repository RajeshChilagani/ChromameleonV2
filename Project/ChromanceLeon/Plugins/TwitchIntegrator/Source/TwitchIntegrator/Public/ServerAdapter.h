#pragma once
#include "CoreMinimal.h"
#include "Windows/MinWindows.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#include "iphlpapi.h"
#include <wininet.h>
#include <iostream>
#include <string>

class ServerAdapter
{
public:
	ServerAdapter();
	~ServerAdapter();



	/*Socket used to talk the twitch API*/
	SOCKET _sock;

	void sendServerMessageWithNoResponse(std::string &message_val);

	std::string sendServerMessageWithResponse(std::string &aouth);

	void closeSocket();

	int receiveMessage(std::string & str);

	void cleanupSocket();

	bool checkInternetConnectionHealth();

	std::string establishServerConnection(const std::string serverAdress,const std::string serverPort);
};

