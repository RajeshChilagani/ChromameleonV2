#include "ServerAdapter.h"
/*The size of the buffer used during the connection to chat*/
const int CONNECTION_BUFFER_SIZE = 3072;

/*
The size of the buffer to receive chat messages.
*/
const int SERVER_ADAPTER_BUFFER_SIZE = 1024;

ServerAdapter::ServerAdapter()
{
}


ServerAdapter::~ServerAdapter()
{
}

void ServerAdapter::closeSocket() {
	closesocket(_sock);
}

int ServerAdapter::receiveMessage(std::string & str)
{
	char buffer[CONNECTION_BUFFER_SIZE];
	int num = 0;
	memset(&buffer, '\0', sizeof(buffer));
	num = recv(_sock, buffer, CONNECTION_BUFFER_SIZE, 0);
	str = buffer;
	return num;
}

void ServerAdapter::cleanupSocket()
{
	WSACleanup();
	closesocket(_sock);
	shutdown(_sock, SD_BOTH);
}
bool ServerAdapter::checkInternetConnectionHealth()
{
	return InternetCheckConnection(L"http://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0);
}
/*
Sends a message to the server the plugin is connected to at the time of the call
*/
std::string ServerAdapter::sendServerMessageWithResponse(std::string &message)
{
	int num = 0;
	char buffer[CONNECTION_BUFFER_SIZE];
	send(_sock, message.c_str(), message.length(), 0);
	memset(&buffer, '\0', sizeof(buffer));
	num = recv(_sock, buffer, CONNECTION_BUFFER_SIZE, 0);
	if (num <= 0) {
		return std::string("");
	}
	return std::string(buffer);
}

void ServerAdapter::sendServerMessageWithNoResponse(std::string &message_val)
{
	send(_sock, message_val.c_str(), message_val.length(), 0);
}

std::string ServerAdapter::establishServerConnection(const std::string serverAdress,const std::string serverPort)
{
	WSADATA WSAData = WSADATA();
	SOCKADDR_IN sin = SOCKADDR_IN();
	std::string msg = "";
	std::string error = "";
	addrinfo hints, *res;
	int iResult = 0;
	bool retflag = true;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock == INVALID_SOCKET) {

		//Error message for when there is an error in the socket creation
		return "Error creating socket" + WSAGetLastError();


	}
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;

	iResult = getaddrinfo(serverAdress.c_str(), serverPort.c_str(), &hints, &res);
	if (iResult != 0) {

		//Error message for when there is an error in the name resolution
		std::wstring ws(gai_strerror(iResult));
		return std::string(ws.begin(), ws.end());
		
	}
	else {
		memset(&sin, 0, sizeof(sin));
		sin = *(struct sockaddr_in *) res->ai_addr;
		iResult = connect(_sock, (SOCKADDR *)&sin, sizeof(sin));
		if (iResult == SOCKET_ERROR) {
			error = "Connection Error­\r\n";
			error += WSAGetLastError() + "\r\n";
			error += sin.sin_addr.s_addr;
			return error;
		}
	}
	retflag = false;
	return error;
}