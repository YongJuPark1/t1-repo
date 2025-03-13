// Fill out your copyright notice in the Description page of Project Settings.


#include "SFRM/Game/SFGameInstance.h"
#include "msgpack/msgpack.hpp"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Common/TcpSocketBuilder.h"
#include "SFRM/Network/TCP/PacketSession.h"
#include "SFRM/Network/TCP/TCPRequest.h"



void USFGameInstance::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip = NULL;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedPtr<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connected Success")));
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connected Falied")));
	}
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
