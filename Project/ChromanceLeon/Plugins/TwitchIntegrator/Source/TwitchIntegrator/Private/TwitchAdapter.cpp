// Fill out your copyright notice in the Description page of Project Settings.

#include "TwitchAdapter.h"

using namespace std;


TwitchAdapter::TwitchAdapter(std::string token, std::string username, std::string channel)
{
	_token = token;
	_username = username;
	_channel = channel;
	twitchConnected = false;
	twitchConnectionChanged = false;
	whisperReceived = false;
}

void TwitchAdapter::sendMessage(const FString message_to_send)
{
	string message_val = "PRIVMSG #" + _channel + " :";
	message_val += TCHAR_TO_ANSI(*message_to_send);
	message_val += "\r\n";
	_serverAdapter.sendServerMessageWithNoResponse(message_val);
	commandPrefix = "!";
}

TwitchAdapter::TwitchAdapter()
{
	_token = "";
	_username = "";
	_channel = "";
	commandPrefix = "";
	twitchConnected = false;
	twitchConnectionChanged = false;
}


std::string TwitchAdapter::sendConnectionInfo()
{
	int num = 0;
	ULONG size = TwitchAdapter::TEST_CONNECTION_BUFFER_SIZE;
	std::string aouth = "PASS oauth:" + _token + "\r\n";
	std::string nick = "NICK " + _username + "\r\n";
	std::string joinChannel = "JOIN #" + _channel + "\r\n";
	std::string tags = "CAP REQ :twitch.tv/tags\r\n";
	std::string commands = "CAP REQ :twitch.tv/tags twitch.tv/commands\r\n";

	_serverAdapter.sendServerMessageWithNoResponse(aouth);
	_serverAdapter.sendServerMessageWithNoResponse(nick);
	_serverAdapter.sendServerMessageWithNoResponse(tags);
	_serverAdapter.sendServerMessageWithNoResponse(commands);
	string response = _serverAdapter.sendServerMessageWithResponse(joinChannel);
	return response;
}

void TwitchAdapter::validateConnection() {
	Sleep(5000);
	if (!twitchConnected && !_serverAdapter.checkInternetConnectionHealth()) {
		toggleTwitchConnection(false);
	}
	if (twitchConnected) {
		while (_sessionIsActive) {
			Sleep(1000);
			if (!_serverAdapter.checkInternetConnectionHealth()) {
				toggleTwitchConnection(false);
				_sessionIsActive = false;
			}
		}
	}
}

void TwitchAdapter::ConnectionHealthCheck()
{

}

std::string TwitchAdapter::connect()
{
	std::string serverConnectionErrorMsg = "";
	_sessionIsActive = true;
	channelReference = UTF8_TO_TCHAR(("#" + _channel).c_str());
	serverConnectionErrorMsg = _serverAdapter.establishServerConnection(TWITCH_SERVER_ADDRESS, TWITCH_SERVER_PORT);


	if (!serverConnectionErrorMsg.empty()) {
		toggleTwitchConnection(false);
		return serverConnectionErrorMsg;
	}
	std::string response = sendConnectionInfo();
	if (_message.Contains("NOTICE"))
	{
		toggleTwitchConnection(false);
		_sessionIsActive = false;
	}
	else {
		_chatListeningThread = std::unique_ptr<std::thread>(new std::thread(&TwitchAdapter::listenToChat, this));
		_chatListeningThread->detach();
		std::unique_ptr<std::thread> _connectionValidationThread = std::unique_ptr<std::thread>(new std::thread(&TwitchAdapter::validateConnection, this));
		_connectionValidationThread->detach();
	}

	return "";
}
void TwitchAdapter::addCommand(FString command)
{
	_commands.push_back(command);
}
void TwitchAdapter::removeCommand(FString command)
{
	int commandIndex = -1;
	for (size_t i = 0; i < _commands.size(); i++)
	{
		if (command.Compare(_commands.at(i)) == 0) {
			commandIndex = i;
			break;
		}
	}

	if (commandIndex != -1) {
		_commands.erase(_commands.begin() + commandIndex);
	}
}


