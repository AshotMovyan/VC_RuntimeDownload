#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "HttpModule.h"
#include "RuntimeLoadImplement.generated.h"

UENUM(BlueprintType)
enum class EHTTPRequestVerb : uint8
{
    GET,
    POST,
    PUT,
    DELETE
};

UCLASS()
class HTTP_REQUEST_API ARuntimeLoadImplement : public AActor
{
    GENERATED_BODY()


public:
    UFUNCTION(BlueprintCallable, Category = "HTTP")
        void MakeHTTPRequest(EHTTPRequestVerb Verb, FString URL, FString Content);

private:
    void OnHTTPRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void OnRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);
    void OnFileDownloadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};