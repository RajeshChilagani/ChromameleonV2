// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "TwitchStructs.generated.h"
/*
Data struct for any metadata field that don't have any specific use they are put in an array called other
*/
USTRUCT(BlueprintType)
struct TWITCHINTEGRATOR_API FChatMessageField {
	GENERATED_BODY()
	/*Name of the metadata field*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FString field_name;
	/*Value of the metadata field*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FString field_value;
};

/*
Data struct for all the main chat message metadata;
*/
USTRUCT(BlueprintType)
struct TWITCHINTEGRATOR_API FChatMessageData {
	GENERATED_BODY()

	/*The username of the person that send the message*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FString sender_username;
	/*If the sender of the message is subbed to the channel*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		bool sender_is_subbed;
	/*If there were bits send with the message*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		bool bits_sent;
	/*The ammount of bits sent with the message*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		float number_of_bits;
	/*The message without metadata*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FString message;
	/*The message without metadata in TEXT form*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FText messsageText;
	/*The color of the sender's username in Linear Color*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FLinearColor sender_username_color;
	/*Sometimes, the field color of the sender's username is empty. In that situation, I generate one at random
	if I did so, this value will equal True*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		bool colorIsAccurate;
	/*The color of the sender's username in Color form*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		FColor sender_username_color_byte;
	/*If the user is a moderator for the selected channel*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		bool isModerator;
	/*Other meta data fields*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		TArray<FChatMessageField> other_Data;
	/*List of all the commands entered*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		TArray<FString> commands_entered;
	/*Weither or not the message contains one or more commands*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		bool containsCommands;
	/*If the sender is VIP*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		bool isVIP;
	/*If the message is a whisper*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Twitch Integrator")
		bool isWhisper;
};


/*Event triggered when receiveign a message from the Twitch API*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChatMessageReceived, FString, message, FChatMessageData, data);
/*Event triggered when receiving a whisper*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChatWhipserReceived, FString, message, FChatMessageData, data);
/*Event triggered when a command that was previously set is entered by any user*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChatCommandEntered, FString, message, FChatMessageData, data);
/*Event triggered when receiveign a message with cheers from the Twitch API*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FChatCheerReceived, FString, sender_username, FString, message, int32, ammount);
/*Event triggered when the connection to twitch chat changes*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTwitchConnectionChanged, bool, connectionStatus);
