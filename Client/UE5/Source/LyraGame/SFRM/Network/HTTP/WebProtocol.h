// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "SFRM/Network/HTTP/WebAPI.h"
#include "SFRM/Network/HTTP/WebData.h"
#include "SFRM/Network/HTTP/WebEnum.h"
#include "WebProtocol.generated.h"

#define BASE_URL TEXT("http://172.16.18.207")


// Typedef 정의
typedef TFunction<void(bool bSuccess, FString ResponseMessage)> OnProtocolComplete;

template <typename T>
FString ToJson(const T& Data)
{
    //static_assert(TIsDerivedFrom<T, UStruct>::Value, "T must be a USTRUCT type.");

    FString OutputString;
    if (FJsonObjectConverter::UStructToJsonObjectString(Data, OutputString))
    {
        return OutputString;
    }
    return TEXT("{}"); // 실패 시 빈 JSON 반환
}



/**
 *
 */
UCLASS()
class LYRAGAME_API UWebProtocol : public UObject
{
    GENERATED_BODY()

public:
    UWebProtocol();
    ~UWebProtocol();

    static void FetchServerConfig(const FString& Environment, TFunction<void(const FString&, int32)> Callback);
};

