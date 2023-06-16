// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPlatformFilePak.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "zlib.h"


class HTTP_REQUEST_API Unziping
{
public:
	Unziping();
	~Unziping();

	static bool UnzipFile(const FString& ZipFilePath, const FString& OutputDir);


};