void TwitchAdapter::cleanUp()
{
	_sessionIsActive = false;
	twitchConnected = false;
	if (_chatListeningThread != NULL && _chatListeningThread->joinable()) {
		_chatListeningThread->join();
	}
	_serverAdapter.closeSocket();
	_serverAdapter.cleanupSocket();
	_commands.clear();


}
void TwitchAdapter::listenToChat()
{
	std::string msg = "";
	std::string response = TWITCH_PING_RESPONSE_MESSAGE;
	int num = 0;
	while (_sessionIsActive) {
		num = _serverAdapter.receiveMessage(msg);
		if (num > 0 && _sessionIsActive) {
			if (msg == TWITCH_PING_MESSAGE) {

				_serverAdapter.sendServerMessageWithNoResponse(response);
			}
			else {
				parseMessage(msg);
				_message = msg.c_str();
				if (!twitchConnected && _message.Contains(channelReference) && _serverAdapter.checkInternetConnectionHealth())
				{
					toggleTwitchConnection(true);
				}
				else if (_message.StartsWith("@badge"))
				{
					_message = msg.c_str();
					messageWaiting = true;
				}
				else if (_message.Contains("NOTICE"))
				{
					toggleTwitchConnection(false);
					_sessionIsActive = false;
					break;
				}
			}
		}
	}
	if (twitchConnected) {
		toggleTwitchConnection(false);
	}
	_serverAdapter.closeSocket();
}
void TwitchAdapter::parseMessage(const std::string& msg)
{
	/*String stream to convert values for the _data fields*/
	stringstream ssstream;
	int value = -1;
	smatch sm = smatch();
	FString group = "";
	string value_temp = "";
	bool colorFound = false;
	bool bitsFound = false;
	int red = 0;
	int green = 0;
	int blue = 0;
	string msg_temp = msg;
	FChatMessageField field_temp;
	_data = FChatMessageData();
	while (regex_search(msg_temp, sm, _regex_val)) {
		group = sm[0].str().c_str();
		if (group.Contains(";bits") && !bitsFound) {
			_cheerReceived = true;
			_data.bits_sent = true;
			value_temp = sm[2].str();
			ssstream << value_temp;
			ssstream >> _data.number_of_bits;
			ssstream.str("");
			ssstream.clear();
		}
		else if (group.Contains("subscriber")) {
			value_temp = sm[2].str();
			ssstream << value_temp;
			ssstream >> value;
			_data.sender_is_subbed = value > 0;
			ssstream.str("");
			ssstream.clear();
		}
		else if (group.Contains("display-name")) {
			value_temp = sm[2].str();
			_data.sender_username = value_temp.c_str();

		}
		else if (group.Contains("color")) {
			colorFound = true;
			value_temp = sm[2].str();
			vector<string> str_values(3);
			str_values[0] = value_temp.substr(1, 2);
			str_values[1] = value_temp.substr(3, 2);
			str_values[2] = value_temp.substr(5, 2);

			ssstream << hex << str_values[0];
			ssstream >> red;
			ssstream.str("");
			ssstream.clear();
			ssstream << hex << str_values[1];
			ssstream >> green;
			ssstream.str("");
			ssstream.clear();
			ssstream << hex << str_values[2];
			ssstream >> blue;
			ssstream.str("");
			ssstream.clear();

			FColor color(red, green, blue, TwitchAdapter::ALPHA_CHAT_COLOR);
			_data.sender_username_color_byte = color;
			_data.sender_username_color = FLinearColor(color);
		}
		else if (group.Contains("mod")) {
			value_temp = sm[2].str();
			ssstream << value_temp;
			ssstream >> value;
			_data.isModerator = value > 0;
			ssstream.str("");
			ssstream.clear();
		}
		else if (group.Contains("badges")) {
			_data.isVIP = group.Contains("VIP");
			ssstream.str("");
			ssstream.clear();
		}
		else {
			value_temp = sm[1].str();
			field_temp.field_name = value_temp.c_str();
			value_temp = sm[2].str();
			field_temp.field_value = value_temp.c_str();
			_data.other_Data.Add(field_temp);
		}
		msg_temp = sm.suffix().str();

	}
	if (_data.sender_username.IsEmpty()) {
		regex_search(msg, sm, _regex_val_username_fallback);
		value_temp = sm[1].str();
		_data.sender_username = UTF8_TO_TCHAR(value_temp.c_str());
	}
	if (regex_search(msg, sm, _regex_val_message) && sm[1].str().length() > 0) {
		_data.message = UTF8_TO_TCHAR(sm[1].str().c_str());
		_data.messsageText = FText::FromString(_data.message);
		_data.isWhisper = false;
		whisperReceived = false;
	}
	else if (regex_search(msg, sm, _regex_val_message_whisper)) {
		_data.message = UTF8_TO_TCHAR(sm[1].str().c_str());
		_data.messsageText = FText::FromString(_data.message);
		_data.isWhisper = true;
		whisperReceived = true;
	}
	for (size_t i = 0; i < _commands.size(); i++)
	{
		if (_data.message.Contains(UTF8_TO_TCHAR(commandPrefix.c_str()) + _commands.at(i))) {
			_data.commands_entered.Add(_commands.at(i));
			_data.containsCommands = true;
			commandEntered = true;
		}
	}
	if (!colorFound) {
		srand(time(NULL));
		red = rand() % TwitchAdapter::BYTE_COLOR_MAX_VALUE;
		green = rand() % TwitchAdapter::BYTE_COLOR_MAX_VALUE;
		blue = rand() % TwitchAdapter::BYTE_COLOR_MAX_VALUE;
		FColor color(red, green, blue, TwitchAdapter::ALPHA_CHAT_COLOR);
		_data.sender_username_color_byte = color;
		_data.sender_username_color = FLinearColor(color);

	}
	else {
		_data.colorIsAccurate = true;
	}
}

void TwitchAdapter::sendWhisper(const FString message_to_send, const FString receiver)
{
	this->sendMessage("/w " + receiver + " " + message_to_send);
}

void TwitchAdapter::toggleTwitchConnection(const bool state)
{
	twitchConnectionChanged = true;
	twitchConnected = state;
}
