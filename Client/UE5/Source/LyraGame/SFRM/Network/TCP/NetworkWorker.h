// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "variant"
#include "msgpack.hpp" // msgpack 헤더
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
					// MsgPack 데이터를 std::string으로 읽고 FString으로 변환
					if (o.type == msgpack::type::STR) {
						v = UTF8_TO_TCHAR(o.as<std::string>().c_str());
					}
					else {
						throw msgpack::type_error();
					}
					return o;
				}
			};

			// FString에 대한 직렬화 규칙
			template<>
			struct pack<FString> {
				template <typename Stream>
				packer<Stream>& operator()(msgpack::packer<Stream>& o, const FString& v) const {
					// FString을 std::string으로 변환하여 MsgPack에 저장
					std::string utf8String = TCHAR_TO_UTF8(*v);
					o.pack(utf8String);
					return o;
				}
			};

			// FString에 대한 객체 규칙 정의
			template<>
			struct object_with_zone<FString> {
				void operator()(msgpack::object::with_zone& o, const FString& v) const {
					// FString을 std::string으로 변환 후 MsgPack 객체로 변환
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

	// 매개변수 생성자
	FPacketHeader(uint32 InPacketSize) : PacketSize(InPacketSize)
	{
	}

	// 직렬화 연산자
	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		return Ar;
	}

	uint32 PacketSize; // 패킷 크기
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