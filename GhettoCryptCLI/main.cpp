#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>
#include "CommandlineInterface.h"
#include "../GhettoCrypt/Util.h"
#include "../GhettoCrypt/Config.h"
#include "../GhettoCrypt/Cipher.h"
#include "../GhettoCrypt/Flexblock.h"
#include "../GhettoCrypt/Block.h"

#include "Bases.h"
#include "BaseConversion.h"
#include "StringTools.h"

// Required for hidden password input
#if defined _WIN32 || defined _WIN64
#include <Windows.h>
#elif defined __GNUG__
#include <termios.h>
#include <unistd.h>
#endif

using namespace GhettoCipher;

namespace {
    inline std::string Bin2CustomBase(const std::string& bin, const std::vector<std::string>& customSet, const std::string& seperator = "")
    {
        std::stringstream ss;

        // Translate to custom set
        const std::vector<std::string> vec_base_custom =
            Leonetienne::GeneralUtility::BaseConversion::BaseX_2_Y<std::string, std::vector<std::string>>(bin, "01", customSet);

        // Convert to string
        for (std::size_t i = 0; i < vec_base_custom.size(); i++)
        {
            ss << vec_base_custom[i];
         
            if (i != vec_base_custom.size() - 1)
                ss << seperator;
        }

        return ss.str();
    }

    inline std::string CustomBase2Bin(const std::string& in, const std::vector<std::string>& customSet, const std::string& seperator = "")
    {
        // Split input into symbols
        const std::vector<std::string> in_symbols = StringTools::Split(in, seperator);

        // Translate to binary
        std::string binary =
            Leonetienne::GeneralUtility::BaseConversion::BaseX_2_Y<std::vector<std::string>, std::string>(in_symbols, customSet, std::string("01"));
        
        // Apply padding to be a multiple of BLOCK_SIZE
        // Count how many bits we need
        std::size_t required_size = 0;
        while (required_size < binary.length())
            required_size += BLOCK_SIZE;

        // Create a string of that many zeroes
        std::string padding = "";
        for (std::size_t i = binary.length(); i < required_size; i++)
            padding += "0";

        // Prepend it to our bitstring
        binary = padding + binary;

        return binary;
    }
}

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

        // Decryption mode: We need to first convert our input to a bitstring
        else
        {
            const std::string userInput = CommandlineInterface::Get()["--intext"].GetString();

            // Are we using iobase 2?
            if (CommandlineInterface::Get().HasParam("--iobase-2"))
                // Yes: convert to binary from base 2
                return CustomBase2Bin(userInput, BASE_2);

            // Are we using iobase 8?
            else if (CommandlineInterface::Get().HasParam("--iobase-8"))
                // Yes: convert to binary from base 8
                return CustomBase2Bin(userInput, BASE_8);

            // Are we using iobase 10?
            else if (CommandlineInterface::Get().HasParam("--iobase-10"))
                // Yes: convert to binary from base 10
                return CustomBase2Bin(userInput, BASE_10);

            // Are we using iobase 64?
            else if (CommandlineInterface::Get().HasParam("--iobase-64"))
                // Yes: convert to binary from base 64
                return CustomBase2Bin(userInput, BASE_64);

            // Are we using iobase uwu?
            else if (CommandlineInterface::Get().HasParam("--iobase-uwu"))
                // Yes: convert to binary from base uwu
                return CustomBase2Bin(userInput, BASE_UWU, " ");

            // Are we using iobase emoji?
            else if (CommandlineInterface::Get().HasParam("--iobase-ugh"))
                // Yes: convert to binary from base ugh
                return CustomBase2Bin(userInput, BASE_UGH, " ");

            // Else, no special output format specified, use hex
            else
                return HexstringToBits(userInput);
        }


    // Case: Read from file
    else if (CommandlineInterface::Get().HasParam("--infile"))
        return ReadFileToBits(CommandlineInterface::Get()["--infile"].GetString());

    // Unreachable
    throw std::runtime_error("This code should not have been reached. Most likely, the cli argument parser failed making sure at least one input method was supplied.");
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
            const std::string outstr = BitsToBytes(output);

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
        if (shouldEncrypt) {
            // We are decrypting:
            // Output to stdout as a formatted bytestring
            std::string formattedCiphertext;

            // Are we using iobase 2?
            if (CommandlineInterface::Get().HasParam("--iobase-2"))
                // Yes: convert hex to base 2
                formattedCiphertext = Bin2CustomBase(output, BASE_2);

            // Are we using iobase 8?
            else if (CommandlineInterface::Get().HasParam("--iobase-8"))
                // Yes: convert binary to base 8
                formattedCiphertext = Bin2CustomBase(output, BASE_8);

            // Are we using iobase 10?
            else if (CommandlineInterface::Get().HasParam("--iobase-10"))
                // Yes: convert binary to base 10
                formattedCiphertext = Bin2CustomBase(output, BASE_10);

            // Are we using iobase 64?
            else if (CommandlineInterface::Get().HasParam("--iobase-64"))
                // Yes: convert binary to base 64
                formattedCiphertext = Bin2CustomBase(output, BASE_64);

            // Are we using iobase uwu?
            else if (CommandlineInterface::Get().HasParam("--iobase-uwu"))
                // Yes: convert binary to base 64
                formattedCiphertext = Bin2CustomBase(output, BASE_UWU, " ");

             // Are we using iobase ugh?
            else if (CommandlineInterface::Get().HasParam("--iobase-ugh"))
                // Yes: convert binary to base 64
                formattedCiphertext = Bin2CustomBase(output, BASE_UGH, " ");

            // Else, no special output format specified, use hex
            else
                formattedCiphertext = BitsToHexstring(output);

            std::cout << formattedCiphertext << std::endl;
        }
        else {
            // We are just decrypting:
            // Just print it string-formatted
            std::cout << BitsToString(output) << std::endl;
        }
    }

    return 0;
}
