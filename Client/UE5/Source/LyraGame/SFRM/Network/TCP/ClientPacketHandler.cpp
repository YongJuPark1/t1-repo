// Fill out your copyright notice in the Description page of Project Settings.


#include "SFRM/Network/TCP/ClientPacketHandler.h"
#include "SFRM/Game/SFGameInstance.h"


bool ClientPacketHandler::Handle_FResLogin(TSharedPtr<class PacketSession>& session, const FResLogin& pkt)
{
	FString DebugMessage = FString::Printf(TEXT("[ResLogin] PacketName: %s, Nick: %s"), *pkt.packetName, *pkt.nick);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);


	if (auto* GameInstance = Cast<USFGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->SetNickname(pkt.nick);
		GameInstance->OnLoginResponse2.Broadcast(pkt.resCode, pkt);
	}
	return true;
}

bool ClientPacketHandler::Handle_ResLogin(TSharedPtr<class PacketSession>& session, const ResLogin& pkt)
{

	std::string paketName = pkt.packetName;
	int32 channelId = pkt.channelId;
	std::string nick = pkt.nick;

	FString DebugMessage = FString::Printf(TEXT("[ResLogin] Channel: %d, Nick: %s"), channelId, UTF8_TO_TCHAR(nick.c_str()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);

	if (auto* GameInstance = Cast<USFGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->SetNickname(nick.c_str());
		GameInstance->OnLoginResponse.Broadcast(pkt.resCode, pkt.channelId, pkt.nick.c_str());
	}
	return true;
}

bool ClientPacketHandler::Handle_ResCheckNick(TSharedPtr<class PacketSession>& session, const ResCheckNick& pkt)
{
	FString DebugMessage = FString::Printf(TEXT("[ResCheckNick] ResCode: %d"), pkt.resCode);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);

	if (auto* GameInstance = Cast<USFGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->OnCheckNickResponse.Broadcast(pkt.resCode);
	}
	return true;
}

bool ClientPacketHandler::Handle_ResCreateNick(TSharedPtr<class PacketSession>& session, const ResCreateNick& pkt)
{
	FString DebugMessage = FString::Printf(TEXT("[ResCreateNick] ResCode: %d"), pkt.resCode);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);

	if (auto* GameInstance = Cast<USFGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->OnCreateNickResponse.Broadcast(pkt.resCode);
	}

	return true;
}

bool ClientPacketHandler::Handle_ResMatching(TSharedPtr<class PacketSession>& session, const ResMatching& pkt)
{
	FString DebugMessage = FString::Printf(TEXT("[ResMatching] ResCode: %d"), pkt.resCode);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);

	if (auto* GameInstance = Cast<USFGameInstance>(GWorld->GetGameInstance()))
	{	
		GameInstance->OnMatchingResponse.Broadcast(pkt.resCode);
	}

	return true;
}

bool ClientPacketHandler::Handle_ResMatchingCancel(TSharedPtr<class PacketSession>& session, const ResMatchingCancel& pkt)
{
	FString DebugMessage = FString::Printf(TEXT("[ResMatchingCancel] ResCode: %d"), pkt.resCode);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);

	return true;
}

bool ClientPacketHandler::Handle_ResMatchingUserCount(TSharedPtr<class PacketSession>& session, const ResMatchingUserCount& pkt)
{
	FString DebugMessage = FString::Printf(TEXT("[ResMatchingUserCount] ResCode: %d, userCount : %d"), pkt.resCode, pkt.userCount);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);

	if (auto* GameInstance = Cast<USFGameInstance>(GWorld->GetGameInstance()))
	{	
	}

	return true;
}

bool ClientPacketHandler::Handle_ResMatchingDone(TSharedPtr<class PacketSession>& session, const ResMatchingDone& pkt)
{
	FString DebugMessage = FString::Printf(TEXT("[ResMatchingDone] ResCode: %d, ip : %s, port : %d , process_id : %d"), pkt.resCode, pkt.ip.c_str(), pkt.port, pkt.process_id);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);

	if (auto* GameInstance = Cast<USFGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->OnMatchingDoneResponse.Broadcast(pkt.resCode, pkt.ip.c_str(), pkt.port);
	}

	return true;
}

