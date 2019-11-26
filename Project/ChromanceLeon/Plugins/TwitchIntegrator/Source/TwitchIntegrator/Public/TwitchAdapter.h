// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include <thread>
#include <regex>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Windows/AllowWindowsPlatformTypes.h"
#include "ServerAdapter.h"
#include "TwitchStructs.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include <regex>


/**
 * 
 */
class TWITCHINTEGRATOR_API TwitchAdapter
{
public:
	TwitchAdapter();
	TwitchAdapter(std::string token, std::string username, std::string channel);

	void sendMessage(const FString message_to_send);

	void sendWhisper(const FString message_to_send, const FString receiver);

	bool messageWaiting;

	bool _cheerReceived;

	bool commandEntered;

	bool twitchConnected;

	bool twitchConnectionChanged;

	bool whisperReceived;

	/*Raw message sent by the Twitch API*/
	FString _message;

	FChatMessageData _data;

	std::string commandPrefix;

	std::string connect();

	void addCommand(FString command);

	void removeCommand(FString command);

	void cleanUp();

	/*The alpha value of the color of */
	static const int ALPHA_CHAT_COLOR = 255;

	/*The upper limit of each channel value to create a color*/
	static const int BYTE_COLOR_MAX_VALUE = BYTE_MAX + 1;
	
	bool _sessionIsActive;

private:
	std::string _token;
	
	std::string _username;
	
	std::string _channel;
	
	std::vector<FString> _commands;

	/*The number of the port of the irc twitch server*/
	const char * TWITCH_SERVER_PORT = static_cast<const char *>("6667");

	/*Address used to connect to the twitch IRC server*/
	const char * TWITCH_SERVER_ADDRESS = static_cast<const char *>("irc.chat.twitch.tv");

	/*The size of the buffer used during the connection to chat*/
	static const int32 TEST_CONNECTION_BUFFER_SIZE = 2048;

	/*The string sent by twitch when it wants to check if the connection is still active*/
	const char * TWITCH_PING_MESSAGE = static_cast<const char *>("PING :tmi.twitch.tv\r\n");

	/*The string sent back to twitch to tell it that the connection is active*/
	const char * TWITCH_PING_RESPONSE_MESSAGE = static_cast<const char *>("PONG :tmi.twitch.tv\r\n");

	const char * TWITCH_MODERATOR_BADGE_VALUE = "1";

	
	FString channelReference;

	/*Regex pattern to capture all metadata from the chat message*/
	 char * _regex_str = "[@;]([a-z\\-]+)+=([A-z0-9\\-#/,]+)+(?=.* :)";
	/*Regex pattern to capture the actual chat message without metadata*/
	char * _regex_str_message = "(?:#\\w+) :(.*)";
	/*Regex pattern to capture the username in case the twitch API doesn't put (it happened in some weird cases)*/
	char *  _regex_str_username_fallback = "^@.*:(\\w+)!.*(?:#\\w+ :)";
	/*Regex pattern to capture the actual message of a whisper*/
	char * _regex_str_whisper_message = "(?:WHISPER \\w+) :(.*)";
	/*Regex to capture all metadata from the chat message*/
	std::regex  _regex_val = std::regex(static_cast<const std::string>("[@;]([a-z\\-]+)=([\\w\\-#/,]+)(?=.* :)"));
	/*regex to capture the acutal chat message without metatdata*/
	std::regex _regex_val_message = std::regex(static_cast<const std::string>(_regex_str_message));
	/**Regex pattern to capture the username in case the twitch API doesn't put (it happened in some weird cases)*/
	std::regex _regex_val_username_fallback = std::regex(static_cast<const std::string>(_regex_str_username_fallback));
	/*Regex pattern to capture the actual message when the message is a whisper*/
	std::regex _regex_val_message_whisper = std::regex(static_cast<const std::string>(_regex_str_whisper_message));

	ServerAdapter _serverAdapter;



	std::unique_ptr<std::thread> _chatListeningThread;

	std::unique_ptr<std::thread> _connectionValidationThread;

	std::string sendConnectionInfo();

	void listenToChat();

	void parseMessage(const std::string &msg);

	void toggleTwitchConnection(const bool state);

	void validateConnection();

	void ConnectionHealthCheck();
};
