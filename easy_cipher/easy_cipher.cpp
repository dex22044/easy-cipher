#include "pch.h"
#include "replaceTables.hpp"

using namespace System;
using namespace System::IO;

int ExitWithError(String^ errStr) {
    Console::WriteLine("Error occured: " + errStr);
    Console::ReadKey();
    return -1;
}

int main(array<System::String ^> ^args)
{
    if (args->Length < 4)return ExitWithError("Usage: AES_Cipher <mode (DEC for decode or ENC for encode)> <input file path> <output file path> <password>");
    int mode = args[0] == "DEC" ? 1 : (args[0] == "ENC" ? 2 : -1);
    if (mode == -1)return ExitWithError("Mode error: ");
    String^ inputFilePath = args[1];
    String^ outputFilePath = args[2];
    String^ key = args[3];

    FileStream^ inputFile;
    FileStream^ outputFile;

    try {
        inputFile = File::Open(inputFilePath, FileMode::Open);
        outputFile = File::Open(outputFilePath, FileMode::Create);
    }
    catch (Exception^) { return ExitWithError("IO error"); }

    array<unsigned char>^ inbuffer = gcnew array<unsigned char>(65536);
    array<unsigned char>^ outbuffer = gcnew array<unsigned char>(65536);
    int readLength = 0;
    int keyLen = key->Length;

    ReplaceTables::Initialize();

    if (mode == 2) {
        while ((readLength = inputFile->Read(inbuffer, 0, 65536)) > 0) {
            for (int i = 0; i < readLength; i++) {
                outbuffer[i] = ReplaceTables::encryptTable[(unsigned char)(inbuffer[i] ^ key[i % keyLen])];
            }
            outputFile->Write(outbuffer, 0, readLength);
        }
    }
    else {
        while ((readLength = inputFile->Read(inbuffer, 0, 65536)) > 0) {
            for (int i = 0; i < readLength; i++) {
                outbuffer[i] = (unsigned char)(ReplaceTables::decryptTable[inbuffer[i]] ^ key[i % keyLen]);
            }
            outputFile->Write(outbuffer, 0, readLength);
        }
    }
    inputFile->Close();
    outputFile->Close();
    return 0;
}
