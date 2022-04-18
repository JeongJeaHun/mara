// Copyright Epic Games, Inc. All Rights Reserved.

#include "Networking.h"
#include "GameFramework/Actor.h"
#include "Common/TcpSocketBuilder.h"
#include "Templates/SharedPointer.h"
#include "TimerManager.h"
#include <string>



#pragma optimize("",off)
ANetworking::ANetworking()
{
	if (!StartTCPReceiver("RamaSocketListener", "127.0.0.1", 8890))
	{
		UE_LOG(LogClass, Log, TEXT("TCP Socket Listener Created"));

		return;
	}
	UE_LOG(LogClass, Log, TEXT("TCP Socket Listener Yay"));

}

bool ANetworking::StartTCPReceiver(const FString& SocketName, const FString& IP, const int32 ThePort)
{
	ListenerSocket = CreateTCPConnectionListener(SocketName,IP,ThePort);

	if (!ListenerSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *IP, ThePort));
		return false;
	}

	//GetWorld()->GetTimerManager().SetTimer(this, &ANetworking::TCPConnectionListener, 0.01, true);



	TCPConnectionListener();

	//FTimerHandle handle;
	//GetWorldTimerManager().SetTimer(handle, this, &ANetworking::TCPConnectionListener, 0.01, true);
	



	return true;
}

FSocket* ANetworking::CreateTCPConnectionListener(const FString& SocketName, const FString& IP, const int32 ThePort, const int32 ReceiveBufferSize)
{
	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(IP, IP4Nums))
	{
		UE_LOG(LogClass, Log, TEXT("Invalid IP! Expecting 4 parts separated by "));
		//VShow("Invalid IP! Expecting 4 parts separated by ");
		return false;
	}

	//create socket
	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), ThePort);
	FSocket* ListenSocket = FTcpSocketBuilder(*SocketName).AsReusable().BoundToEndpoint(Endpoint).Listening(8);

	//set buffer size
	int32 NewSize =0;
	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize,NewSize);


	return ListenSocket;
}

void ANetworking::TCPConnectionListener()
{
	if (!ListenerSocket)
	{
		return;
	}

	
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending = true;

	//bool a= ListenerSocket->HasPendingConnection(Pending);
	//Pending = true;

	if (ListenerSocket->HasPendingConnection(Pending) && Pending)
	{
		if (ConnectionSocket)
		{
			ConnectionSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
		}

		//New Connection receive
		ConnectionSocket = ListenerSocket->Accept(*RemoteAddress,TEXT("TCP Received Socket Connection"));

		if (ConnectionSocket != NULL)
		{
			RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);
			
			UE_LOG(LogClass, Log, TEXT("Accepted Connection"));

			FTimerHandle LitenHandle;
			GetWorldTimerManager().SetTimer(LitenHandle, this, &ANetworking::TCPSocketListener, 0.01, true);
			//GetWorldTimerManager().SetTimer(this, &ANetworking::TCPSocketListener, 0.01, true);
		}
	}
}

void ANetworking::TCPSocketListener()
{
	if (!ConnectionSocket)
	{
		return;
	}

	TArray<uint8> ReceivedData;

	uint32 Size;

	while (ConnectionSocket->HasPendingData(Size)) //HasPendingData 대기열에 보류 중인 데이터가 있는지 확인하기 위해 소켓을 쿼리한다
	{
		//ReceivedData.Init(FMath::Min(Size, 65507u));
		ReceivedData.SetNum(FMath::Min(Size, 65507u));
		//ReceivedData.SetNum(Size);
		

		int32 Read =0;
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(),Read);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Data Bytes Read -> %d"), ReceivedData.Num()));
	}
	if (ReceivedData.Num() <= 0)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Data Bytes Read -> %d"), ReceivedData.Num()));

	const FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("As String Data -> %s"), *ReceivedUE4String));


}



bool ANetworking::FormatIP4ToNumber(const FString& IP, uint8(&Out)[4])
{
	IP.Replace(TEXT(" "), TEXT(""));

	//string parts 

	TArray<FString> Parts;
	IP.ParseIntoArray(Parts,TEXT("."),true);
	if (Parts.Num() != 4)
	{
		return false;
	}

	//string to number parts
	for (int32 i = 0; i < 4; i++)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}

	return true;
}

FString ANetworking::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	//BinaryArray.Add(0);

	std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()),BinaryArray.Num());

	return FString(cstr.c_str());
}
#pragma optimize("",on)

