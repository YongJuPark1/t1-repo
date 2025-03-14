// Fill out your copyright notice in the Description page of Project Settings.


#include "SFRM/Game/SFGameInstance.h"
#include "msgpack/msgpack.hpp"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Common/TcpSocketBuilder.h"
#include "SFRM/Network/TCP/PacketSession.h"
#include "SFRM/Network/TCP/TCPRequest.h"
#include "SFRM/Network/HTTP/WebProtocol.h"
#include "LyraLogChannels.h"

void USFGameInstance::Init()
{
	Super::Init();

	if (false == GetWorld()->GetTimerManager().IsTimerActive(PingTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(PingTimerHandle, this, &USFGameInstance::SendPingCheck, 3.0f, true);
	}

	if (false == GetWorld()->GetTimerManager().IsTimerActive(RecvTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(RecvTimerHandle, this, &USFGameInstance::HandleRecvPackets, 0.1f, true);
	}

}
void USFGameInstance::ConnectToGameServer()
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer)
	{
		return;
	}

	if (Socket || Connected)
	{
		DisconnectFromGameServer();
	}

	UWebProtocol::FetchServerConfig("LOCAL", [this](const FString& _IP, int32 _Port)
	{
		this->IpAddress = _IP;
		this->Port = _Port;
		FString DebugMessage = FString::Printf(TEXT("[GetServerConfig] IP: %s, Port: %d"), *_IP, _Port);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);

		Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

		FIPv4Address Ip = NULL;
		FIPv4Address::Parse(IpAddress, Ip);

		TSharedPtr<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		InternetAddr->SetIp(Ip.Value);
		InternetAddr->SetPort(_Port);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

		Connected = Socket->Connect(*InternetAddr);

		if (Connected)
		{
			UE_LOG(LogLyraExperience, Log, TEXT("********************Connected Server Successed********************"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connected Success")));
			GameServerSession = MakeShared<PacketSession>(Socket);
			GameServerSession->Run();
		}
		else
		{
			UE_LOG(LogLyraExperience, Log, TEXT("********************Connected Server Falied********************"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connected Falied")));
		}

		OnConnectGameServer.Broadcast(Connected);

	});
	
}

void USFGameInstance::DisconnectFromGameServer()
{
	if (Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
		GameServerSession = nullptr;
	}
}

void USFGameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}



void USFGameInstance::SendPacket(TSharedPtr<class SendBuffer> SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void USFGameInstance::SendPingCheck()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	ReqPing packet;
	SEND_PACKET(packet);
}

void USFGameInstance::LoginProcess()
{
	ReqLogin packet;
	packet.platformType = 1;
	packet.token = TCHAR_TO_UTF8(*strUSN);
	SEND_PACKET(packet);
	
}

void USFGameInstance::SendCheckNick()
{
	ReqCheckNick packet;
	packet.nick = TCHAR_TO_UTF8(*strNick);
	SEND_PACKET(packet);
}

void USFGameInstance::SendCreateNick()
{
	ReqCreateNick packet;
	packet.nick = TCHAR_TO_UTF8(*strNick);
	SEND_PACKET(packet);
}

void USFGameInstance::SendMatching()
{
	ReqMatching packet;
	packet.matchingType = 1;
	SEND_PACKET(packet);
}

void USFGameInstance::SendMatchingCancel()
{
	ReqMatchingCancel packet;
	SEND_PACKET(packet);
}

void USFGameInstance::SendMatchingUserCount()
{
}

void USFGameInstance::SendMatchingDone()
{
}

void USFGameInstance::SetUSN(const FText& USN)
{
	strUSN = USN.ToString();
}

void USFGameInstance::SetNickname(const FString& Nick)
{
	strNick = Nick;
}
