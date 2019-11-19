/*

Copyright (c) 2017-2018 Louis Contant louiscontant@hotmail.com



This file is part of the project Twitch Integrator

Twitch Integrator can not be copied and/or distributed without the express
persmission of Louis Contant


*/

#pragma once

#include "CoreMinimal.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "ServerAdapter.h"
#include "TwitchStructs.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "TwitchAdapter.h"
#include <iostream>
#include <string>
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "TwitchIntegrator.h"
#include "twitchChat.generated.h"


/*
This actor is the connector to twitch chat
You can capture all messages received via the events 
thrown by this actor and send a message to twitch chat.
*/
UCLASS()
class AtwitchChat : public AActor
{

	GENERATED_BODY()

public:	

	/*Blueprint assignable event for when a chat message is received from the Twitch API*/
	UPROPERTY(BlueprintAssignable, Category = "Twitch Integrator")
	FChatMessageReceived OnChatMessage;
	/*Blueprint assignable event for when a chat message with a cheer is received from the Twitch API*/
	UPROPERTY(BlueprintAssignable, Category = "Twitch Integrator")
	FChatCheerReceived onCheer;
	/*Blueprint assignable event for the connection to twitch either opens or closes*/
	UPROPERTY(BlueprintAssignable, Category = "Twitch Integrator")
	FTwitchConnectionChanged onConnectionChanged;
	/*Blueprint assignable event for when a command has been entered*/
	UPROPERTY(BlueprintAssignable, Category = "Twitch Integrator")
	FChatCommandEntered onChatCommandEntered;
	/*Blueprint assignable event for when a whisper is received*/
	UPROPERTY(BlueprintAssignable, Category = "Twitch Integrator")
	FChatWhipserReceived onWhisper;
	/*Authentication token use to connect to the API*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
	FString token;
	/*The name of the channel with the chat to which I will connect*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
	FString channel;
	/*The username of the owner of the authentication token*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
	FString username;
	/*Use this to enable debbuging messages*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
	bool bEnableDebugMessages;
	/*Prefix to the commands which is the string before the actual command by default it's '!' so a test command would be called with !test*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
	FString commandPrefix;

	UPROPERTY(BlueprintReadOnly, Category = "Twitch Integrator")
		bool bIsConnected;
	// Sets default values for this actor's properties
	AtwitchChat();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	/*Sends a message in the designated chat*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
	void sendMessage(const FString message_to_send);
	/*Sends a whisper to the user with the username specified Be sure to use a token with the additionnal scopes whispers:edit and whispers:read*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void sendWhisper(const FString message_to_send, const FString receiver);

	/*Sets the username for the session (does not work if you're already connected)*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
	void setUsername(const FString username_param);

	/*Sets the token for the session (does not work if you're already connected)*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
	void setToken(const FString token_param);

	/*Sets the channel for the session (does not work if you're already connected)*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
	void setChannel(const FString channel_param);

	/*Add command to the list of commands to listen for*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void addCommand(const FString command);

	/*Removes command from the list of commands to listen for*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void removeCommand(const FString command);

	/*Connects to twitch*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
	void connectToTwitch();

	/*Disconnects from twitch*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
	void disconnectFromTwitch();

	/*Sets the prefix for commands*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
	void setCommandPrefix(const FString command_prefix);

	void listenChat();

private:


	bool tokenInitialized;

	bool channelInitialized;

	bool usernameInitialized;


	ServerAdapter _serverAdapter;

	TwitchAdapter _twitchAdapter;

	/*metadata for the next chat message to be treated by the onChatMessageReceived Event or the onCheer */
	FChatMessageData _data;

	/*std::string version of the username used by the owner of the token*/
	std::string _username_str;

	/*std::string vesrion of the channel to connect to*/
	std::string _channel_str;

	/*std::string version of the token designated by the user in the engine*/
	std::string _token_str;

	/*std::string version of the user defined command prefix*/
	std::string _command_prefix_str;

	/*Pointer to the thread that listens to the designated chat used to be able to join it at the end of the actor's lifecyle */
	std::unique_ptr<std::thread> _listen;



	/*Boolean gate to make sure I don't get authentication messages in the onChatMessage event*/
	bool _connection_happening;
	/*If a message is waiting to be treated by the onChatMessageReceived Event or the onCheer event*/
	bool _message_waiting;
	/*If the actor is being destroyed*/
	bool _actorEndingPlay;
	/*If a cheer has been received in the chat designated*/
	bool _cheer_received;
	/*Raw message sent by the Twitch API*/
	FString _message;

	std::vector<FString> _commands;

	
	void sendConnectionInfoToChat();


	
};
