// Fill out your copyright notice in the Description page of Project Settings.


#include "SFRM/Network/TCP/PacketSession.h"
#include "SFRM/Network/TCP/NetworkWorker.h"
#include "msgpack/msgpack.hpp"

PacketSession::PacketSession(FSocket* Socket) : Socket(Socket)
{
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());
	SendWorkerThread = MakeShared<SendWorker>(Socket, AsShared());
}

void PacketSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;

		Decode(Packet);
	}
}

void PacketSession::SendPacket(TSharedPtr<class SendBuffer> SendBuffer)
{
	SendPacketQueue.Enqueue(SendBuffer);
}

void PacketSession::Disconnect()
{
	if (RecvWorkerThread)
	{
		RecvWorkerThread->Destroy();
		RecvWorkerThread = nullptr;
	}

	if (SendWorkerThread)
	{
		SendWorkerThread->Destroy();
		SendWorkerThread = nullptr;
	}
}

void PacketSession::Decode(const TArray<uint8>& SerializedData)
{
	if (SerializedData.Num() < 6) // 최소 6바이트(4바이트 + 2바이트) 필요
	{
		UE_LOG(LogTemp, Error, TEXT("SerializedData is too small to contain required fields!"));
		return;
	}

	const uint8* DataPtr = SerializedData.GetData();


	int32 PacketSize = 0;
	FMemory::Memcpy(&PacketSize, DataPtr, sizeof(PacketSize));
	DataPtr += sizeof(PacketSize);

	int16 CmdLength = 0;
	FMemory::Memcpy(&CmdLength, DataPtr, sizeof(CmdLength));
	DataPtr += sizeof(CmdLength);

	if (CmdLength < 0 || SerializedData.Num() < 6 + CmdLength)
	{
		UE_LOG(LogTemp, Error, TEXT("SerializedData does not contain enough data for the specified cmd length!"));
		return;
	}

	FString cmd = FString(UTF8_TO_TCHAR(reinterpret_cast<const char*>(DataPtr)));
	DataPtr += CmdLength;

	int16 NonceLength = 0;
	FMemory::Memcpy(&NonceLength, DataPtr, sizeof(NonceLength));
	DataPtr += sizeof(NonceLength);

	int32 BodyLength = 0;
	FMemory::Memcpy(&BodyLength, DataPtr, sizeof(BodyLength));
	DataPtr += sizeof(BodyLength);

	TArray<uint8> BodyData;

	if (BodyLength < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("BodyLength < 0"));
		return;
	}

	BodyData.Append(DataPtr, BodyLength);
	DataPtr += BodyLength;

	TSharedPtr<class PacketSession> ThisPtr = AsShared();

	if (cmd == "ReqLogin")
	{
		ClientPacketHandler::Handle_ResLogin(ThisPtr, Unpack<ResLogin>(BodyData));
		ClientPacketHandler::Handle_FResLogin(ThisPtr, Unpack<FResLogin>(BodyData));
		
	}
	else if (cmd == "ReqCheckNick")
	{
		ClientPacketHandler::Handle_ResCheckNick(ThisPtr, Unpack<ResCheckNick>(BodyData));
	}
	else if (cmd == "ReqCreateNick")
	{
		ClientPacketHandler::Handle_ResCreateNick(ThisPtr, Unpack<ResCreateNick>(BodyData));
	}
	else if (cmd == "ReqMatching")
	{
		ClientPacketHandler::Handle_ResMatching(ThisPtr, Unpack<ResMatching>(BodyData));
	}
	else if (cmd == "ReqMatchingUserCount")
	{
		ClientPacketHandler::Handle_ResMatchingUserCount(ThisPtr, Unpack<ResMatchingUserCount>(BodyData));
	}
	else if (cmd == "ReqMatchingDone")
	{
		ClientPacketHandler::Handle_ResMatchingDone(ThisPtr, Unpack<ResMatchingDone>(BodyData));
	}
}

template<typename T>
inline T PacketSession::Unpack(const TArray<uint8>& SerializedData)
{
	try
	{
		T Packet;
		msgpack::unpacked Unpacked;
		msgpack::unpack(Unpacked, reinterpret_cast<const char*>(SerializedData.GetData()), SerializedData.Num());
		Unpacked.get().convert(Packet);
		return Packet;
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogTemp, Error, TEXT("Unpacking error: %s"), ANSI_TO_TCHAR(e.what()));
		return T();
	}
}

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.SetNum(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* data, int32 len)
{
	::memcpy(_buffer.GetData(), data, len);
	_writeSize = len;
}

void SendBuffer::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}