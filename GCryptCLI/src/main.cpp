#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>
#include <GCrypt/Util.h>
#include <GCrypt/Config.h>
#include <GCrypt/Cipher.h>
#include <GCrypt/Flexblock.h>
#include <GCrypt/Block.h>
#include "CommandlineInterface.h"

#include "Bases.h"
#include <GeneralUtility/BaseConversion.h>
#include <StringTools/StringTools.h>

// Required for hidden password input
#if defined _WIN32 || defined _WIN64
#include <Windows.h>
#elif defined __GNUG__
#include <termios.h>
#include <unistd.h>
#endif

using namespace ::Leonetienne::GCrypt;
using namespace ::Leonetienne::StringTools;
using namespace ::Leonetienne::GeneralUtility;

enum class OPERATION_MODE {
    ENCRYPT,
    DECRYPT,
    HASH
};

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
        return PasswordToKey(CommandlineInterface::Get()["--key"].GetString());

    // Case: Ask for key
    else if (CommandlineInterface::Get().HasParam("--keyask"))
        return PasswordToKey(PasswordPrompt());

    // Case: Read key from file
    else if (CommandlineInterface::Get().HasParam("--keyfile"))
    {
        const std::string keyfilepath = CommandlineInterface::Get()["--keyfile"].GetString();

        // Read this many chars
        const std::size_t maxChars = BLOCK_SIZE / 8;

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

const Flexblock GetInputText(const OPERATION_MODE operationMode)
{
    // Easy-case: input text supplied as param
    if (CommandlineInterface::Get().HasParam("--intext")) {
        switch (operationMode) {
        // Encryption, or hashing mode: We want to return the text as-is, as bits
        case OPERATION_MODE::ENCRYPT:
        case OPERATION_MODE::HASH:
            return StringToBits(CommandlineInterface::Get()["--intext"].GetString());

        // Decryption mode: We need to first convert our input to a bitstring
        case OPERATION_MODE::DECRYPT:
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
    }


    // Case: Read from file
    else if (CommandlineInterface::Get().HasParam("--infile"))
        return ReadFileToBits(CommandlineInterface::Get()["--infile"].GetString());

    // Unreachable
    throw std::runtime_error("This code should not have been reached. Most likely, the cli argument parser failed making sure at least one input method was supplied.");
}

const std::string GetOutfileName(const OPERATION_MODE operationMode)
{
    // Do we have an output file name specified?
    // Use it.
    if (CommandlineInterface::Get().HasParam("--ofile"))
        return CommandlineInterface::Get()["--ofile"].GetString();

    // Else: append a custom postfix to the inputs filename
    else
        return CommandlineInterface::Get()["--infile"].GetString() +
            (operationMode == OPERATION_MODE::ENCRYPT ? ".crypt" : ".plain");
}

int main(int argc, char** argv)
{
    // Init cmdargs
    CommandlineInterface::Init(argc, argv);

    // Get operation modes
    OPERATION_MODE opMode;
    if (CommandlineInterface::Get().HasParam("--encrypt"))
        opMode = OPERATION_MODE::ENCRYPT;
    else if (CommandlineInterface::Get().HasParam("--hash"))
        opMode = OPERATION_MODE::HASH;
    else if (CommandlineInterface::Get().HasParam("--decrypt"))
        opMode = OPERATION_MODE::DECRYPT;
    else
        // Unreachable
        throw std::runtime_error("This code should not have been reached. Most likely, the cli argument parser failed making sure at least one mode of operation (-e, -d, -h) was supplied.");

    const bool isFileMode = CommandlineInterface::Get().HasParam("--infile");
    const bool printDigestionProgress = CommandlineInterface::Get().HasParam("--progress");


    // Digest
    Flexblock output;

    switch (opMode) {
        case OPERATION_MODE::ENCRYPT: {
            // Gather input and key
            const Block encryptionKey = GetEncryptionKey();
            const Flexblock input = GetInputText(opMode);

            // Create cipher
            Cipher cipher(encryptionKey);

            // Run it
            output = cipher.Encipher(input, printDigestionProgress);
            break;
        }

        case OPERATION_MODE::DECRYPT: {
            // Gather input and key
            const Block encryptionKey = GetEncryptionKey();
            const Flexblock input = GetInputText(opMode);

            // Create cipher
            Cipher cipher(encryptionKey);

            // Run it
            output = cipher.Decipher(input, printDigestionProgress);
            break;
        }

        case OPERATION_MODE::HASH: {
            // Gather input
            Flexblock input = GetInputText(opMode);

            // Also add an extra left-padded block with the length of the input,
            // to make sure, H(n) != H(n + 0x0)
            input += PadStringToLength(Block(input.size()).to_string(), BLOCK_SIZE, '0');

            // Derive an encryption key
            // This is quiet sensitive, because it absolutely MUST be impossible
            // to acquire K without already knowing M.
            // This alone is making this cipher a one-way function.
            // Just reducing the output is not a solution, because: If len(input) <= BLOCK_SIZE: H(m) = E(m)_k ^ k, with k being 0 <= k <= BLOCK_SIZE. That's not good.
            //
            // So here's what we're doing:
            // - We're going to take the first BLOCK_SIZE bits of the input (it's length is at least that because of the size embedding)
            // - We're creating a sudo-random initialization vector, seeded with this block.
            // - That's our encryption key.
            // This way, the encryption key can only be obtained by already knowing the first BLOCK_SIZE bits of the cleartext.
            // By not taking in ALL the bits for deriving a key, we're saving a LOT on performance, as otherwise we would basically be encrypting the input THREE times.
            // Why create an initialization vector at all? To make sure, even an input of only zeroes would provide a strong key
            const Block encryptionKey = InitializationVector(
                    Block(input.substr(0, BLOCK_SIZE))
                );

            // Create cipher
            Cipher cipher(encryptionKey);

            // Run it
            output = cipher.Encipher(input, printDigestionProgress);

            if (printDigestionProgress) {
                std::cout << "Done encrypting... Applying reduction function on ciphertext..." << std::endl;
                std::cout << "This will take about just as long..." << std::endl;
            }

            // Reduce output to a single block
            output = ReductionFunction_Flexblock2Block(output).to_string();

            break;
        }
    }


    // Now output the darn thing.
    // Outputting to a file (only if not hashing)
    if ((isFileMode) && (opMode != OPERATION_MODE::HASH))
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
            const std::string outfileName = GetOutfileName(opMode);
            WriteBitsToFile(outfileName, output);
        }
    }
    // Else: we are just dealing with a string, or want to print a hashsum
    else
    {
        switch (opMode) {
        case OPERATION_MODE::ENCRYPT:
        case OPERATION_MODE::HASH: {

            // We are encrypting or hashing:
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
            break;
        }

        case OPERATION_MODE::DECRYPT:
            // We are just decrypting:
            // Just print it string-formatted
            std::cout << BitsToString(output) << std::endl;
            break;
        }
    }

    return 0;
}

