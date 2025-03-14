// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "variant"
#include "vector"
#include "msgpack.hpp"
#include "TCPResponse.generated.h"

namespace msgpack {
	MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
		namespace adaptor {

			template<typename T>
			struct convert<TArray<T>> {
				msgpack::object const& operator()(msgpack::object const& o, TArray<T>& v) const {
					if (o.type != msgpack::type::ARRAY) {
						throw msgpack::type_error();
					}

					v.Empty();
					for (size_t i = 0; i < o.via.array.size; ++i) {
						T item;
						o.via.array.ptr[i].convert(item);
						v.Add(item);
					}
					return o;
				}
			};

			template<typename T>
			struct pack<TArray<T>> {
				template <typename Stream>
				packer<Stream>& operator()(msgpack::packer<Stream>& o, const TArray<T>& v) const {
					o.pack_array(v.Num());
					for (const auto& item : v) {
						o.pack(item);
					}
					return o;
				}
			};

			template<typename T>
			struct object_with_zone<TArray<T>> {
				void operator()(msgpack::object::with_zone& o, const TArray<T>& v) const {
					o.type = msgpack::type::ARRAY;
					o.via.array.size = v.Num();
					o.via.array.ptr = static_cast<msgpack::object*>(
						o.zone.allocate_align(sizeof(msgpack::object) * v.Num())
						);

					for (size_t i = 0; i < v.Num(); ++i) {
						o.via.array.ptr[i] = msgpack::object(v[i], o.zone);
					}
				}
			};

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

