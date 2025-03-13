// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SFRM/Network/TCP/PacketSession.h"
#include "SFRM/Game/SFGameInstance.h"
#include "SFRM/Network/TCP/TCPRequest.h"
#include "SFRM/Network/TCP/TCPResponse.h"
/**
 * 
 */

class LYRAGAME_API ClientPacketHandler
{
public:
	static bool Handle_FResLogin(TSharedPtr<class PacketSession>& session, const FResLogin& pkt);
	static bool Handle_ResLogin(TSharedPtr<class PacketSession>& session, const ResLogin& pkt);
	static bool Handle_ResCheckNick(TSharedPtr<class PacketSession>& session, const ResCheckNick& pkt);
	static bool Handle_ResCreateNick(TSharedPtr<class PacketSession>& session, const ResCreateNick& pkt);
	static bool Handle_ResMatching(TSharedPtr<class PacketSession>& session, const ResMatching& pkt);
	static bool Handle_ResMatchingCancel(TSharedPtr<class PacketSession>& session, const ResMatchingCancel& pkt);
	static bool Handle_ResMatchingUserCount(TSharedPtr<class PacketSession>& session, const ResMatchingUserCount& pkt);
	static bool Handle_ResMatchingDone(TSharedPtr<class PacketSession>& session, const ResMatchingDone& pkt);


public:
	static TSharedPtr<class SendBuffer> MakeSendBuffer(ReqPing& pkt) { return MakeSendBuffers(pkt); }
	static TSharedPtr<class SendBuffer> MakeSendBuffer(ReqLogin& pkt) { return MakeSendBuffers(pkt); }
	static TSharedPtr<class SendBuffer> MakeSendBuffer(ReqCheckNick& pkt) { return MakeSendBuffers(pkt); }
	static TSharedPtr<class SendBuffer> MakeSendBuffer(ReqCreateNick& pkt) { return MakeSendBuffers(pkt); }
	static TSharedPtr<class SendBuffer> MakeSendBuffer(ReqMatching& pkt) { return MakeSendBuffers(pkt); }
	static TSharedPtr<class SendBuffer> MakeSendBuffer(ReqMatchingCancel& pkt) { return MakeSendBuffers(pkt); }
	static TSharedPtr<class SendBuffer> MakeSendBuffer(ReqMatchingUserCount& pkt) { return MakeSendBuffers(pkt); }
	static TSharedPtr<class SendBuffer> MakeSendBuffer(ReqMatchingDone& pkt) { return MakeSendBuffers(pkt); }

private:
	template<typename T>
	static TSharedPtr<class SendBuffer> MakeSendBuffers(T& pkt)
	{
		std::string cmd = pkt.GetPacketName();
		int16 cmdLength = static_cast<int16>(cmd.length());
		int16 nonceLength = 0;
		TArray<uint8> body = pkt.Serialize();
		int32 bodyLength = body.Num();

		int32 totalSize = sizeof(int16) + cmdLength + sizeof(int16) + sizeof(int32) + bodyLength;

		TArray<uint8> PacketData;

		uint8 TotalSizeBytes[4];
		TotalSizeBytes[0] = (totalSize) & 0xFF;
		TotalSizeBytes[1] = (totalSize >> 8) & 0xFF;
		TotalSizeBytes[2] = (totalSize >> 16) & 0xFF;
		TotalSizeBytes[3] = (totalSize >> 24) & 0xFF;
		PacketData.Append(TotalSizeBytes, sizeof(totalSize));

		//cmd Length
		uint8 cmdLengthBytes[2];
		cmdLengthBytes[0] = (cmdLength) & 0xFF;
		cmdLengthBytes[1] = (cmdLength >> 8) & 0xFF;

		PacketData.Append(cmdLengthBytes, sizeof(cmdLengthBytes));

		//cmd
		FString fstrCmd = FString(cmd.c_str());
		FTCHARToUTF8 Converter(*fstrCmd);
		PacketData.Append((uint8*)Converter.Get(), cmdLength);

		//nonce Length
		uint8 nonceLengthBytes[2];
		nonceLengthBytes[0] = (nonceLength) & 0xFF;
		nonceLengthBytes[1] = (nonceLength >> 8) & 0xFF;
		PacketData.Append(nonceLengthBytes, sizeof(nonceLengthBytes));

		//nonce ����

		//body Length
		uint8 bodyLengthBytes[4];
		bodyLengthBytes[0] = (bodyLength) & 0xFF;
		bodyLengthBytes[1] = (bodyLength >> 8) & 0xFF;
		bodyLengthBytes[2] = (bodyLength >> 16) & 0xFF;
		bodyLengthBytes[3] = (bodyLength >> 24) & 0xFF;
		PacketData.Append(bodyLengthBytes, sizeof(bodyLengthBytes));

		//body
		PacketData.Append(body.GetData(), body.Num());

		TSharedPtr<class SendBuffer> sendBuffer = MakeShared<SendBuffer>(PacketData.Num());
		sendBuffer->CopyData(PacketData.GetData(), PacketData.Num());
		sendBuffer->Close(PacketData.Num());
		return sendBuffer;
	}

};
