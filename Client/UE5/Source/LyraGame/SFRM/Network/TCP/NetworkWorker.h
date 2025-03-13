// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "variant"
#include "msgpack.hpp" // msgpack ���
#include "SFRM/Game/SFGameInstance.h"

namespace msgpack {
	MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
		namespace adaptor {

			template <typename... Types>
			struct pack<std::variant<Types...>> {
				template <typename Stream>
				packer<Stream>& operator()(msgpack::packer<Stream>& o, const std::variant<Types...>& v) const {
					std::visit([&o](auto&& value) { o.pack(value); }, v);
					return o;
				}
			};
			template <typename... Types>
			struct object_with_zone<std::variant<Types...>> {
				void operator()(msgpack::object::with_zone& o, const std::variant<Types...>& v) const {
					std::visit([&o](auto&& value) { o << value; }, v);
				}
			};

			template<>
			struct convert<FString> {
				msgpack::object const& operator()(msgpack::object const& o, FString& v) const {
					// MsgPack �����͸� std::string���� �а� FString���� ��ȯ
					if (o.type == msgpack::type::STR) {
						v = UTF8_TO_TCHAR(o.as<std::string>().c_str());
					}
					else {
						throw msgpack::type_error();
					}
					return o;
				}
			};

			// FString�� ���� ����ȭ ��Ģ
			template<>
			struct pack<FString> {
				template <typename Stream>
				packer<Stream>& operator()(msgpack::packer<Stream>& o, const FString& v) const {
					// FString�� std::string���� ��ȯ�Ͽ� MsgPack�� ����
					std::string utf8String = TCHAR_TO_UTF8(*v);
					o.pack(utf8String);
					return o;
				}
			};

			// FString�� ���� ��ü ��Ģ ����
			template<>
			struct object_with_zone<FString> {
				void operator()(msgpack::object::with_zone& o, const FString& v) const {
					// FString�� std::string���� ��ȯ �� MsgPack ��ü�� ��ȯ
					std::string utf8String = TCHAR_TO_UTF8(*v);
					o.type = msgpack::type::STR;
					o.via.str.size = utf8String.size();
					o.via.str.ptr = utf8String.c_str();
				}
			};

		}
	}
}

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