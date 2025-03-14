// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "msgpack.hpp" // msgpack ���
#include "SFRM/Game/SFGameInstance.h"


class FSocket;
struct LYRAGAME_API FPacketHeader
{
	FPacketHeader() : PacketSize(0) {}

	// �Ű����� ������
	FPacketHeader(uint32 InPacketSize) : PacketSize(InPacketSize)
	{
	}

	// ����ȭ ������
	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		return Ar;
	}

	uint32 PacketSize; // ��Ŷ ũ��
};

class LYRAGAME_API RecvWorker : public FRunnable
{
public:
	RecvWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~RecvWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

private:
	bool RecivePacket(TArray<uint8>& OutPacket);
	bool ReceiveDeseiredBytes(uint8* Results, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket;
	TWeakPtr<class PacketSession> SessionRef;

};

class LYRAGAME_API SendWorker : public FRunnable
{
public:
	SendWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~SendWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	bool SendPacket(TSharedPtr<class SendBuffer> SendBuffer);

	void Destroy();

private:
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket;
	TWeakPtr<class PacketSession> SessionRef;
};