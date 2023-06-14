#include "RuntimeLoadImplement.h"

void ARuntimeLoadImplement::MakeHTTPRequest(EHTTPRequestVerb Verb, FString URL, FString Content)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

    // Set the request verb based on the provided enum
    switch (Verb)
    {
    case EHTTPRequestVerb::GET:
        Request->SetVerb(TEXT("GET"));
        break;
    case EHTTPRequestVerb::POST:
        Request->SetVerb(TEXT("POST"));
        break;
    case EHTTPRequestVerb::PUT:
        Request->SetVerb(TEXT("PUT"));
        break;
    case EHTTPRequestVerb::DELETE:
        Request->SetVerb(TEXT("DELETE"));
        break;
    default:
        break;
    }

    Request->SetURL(URL);
    Request->SetContentAsString(Content);
    Request->OnProcessRequestComplete().BindUObject(this, &ARuntimeLoadImplement::OnHTTPRequestCompleted);
    Request->OnRequestProgress().BindUObject(this, &ARuntimeLoadImplement::OnRequestProgress);

    if (Request->GetVerb() == TEXT("GET"))
    {
        Request->OnProcessRequestComplete().BindUObject(this, &ARuntimeLoadImplement::OnFileDownloadComplete);
        Request->ProcessRequest();
    }
    else
    {
        if (!Request->ProcessRequest())
        {
            // Handle request processing failure
        }
    }
}

void ARuntimeLoadImplement::OnHTTPRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        FString ResponseString = Response->GetContentAsString();

        if (Request->GetVerb() == TEXT("GET"))
        {
            // Handle the downloaded file (ResponseString)
        }
        else
        {
            // Handle other types of responses (e.g., POST, PUT, DELETE)
        }
    }
    else
    {
        // Handle request failure
    }
}

void ARuntimeLoadImplement::OnRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
    // Calculate the download progress percentage or perform other progress-related tasks

}
void ARuntimeLoadImplement::OnFileDownloadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {

        FPaths::ProjectDir();
        FString SavePath = FPaths::ProjectContentDir() + "cookies/" + "001.zip"; // Set the save path to the project's Content directory with the desired file name and extension

        FFileHelper::SaveArrayToFile(Response->GetContent(), *SavePath);

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s = StringVariable"), *SavePath));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("StringVariable"));

        // Handle the downloaded file at SavePath
    }
    else
    {
        // Handle file download failure
    }
}





        //UE_LOG(LogTemp, Warning, TEXT("HTTP Recuest SUCCES"));
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s = StringVariable"), *ResponseString));