#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "System/LyraGameInstance.h"
#include "SFRM/Network/TCP/TCPResponse.h"
#include "SFRM/Network/TCP/ClientPacketHandler.h"
#include "SFRM/Network/TCP/PacketSession.h"
#include "SFGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectGameServer, bool, bConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLoginResponse, int32, resultCode, int32, channelId, FString, nick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckNickResponse, int32, resultCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateNickResponse, int32, resultCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchingResponse, int32, resultCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchingCancelResponse, int32, resultCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMatchingDoneResponse, int32, resultCode, FString, ip, int32, port);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoginResponse2, int32, resultCode, FResLogin, res);

/**
 * 
 */
UCLASS()
class LYRAGAME_API USFGameInstance : public ULyraGameInstance
{
	GENERATED_BODY()	

protected:
	virtual void Init() override;

public:
	void SendPacket(TSharedPtr<class SendBuffer> SendBuffer);

	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	UFUNCTION(BlueprintCallable)
	void SendPingCheck();

	UFUNCTION(BlueprintCallable)
	void LoginProcess();

	UFUNCTION(BlueprintCallable)
	void SendCheckNick();

	UFUNCTION(BlueprintCallable)
	void SendCreateNick();

	UFUNCTION(BlueprintCallable)
	void SendMatching();

	UFUNCTION(BlueprintCallable)
	void SendMatchingCancel();

	UFUNCTION(BlueprintCallable)
	void SendMatchingUserCount();

	UFUNCTION(BlueprintCallable)
	void SendMatchingDone();

public:
	UFUNCTION(BlueprintCallable)
	void SetUSN(const FText& USN);

	UFUNCTION(BlueprintCallable)
	void SetNickname(const FString& Nick);

public:
	UPROPERTY(BlueprintAssignable, Category = "Login")
	FOnConnectGameServer OnConnectGameServer;


	UPROPERTY(BlueprintAssignable, Category = "Login")
	FOnLoginResponse OnLoginResponse;

	UPROPERTY(BlueprintAssignable, Category = "Login")
	FOnLoginResponse2 OnLoginResponse2;

	UPROPERTY(BlueprintAssignable, Category = "Nick")
	FOnCheckNickResponse OnCheckNickResponse;

	UPROPERTY(BlueprintAssignable, Category = "Nick")
	FOnCreateNickResponse OnCreateNickResponse;

	UPROPERTY(BlueprintAssignable, Category = "Matching")
	FOnMatchingResponse OnMatchingResponse;

	UPROPERTY(BlueprintAssignable, Category = "Matching")
	FOnMatchingCancelResponse OnMatchingCancelResponse;

	UPROPERTY(BlueprintAssignable, Category = "Matching")
	FOnMatchingDoneResponse OnMatchingDoneResponse;


public:
	class FSocket* Socket;
	//FString IpAddress = TEXT("127.0.0.1");
	FString IpAddress = TEXT("172.16.18.207");
	int16 Port = 10501;
	TSharedPtr<class PacketSession> GameServerSession;

	FString strUSN;
	FString strSessionId;
	FString strNick;

private:
	FTimerHandle PingTimerHandle;
	FTimerHandle RecvTimerHandle;

	bool Connected = false;

};

#define SEND_PACKET(Pkt)														\
	TSharedPtr<class SendBuffer> SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);		\
	Cast<USFGameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);