// Fill out your copyright notice in the Description page of Project Settings.


#include "SFRM/Network/HTTP/WebProtocol.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Templates/SharedPointer.h"

UWebProtocol::UWebProtocol()
{
}

UWebProtocol::~UWebProtocol()
{
}


void UWebProtocol::FetchServerConfig(const FString& Environment, TFunction<void(const FString&, int32)> Callback)
{
    FString ServerConfigUrl = FString(BASE_URL) + TEXT("/ServerConfig.json");

    // HTTP 夸没 积己
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(ServerConfigUrl);
    HttpRequest->SetVerb(TEXT("GET"));

    // HTTP 览翠 贸府
    HttpRequest->OnProcessRequestComplete().BindLambda([Callback, Environment](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        if (bWasSuccessful && Response.IsValid())
        {
            FString ResponseString = Response->GetContentAsString();
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

            if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
            {
                TSharedPtr<FJsonObject> EnvironmentsObject = JsonObject->GetObjectField(TEXT("environments"));
                if (EnvironmentsObject.IsValid() && EnvironmentsObject->HasField(Environment))
                {
                    TSharedPtr<FJsonObject> EnvironmentObject = EnvironmentsObject->GetObjectField(Environment);
                    TSharedPtr<FJsonObject> ServerObject = EnvironmentObject->GetObjectField(TEXT("server"));

                    FString IP = ServerObject->GetStringField(TEXT("ip"));
                    int32 Port = ServerObject->GetIntegerField(TEXT("port"));

                    Callback(IP, Port);
                    return;
                }
            }
        }

        Callback(TEXT("0.0.0.0"), 0);
    });

    HttpRequest->ProcessRequest();
}
