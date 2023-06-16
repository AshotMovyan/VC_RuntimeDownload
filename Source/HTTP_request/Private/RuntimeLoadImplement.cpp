#include "RuntimeLoadImplement.h"
#include "RuntimeArchiverBase.h"
#include <fstream>
#include <zlib.h>

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

    FPaths::ProjectDir();
    SavePath = FPaths::ProjectContentDir() + "cookies/" + "001.zip"; // Set the save path to the project's Content directory with the desired file name and extension

    /*
    const std::string inputFilePath = "E:/Ashot/Files/GZTest/New folder.tar.gz";
    const std::string outputFilePath = "E:/Ashot/Files/GZTest/uncompressed_file.tar";

    if (DecompressFile(inputFilePath, outputFilePath)) {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("true"));
    }
    else {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("StringVariable"));
    }*/
}

void ARuntimeLoadImplement::ExtractFiles(const FString& ArchiveFilePath, const FString& OutputDirectory)
{
    // Create an instance of the archiver
    URuntimeArchiverBase* Archiver = URuntimeArchiverBase::CreateRuntimeArchiver(ARuntimeLoadImplement::StaticClass(), URuntimeArchiverBase::StaticClass());

    // Check if the archiver instance was created successfully
    if (Archiver)
    {
        // Set up the delegates for result and progress callbacks (if needed)
        FRuntimeArchiverAsyncOperationResult OnResult;
        FRuntimeArchiverAsyncOperationProgress OnProgress;
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("qwertyui"));
        // Call ExtractEntriesToStorage_Directory function on the archiver instance
        Archiver->ExtractEntriesToStorage_Directory(OnResult, FString("qwer"), ArchiveFilePath, true, true);
    }
    else
    {
        // Handle error: Failed to create archiver instance
    }
    //archiverBaseic->ExtractEntriesToStorage_Directory(&ARuntimeLoadImplement::OnExtract, FString("name"), ArchiveFilePath, false);
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

        FFileHelper::SaveArrayToFile(Response->GetContent(), *SavePath);
        
        //ARuntimeLoadImplement::ExtractFiles(FString());

        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s = StringVariable"), *SavePath));
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("StringVariable"));

        // Handle the downloaded file at SavePath
    }
    else
    {
        // Handle file download failure
    }
}




/*
bool ARuntimeLoadImplement::DecompressFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("test1"));
    gzFile inputFile = gzopen(inputFilePath.c_str(), "rb");
    if (inputFile == NULL) {
        return false;
    }
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("test2"));
    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        gzclose(inputFile);
        return false;
    }

    const int bufferSize = 128 * 1024; // Adjust the buffer size according to your needs
    char buffer[bufferSize];

    int bytesRead = 0;
    while (!gzeof(inputFile)) {
        bytesRead = gzread(inputFile, buffer, bufferSize);

        if (bytesRead > 0) {
            outputFile.write(buffer, bytesRead);
        }
    }

    gzclose(inputFile);
    outputFile.close();

    return true;
}
*/

        //UE_LOG(LogTemp, Warning, TEXT("HTTP Recuest SUCCES"));
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s = StringVariable"), *ResponseString));