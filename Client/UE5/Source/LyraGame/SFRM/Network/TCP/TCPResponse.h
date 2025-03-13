// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "msgpack.hpp"
#include "TCPResponse.generated.h"

USTRUCT(BlueprintType)
struct FResLogin
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite)
	FString packetName;

	UPROPERTY(BlueprintReadWrite)
	int32 resCode;

	UPROPERTY(BlueprintReadWrite)
	int32 channelId;

	UPROPERTY(BlueprintReadWrite)
	FString nick;

	MSGPACK_DEFINE(packetName, resCode, channelId, nick);
};

/**
 * 
 */
class LYRAGAME_API TCPResponse
{
public:
	std::string packetName;
	int32 resCode;

public:
	std::string GetPacketName()
	{
		return packetName;
	}

	TCPResponse() : packetName(""), resCode(0) {}
	virtual ~TCPResponse();
};


class ResLogin : public TCPResponse
{
public:
	int32 channelId;
	std::string nick;

	ResLogin() : TCPResponse(), channelId(0), nick("") {}

	MSGPACK_DEFINE(packetName, resCode, channelId, nick);
};

class ResCheckNick : public TCPResponse
{
public:
	ResCheckNick() : TCPResponse() {}

	MSGPACK_DEFINE(packetName, resCode);
};

class ResCreateNick : public TCPResponse
{
public:
	ResCreateNick() : TCPResponse() {}

	MSGPACK_DEFINE(packetName, resCode);
};

class ResMatching : public TCPResponse
{
public:
	ResMatching() : TCPResponse() {}

	MSGPACK_DEFINE(packetName, resCode);
};

class ResMatchingCancel : public TCPResponse
{
public:
	ResMatchingCancel() : TCPResponse() {}

	MSGPACK_DEFINE(packetName, resCode);
};

class ResMatchingUserCount : public TCPResponse
{
public:
	int32 userCount;

	ResMatchingUserCount() : TCPResponse(), userCount(0) {}

	MSGPACK_DEFINE(packetName, resCode, userCount);
};

class ResMatchingDone : public TCPResponse
{
public:
	std::string ip;
	int32 port;
	int32 process_id;

	ResMatchingDone() : TCPResponse(), ip(""), port(0), process_id(0) {}

	MSGPACK_DEFINE(packetName, resCode, ip, port, process_id);
};

