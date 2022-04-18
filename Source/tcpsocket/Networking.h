// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

// 삭제해도 무방 아래
#include <Engine.h>
#include <Sockets.h>
#include <SocketSubsystem.h>
#include <Interfaces/IPv4/IPv4Endpoint.h>
// 삭제해도 무방 위

#include "Networking.generated.h"

UCLASS()
class ANetworking : public AActor
{
	GENERATED_BODY()

	ANetworking();

public:
	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;
	FIPv4Endpoint RemoteAddressForConnection;

	bool StartTCPReceiver(
		const FString& SocketName,
		const FString& IP,
		const int32 ThePort
	);

	FSocket* CreateTCPConnectionListener(
		const FString& SocketName,
		const FString& IP,
		const int32 ThePort,
		const int32 ReceiveBufferSize = 2 * 1024 * 1024
	);

	//timer functions, could be threads
	void TCPConnectionListener();
	void TCPSocketListener();

	bool FormatIP4ToNumber(const FString& IP, uint8 (&Out)[4]);

	FString StringFromBinaryArray(const TArray<uint8> & BinaryArray);


public:
  /*  FORCEINLINE void VShow(const TCHAR* Str)
    {
        ClientMessage(FString(Str));
    }

    FORCEINLINE void VShow(const FString& Str)
    {
        ClientMessage(Str);
    }
    FORCEINLINE void VShow(const FString& Str, const FString& Str2)
    {
        ClientMessage(Str + FString(" ") + Str2);
    }
    FORCEINLINE void VShow(FString Str, const float& Value)
    {
        Str += " ";
        Str += FString::SanitizeFloat(Value);
        ClientMessage(Str);
    }
    FORCEINLINE void VShow(FString Str, const int32& Value)
    {
        Str += " ";
        Str += FString::FromInt(Value);
        ClientMessage(Str);
    }
    FORCEINLINE void VShow(FString Str, const uint32& Value)
    {
        Str += " ";
        Str += FString::FromInt(Value);
        ClientMessage(Str);
    }*/
};

/*

public:
    FORCEINLINE void VShow(const TCHAR* Str)
    {
        ClientMessage(FString(Str));
    }

    FORCEINLINE void VShow(const FString& Str)
    {
        ClientMessage(Str);
    }
    FORCEINLINE void VShow(const FString& Str, const FString& Str2)
    {
        ClientMessage(Str + FString(" ") + Str2);
    }
    FORCEINLINE void VShow(FString Str, const float& Value)
    {
        Str += " ";
        Str += FString::SanitizeFloat(Value);
        ClientMessage(Str);
    }
    FORCEINLINE void VShow(FString Str, const int32& Value)
    {
        Str += " ";
        Str += FString::FromInt(Value);
        ClientMessage(Str);
    }
    FORCEINLINE void VShow(FString Str, const uint32& Value)
    {
        Str += " ";
        Str += FString::FromInt(Value);
        ClientMessage(Str);
    }

*/