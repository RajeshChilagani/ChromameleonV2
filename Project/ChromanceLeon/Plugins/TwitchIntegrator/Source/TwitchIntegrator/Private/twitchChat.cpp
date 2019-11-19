/*

Copyright (c) 2017-2018 Louis Contant louiscontant@hotmail.com



This file is part of the project Twitch Integrator

Twitch Integrator can not be copied and/or distributed without the express
persmission of Louis Contant


*/

#include "twitchChat.h"

using namespace std;

// Sets default values
AtwitchChat::AtwitchChat()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_twitchAdapter = TwitchAdapter();
	_cheer_received = false;
	_message_waiting = false;
	_connection_happening = true;
	bIsConnected = false;
}



/*
 Handles the connection routine to a twitch chat
*/
void AtwitchChat::sendConnectionInfoToChat()
{
}

/*
Does what it says, sends a message to the designated chat
*/
void AtwitchChat::sendMessage(const FString message_to_send)
{
	_twitchAdapter.sendMessage(message_to_send);
}

void AtwitchChat::sendWhisper(const FString message_to_send, const FString receiver)
{
	_twitchAdapter.sendWhisper(message_to_send, receiver);
}

void AtwitchChat::setUsername(const FString username_param)
{
	
	usernameInitialized = true;
	_username_str = TCHAR_TO_ANSI(*username_param);
}

void AtwitchChat::setToken(const FString token_param)
{
	tokenInitialized = true;
	_token_str = TCHAR_TO_ANSI(*token_param);
}

void AtwitchChat::setChannel(const FString channel_param)
{
	channelInitialized = true;
	_channel_str = TCHAR_TO_ANSI(*channel_param);
}

void AtwitchChat::addCommand(const FString command)
{
	_twitchAdapter.addCommand(command);
}

void AtwitchChat::removeCommand(const FString command)
{
	_twitchAdapter.removeCommand(command);
}


void AtwitchChat::disconnectFromTwitch()
{
	_twitchAdapter.cleanUp();
}
void AtwitchChat::setCommandPrefix(const FString command_prefix)
{
	commandPrefix = command_prefix;
	_command_prefix_str = TCHAR_TO_ANSI(*command_prefix);
	_twitchAdapter.commandPrefix = _command_prefix_str;
}
/**
A loop that will continusly listen to the designated chat and fill the various info for the events
These events will be triggered in the Click function

**/
void AtwitchChat::listenChat() {
	
}


/**
* Estblishes the connection to the designated Twitch chat
**/
void AtwitchChat::connectToTwitch() {

	if (tokenInitialized && channelInitialized && usernameInitialized && !_twitchAdapter.twitchConnected) {
		_twitchAdapter = TwitchAdapter(_token_str, _username_str, _channel_str);
	    _twitchAdapter.commandPrefix = _command_prefix_str;
		std::string error  = _twitchAdapter.connect();
		if (!error.empty() && bEnableDebugMessages) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, UTF8_TO_TCHAR(error.c_str()));
		}
	}
	

}









// Called when the game starts or when spawned
void AtwitchChat::BeginPlay()
{
	AActor::BeginPlay();
	
	tokenInitialized = false;
	channelInitialized = false;
	usernameInitialized = false;
	_actorEndingPlay = false;
	_token_str = "";
	_username_str = "";
	_channel_str = "";
	if (TCHAR_TO_ANSI(*token))
	{
		_token_str = TCHAR_TO_ANSI(*token);
		tokenInitialized = !_token_str.empty();
	}
	if (TCHAR_TO_ANSI(*channel))
	{
		_channel_str = TCHAR_TO_ANSI(*channel);
		channelInitialized = !_channel_str.empty();
	}
	if (TCHAR_TO_ANSI(*username)) {
		_username_str = TCHAR_TO_ANSI(*username);
		usernameInitialized = !_username_str.empty();

	}
	
	if (TCHAR_TO_ANSI(*commandPrefix)) {
		_command_prefix_str = TCHAR_TO_ANSI(*commandPrefix);
	}
	else {
		_command_prefix_str = "!";
	}

	if (tokenInitialized && channelInitialized && usernameInitialized) {
		
		connectToTwitch();

	}

}
/*
Using this to clean up behind myself
*/
void AtwitchChat::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AActor::EndPlay(EndPlayReason);


	_twitchAdapter.cleanUp();
}

// Called every frame
void AtwitchChat::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
	/*message waiting is modified in listenChat() if there's a message that's received from the twitch API*/
	if (_twitchAdapter.messageWaiting) {
		OnChatMessage.Broadcast(_twitchAdapter._message, _twitchAdapter._data);
		_twitchAdapter.messageWaiting = false;
	}
	/*cheerReceived is modified in listenChat() if there's a cheer in a message*/
	if (_twitchAdapter._cheerReceived) {
		onCheer.Broadcast(_twitchAdapter._data.sender_username, _twitchAdapter._data.message, _twitchAdapter._data.number_of_bits);
		_twitchAdapter._cheerReceived = false;
	}
	if (_twitchAdapter.twitchConnectionChanged) {
		bIsConnected = _twitchAdapter.twitchConnected;
		onConnectionChanged.Broadcast(_twitchAdapter.twitchConnected);
		_twitchAdapter.twitchConnectionChanged = false;
	}

	if (_twitchAdapter.commandEntered) {
		onChatCommandEntered.Broadcast(_twitchAdapter._message, _twitchAdapter._data);
		_twitchAdapter.commandEntered = false;
	}

	if (_twitchAdapter.whisperReceived) {
		onWhisper.Broadcast(_twitchAdapter._message, _twitchAdapter._data);
		_twitchAdapter.whisperReceived = false;
	}

}

