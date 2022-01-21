#include <iostream>
#include <fstream>
#include <cstring>
#include "CommandlineInterface.h"
#include "../GhettoCrypt/Util.h"
#include "../GhettoCrypt/Config.h"
#include "../GhettoCrypt/Cipher.h"
#include "../GhettoCrypt/Flexblock.h"
#include "../GhettoCrypt/Block.h"

#if defined _WIN32 || defined _WIN64
#include <Windows.h>
#elif defined __GNUG__
#include <termios.h>
#include <unistd.h>
#endif

using namespace GhettoCipher;

//! Will prompt a user password from stdin, hiding the input
std::string PasswordPrompt()
{
    // Disable stdin-echo
    #if defined _WIN32 || defined _WIN64
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

    #elif defined __GNUG__
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    #endif

    // Prompt stdin
    std::string key;
    std::cin >> key;

    // Restore previous config
    #if defined _WIN32 || defined _WIN64
    SetConsoleMode(hStdin, mode);

    #elif defined __GNUG__
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    #endif

    // Return input
    return key;
}

Block GetEncryptionKey()
{
    // Easy-case: key supplied as param
    if (CommandlineInterface::Get().HasParam("--key"))
        return StringToBitblock(CommandlineInterface::Get()["--key"].GetString());

    // Case: Ask for key
    else if (CommandlineInterface::Get().HasParam("--keyask"))
        return StringToBitblock(PasswordPrompt());

    // Case: Read key from file
    else if (CommandlineInterface::Get().HasParam("--keyfile"))
    {
        const std::string keyfilepath = CommandlineInterface::Get()["--keyfile"].GetString();

        // Read this many chars
        const std::size_t maxChars = GhettoCipher::BLOCK_SIZE / 8;

        // Open ifilestream for keyfile
        std::ifstream ifs(keyfilepath, std::ios::in | std::ios::binary);

        // Is the file open now? Or were there any issues...
        if (!ifs.good())
        {
            std::cerr << "Unable to open ifilestream for keyfile \"" << keyfilepath << "\"! Aborting..." << std::endl;
            exit(-1);
        }

        // Read these chars to buffer
        char* ckeyfileContent = new char[maxChars];
        memset(ckeyfileContent, 0, maxChars * sizeof(char));
        ifs.read(ckeyfileContent, maxChars);
        ifs.close();

        // Convert the buffer to a bit block of key size
        std::stringstream ss;
        for (std::size_t i = 0; i < maxChars; i++)
            ss << std::bitset<8>(ckeyfileContent[i]);
        
        Block key(ss.str());

        // And delete the buffer
        delete[] ckeyfileContent;
        ckeyfileContent = nullptr;

        // Return it
        return key;
    }

    // Unreachable
    throw std::runtime_error("This code should not have been reached. Most likely, the cli argument parser failed making sure at least one key method was supplied.");
}

const Flexblock GetInputText(bool encryptionMode)
{
    // Easy-case: input text supplied as param
    if (CommandlineInterface::Get().HasParam("--intext"))
        // Encryption mode: We want to return the text as-is, as bits
        if (encryptionMode)
            return StringToBits(CommandlineInterface::Get()["--intext"].GetString());
        // Decryption mode: We need to first convert hexstring to bitstring
        else
            return HexstringToBits(CommandlineInterface::Get()["--intext"].GetString());


    // Case: Read from file
    else if (CommandlineInterface::Get().HasParam("--infile"))
        return ReadFileToBits(CommandlineInterface::Get()["--infile"].GetString());

    // Unreachable
    throw std::runtime_error("This code should not have been reached. Most likely, the cli argument parser failed making sure at least one key method was supplied.");
}

const std::string GetOutfileName(const bool isEncryptionMode)
{
    // Do we have an output file name specified?
    // Use it.
    if (CommandlineInterface::Get().HasParam("--ofile"))
        return CommandlineInterface::Get()["--ofile"].GetString();
    
    // Else: append a custom postfix to the inputs filename
    else
        return CommandlineInterface::Get()["--infile"].GetString() + (isEncryptionMode ? ".crypt" : ".plain");
}

int main(int argc, char** argv)
{
    // Init cmdargs
    CommandlineInterface::Init(argc, argv);

    // Get encryption key
    const Block encryptionKey = GetEncryptionKey();

    // Get operation modes
    const bool shouldEncrypt = CommandlineInterface::Get().HasParam("--encrypt");
    const bool isFileMode = CommandlineInterface::Get().HasParam("--infile");

    // Get the input text
    const Flexblock input = GetInputText(shouldEncrypt);

    // Digest
    Flexblock output;
    Cipher cipher(encryptionKey);

    const bool printDigestionProgress = CommandlineInterface::Get().HasParam("--progress");

    if (shouldEncrypt)
        output = cipher.Encipher(input, printDigestionProgress);
    else
        output = cipher.Decipher(input, printDigestionProgress);

    // Now output the darn thing.
    // Outputting a file
    if (isFileMode)
    {
        // File mode is a bit different.

        // Dump to stdout?
        if (CommandlineInterface::Get().HasParam("--ostdout"))
        {
            const std::string outstr = BitsToString(output);

            // We have to print char-by-char to prevent a nullbyte terminating output.
            for (std::size_t i = 0; i < outstr.size(); i++)
                std::cout << outstr[i];
        }
        // Else: Dump to file
        else
        {
            const std::string outfileName = GetOutfileName(shouldEncrypt);
            WriteBitsToFile(outfileName, output);
        }
    }
    // Else: we are just dealing with a string
    else
    {
        // Output to stdout as a hexstring
        if (shouldEncrypt)
            std::cout << BitsToHexstring(output) << std::endl;
        else
            std::cout << BitsToString(output) << std::endl;
    }

    return 0;
}
