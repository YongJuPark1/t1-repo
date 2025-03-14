// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WebData.generated.h"


USTRUCT(BlueprintType)
struct FResData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 result_code = 0;

    UPROPERTY(BlueprintReadWrite)
    FString result_message = "";
};

/**
 *
 */
class LYRAGAME_API WebData
{
public:
    WebData();
    ~WebData();
};
