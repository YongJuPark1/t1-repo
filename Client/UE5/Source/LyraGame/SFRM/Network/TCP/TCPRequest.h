// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "variant"
#include "SFRM/Network/TCP/NetworkWorker.h"

/**
 * 
 */
class LYRAGAME_API TCPRequest
{
public:
	std::string GetPacketName() 
	{
		return packetName;
	}

	virtual TArray<uint8> Serialize() const
	{
		msgpack::sbuffer Buffer;
		msgpack::packer<msgpack::sbuffer> Packer(&Buffer);

		auto Fields = GetSerializableFields();
		Packer.pack_array(Fields.size());

		for (const auto& Field : Fields)
		{
			Packer.pack(Field);
		}

		TArray<uint8> SerializedData;
		SerializedData.Append((uint8*)Buffer.data(), Buffer.size());
		return SerializedData;
	}

public:
	TCPRequest();
	virtual ~TCPRequest();

protected:
	using SerializableField = std::variant<int32, std::string, float, double, bool>;

	std::string packetName;

	virtual std::vector<SerializableField> GetSerializableFields() const
	{
		return { packetName };
	}
};

class ReqLogin : public TCPRequest
{
public:
	ReqLogin() { packetName = "ReqLogin"; };
	int32 platformType;
	std::string token;

protected:
	std::vector<SerializableField> GetSerializableFields() const override
	{
		auto Fields = TCPRequest::GetSerializableFields();
		Fields.push_back(platformType);
		Fields.push_back(token);
		return Fields;
	}
};

class ReqPing : public TCPRequest
{
public:
	ReqPing() { packetName = "ReqPing"; }
};

class ReqCheckNick : public TCPRequest
{
public:
	ReqCheckNick() { packetName = "ReqCheckNick"; };
	std::string nick;

protected:
	std::vector<SerializableField> GetSerializableFields() const override
	{
		auto Fields = TCPRequest::GetSerializableFields();
		Fields.push_back(nick);
		return Fields;
	}
};

class ReqCreateNick : public TCPRequest
{
public:
	ReqCreateNick() { packetName = "ReqCreateNick"; };
	std::string nick;

protected:
	std::vector<SerializableField> GetSerializableFields() const override
	{
		auto Fields = TCPRequest::GetSerializableFields();
		Fields.push_back(nick);
		return Fields;
	}
};

class ReqMatching : public TCPRequest
{
public:
	ReqMatching() { packetName = "ReqMatching"; };
	int32 matchingType;

protected:
	std::vector<SerializableField> GetSerializableFields() const override
	{
		auto Fields = TCPRequest::GetSerializableFields();
		Fields.push_back(matchingType);
		return Fields;
	}
};

class ReqMatchingCancel : public TCPRequest
{
public:
	ReqMatchingCancel() { packetName = "ReqMatchingCancel"; }
};

class ReqMatchingUserCount : public TCPRequest
{
public:
	ReqMatchingUserCount() { packetName = "ReqMatchingUserCount"; }
};

class ReqMatchingDone : public TCPRequest
{
public:
	ReqMatchingDone() { packetName = "ReqMatchingDone"; }
};