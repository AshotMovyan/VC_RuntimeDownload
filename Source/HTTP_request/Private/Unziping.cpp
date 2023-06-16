// Fill out your copyright notice in the Description page of Project Settings.



#include "Unziping.h"
//#include "ZipFileFunctionLibrary.h"
#include <fstream>
#include <zlib.h>

bool DecompressFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    gzFile inputFile = gzopen(inputFilePath.c_str(), "rb");
    if (inputFile == NULL) {
        return false;
    }

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

int main() {
    const std::string inputFilePath = "C:/Users/ashot/OneDrive/Рабочий стол/New folder (2)/New folder.tar.gz";
    const std::string outputFilePath = "C:/Users/ashot/OneDrive/Рабочий стол/New folder (2)/uncompressed_file.txt";

    if (DecompressFile(inputFilePath, outputFilePath)) {
        return 0;
    }
    else {
        return 1;
    }
}

Unziping::Unziping()
{
}

Unziping::~Unziping()
{
}
