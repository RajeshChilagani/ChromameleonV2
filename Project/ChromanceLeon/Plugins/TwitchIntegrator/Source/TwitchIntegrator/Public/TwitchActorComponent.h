// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "TwitchStructs.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "TwitchAdapter.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "TwitchActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWITCHINTEGRATOR_API UTwitchActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTwitchActorComponent();

	/*Authentication token use to connect to the API*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FString token;
	/*The name of the channel with the chat to which I will connect*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FString channel;
	/*The username of the owner of the authentication token*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FString username;

	/*Blueprint assignable event for when a chat message is received from the Twitch API*/
	UPROPERTY(BlueprintAssignable, Category = "Twitch Integrator")
		FChatMessageReceived OnChatMessage;
	/*Blueprint assignable event for when a chat message with a cheer is received from the Twitch API*/
	UPROPERTY(BlueprintAssignable, Category = "Twitch Integrator")
		FChatCheerReceived onCheer;
	UPROPERTY(BlueprintAssignable, Category = "Twitch Integrator")
		FTwitchConnectionChanged onConnectionChanged;
	UPROPERTY(BlueprintAssignable, Category = "Twitch Integrator")
		FChatCommandEntered onChatCommandEntered;
	/*Blueprint assignable event for when a whisper is received*/
	UPROPERTY(BlueprintAssignable, Category = "Twitch Integrator")
		FChatWhipserReceived onWhisper;
	UPROPERTY(BlueprintReadOnly, Category = "Twitch Integrator")
		bool bIsConnected;
	/*Prefix to the commands*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FString commandPrefix;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnComponentDestroyed(const bool bDestroyingHierarchy) override;

	/*Sends a message in the designated chat*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void sendMessage(const FString message_to_send);

	/*Sets the username for the session (does not work if you're already connected)*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void setUsername(const FString username_param);

	/*Sets the token for the session (does not work if you're already connected)*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void setToken(const FString token_param);

	/*Sets the channel for the session (does not work if you're already connected)*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void setChannel(const FString channel_param);
	
	/*Sets the channel for the session (does not work if you're already connected)*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void setCommandPrefix(const FString command_prefix);

	/*Connects to twitch*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void connectToTwitch();

	/*Disconnects from twitch*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void disconnectFromTwitch();

	/*Add command to the list of commands to listen for*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void addCommand(const FString command);

	/*Removes command from the list of commands to listen for*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void removeCommand(const FString command);

	/*Use this to enable debbuging messages*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		bool bEnableDebugMessages;

	/*Sends a whisper to the user with the username specified Be sure to use a token with the additionnal scopes whispers:edit and whispers:read*/
	UFUNCTION(BlueprintCallable, Category = "Twitch Integrator")
		void sendWhisper(const FString message, const FString receiver);

private:
	/*std::string version of the username used by the owner of the token*/
	std::string _username_str;

	/*std::string vesrion of the channel to connect to*/
	std::string _channel_str;

	/*std::string version of the token designated by the user in the engine*/
	std::string _token_str;

	/*std::string version of the commands*/
	std::string _command_prefix_str;

	TwitchAdapter _twitchAdapter;

	bool tokenInitialized;

	bool channelInitialized;

	bool usernameInitialized;

	
};
