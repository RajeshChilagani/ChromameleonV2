// Fill out your copyright notice in the Description page of Project Settings.

#include "TwitchActorComponent.h"


// Sets default values for this component's properties
UTwitchActorComponent::UTwitchActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	_twitchAdapter = TwitchAdapter();
	bIsConnected = false;
	// ...
}


// Called when the game starts
void UTwitchActorComponent::BeginPlay()
{
	Super::BeginPlay();

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
	// ...

}


// Called every frame
void UTwitchActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
	/*If the twitch connection changed*/
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
	// ...
}

void UTwitchActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (_twitchAdapter._sessionIsActive) {
		_twitchAdapter.cleanUp();
	}

}

void UTwitchActorComponent::OnComponentDestroyed(const bool bDestroyingHierarchy)
{
	if (bDestroyingHierarchy && _twitchAdapter._sessionIsActive) {
		_twitchAdapter.cleanUp();
	}

}

void UTwitchActorComponent::sendMessage(const FString message_to_send)
{
	_twitchAdapter.sendMessage(message_to_send);
}

void UTwitchActorComponent::setUsername(const FString username_param)
{
	usernameInitialized = true;
	_username_str = TCHAR_TO_ANSI(*username_param);
}

void UTwitchActorComponent::setToken(const FString token_param)
{
	tokenInitialized = true;
	_token_str = TCHAR_TO_ANSI(*token_param);
}

void UTwitchActorComponent::setChannel(const FString channel_param)
{
	channelInitialized = true;
	_channel_str = TCHAR_TO_ANSI(*channel_param);
}

void UTwitchActorComponent::setCommandPrefix(const FString command_prefix)
{
	commandPrefix = command_prefix;
	_command_prefix_str = TCHAR_TO_ANSI(*command_prefix);
	_twitchAdapter.commandPrefix = _command_prefix_str;
}

void UTwitchActorComponent::addCommand(const FString command)
{
	_twitchAdapter.addCommand(command);
}

void UTwitchActorComponent::removeCommand(const FString command)
{
	_twitchAdapter.removeCommand(command);
}

void UTwitchActorComponent::sendWhisper(const FString message, const FString receiver)
{
	_twitchAdapter.sendWhisper(message, receiver);
}

void UTwitchActorComponent::connectToTwitch()
{
	if (tokenInitialized && channelInitialized && usernameInitialized && !_twitchAdapter.twitchConnected) {
		_twitchAdapter = TwitchAdapter(_token_str, _username_str, _channel_str);
		_twitchAdapter.commandPrefix = _command_prefix_str;
		std::string error = _twitchAdapter.connect();
		if (!error.empty() && bEnableDebugMessages) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, error.c_str());
		}
	}
}

void UTwitchActorComponent::disconnectFromTwitch()
{
	_twitchAdapter.cleanUp();
}
