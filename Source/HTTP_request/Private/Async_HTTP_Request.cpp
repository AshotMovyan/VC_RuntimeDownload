// Fill out your copyright notice in the Description page of Project Settings.


#include "Async_HTTP_Request.h"
#include "RuntimeArchiverBase.h"

UAsync_HTTP_Request* UAsync_HTTP_Request::DownloadAndUnzipContent(FHttpRequestData HttpRequestData, FUnzipData UnzipData, FDirectory Directory)
{
    UAsync_HTTP_Request* BlueprintAction = NewObject<UAsync_HTTP_Request>();
    BlueprintAction->HttpRequestData = HttpRequestData;
    BlueprintAction->UnzipData = UnzipData;
    BlueprintAction->Directory = Directory;
    BlueprintAction->MyArchiver = URuntimeArchiverBase::CreateRuntimeArchiver(BlueprintAction, UnzipData.ArchiverClass);


    return BlueprintAction;
}

void UAsync_HTTP_Request::Activate()
{
    //Super::Activate();


    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

    //Directory.ArchivePath = SelectIsParentDirectory(Directory.ArchivePath, Directory.bAddParentDirectory);
    //Directory.DirectoryPath = SelectIsParentDirectory(Directory.DirectoryPath, Directory.bAddParentDirectory);

    // Set the request verb based on the provided enum
    switch (HttpRequestData.Verb)
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

    Request->SetURL(HttpRequestData.URL);
    Request->SetContentAsString(HttpRequestData.Content);
    Request->OnProcessRequestComplete().BindUObject(this, &UAsync_HTTP_Request::HandleHTTPRequestCompleted);
    Request->OnRequestProgress().BindUObject(this, &UAsync_HTTP_Request::HandleRequestProgress);

    Request->ProcessRequest();
}

void UAsync_HTTP_Request::HandleHTTPRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        //BlueprintAction
        //URuntimeArchiverBase* RuntimeArchiverRef = URuntimeArchiverBase::CreateRuntimeArchiver(BlueprintAction, ArchiverClass);
        
        FString SavePath = Directory.bAddParentDirectory ? FPaths::ProjectContentDir() + Directory.ArchivePath : Directory.ArchivePath + Directory.EntryName;
        FFileHelper::SaveArrayToFile(Response->GetContent(), *SavePath);
        MyUnarchivingToStorage();
        // Call the completion delegate with the success status
        OnRequestCompleted.Broadcast(true);
    }
    else
    {
        // Call the completion delegate with the failure status
        OnRequestCompleted.Broadcast(false);
    }

    // Mark the action as completed
    //SetReadyToDestroy();
    //MarkAsFinished();
}

void UAsync_HTTP_Request::HandleRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
    // Call the progress delegate with the bytes sent and received
    OnRequestProgress.Broadcast(BytesSent, BytesReceived);
}

FString UAsync_HTTP_Request::SelectIsParentDirectory(FString Path, bool bAddParentDirectory_arg)
{
    return bAddParentDirectory_arg ? FPaths::ProjectContentDir() + Path : Path;
}

void UAsync_HTTP_Request::MyUnarchivingToStorage()
{
    if (!MyArchiver->OpenArchiveFromStorage(Directory.ArchivePath))
    {
        OnRequestCompleted.Broadcast(false);
        return;
    }

    UnarchiveOperationResult.BindDynamic(this, &UAsync_HTTP_Request::UnarchiveOnResult_Callback);

    MyArchiver->ExtractEntriesToStorage_Directory(UnarchiveOperationResult, MoveTemp(Directory.EntryName), MoveTemp(Directory.DirectoryPath), Directory.bAddParentDirectory, UnzipData.bForceOverwrite);

}



void UAsync_HTTP_Request::UnarchiveOnResult_Callback(bool bSuccess)
{
    UnarchiveOperationResult.Clear();

    bSuccess &= MyArchiver->CloseArchive();

    if (!bSuccess)
    {
        OnRequestCompleted.Broadcast(false);
        return;
    }

    OnRequestCompleted.Broadcast(true);
}

void UAsync_HTTP_Request::UnarchiveOnProgress_Callback()
{
    OnRequestProgress.Broadcast(0, 0);
}