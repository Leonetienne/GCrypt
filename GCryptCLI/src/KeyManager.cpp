#include "KeyManager.h"
#include "CommandlineInterface.h"
#include "Configuration.h"
#include <cstring>
#include <iostream>

// Required for hidden password input
#if defined _WIN32 || defined _WIN64
#include <Windows.h>
#elif defined __GNUG__
#include <termios.h>
#include <unistd.h>
#endif

void KeyManager::PrepareKey() {

  // Special-case: We are hashing:
  //   no key needed.
  if (Configuration::activeModule == Configuration::MODULE::HASH) {
    return;
  }

  // Special-case: We are generating a keyfile:
  //   just take a random one
  else if (Configuration::activeModule == Configuration::MODULE::GENERATE_KEYFILE) {
    key = Key::Random();
    return;
  }

  // Else:
  //   Is a password passed on the command line?
  else if (CommandlineInterface::Get().HasParam("--key")) {
    // Fetch it, and hash it to a key
    std::string password = CommandlineInterface::Get()["--key"].GetString();

    key = Key::FromPassword(password);

    // Don't forget to zero string memory.
    memset(password.data(), 0, password.size());
    return;
  }

  //   Should we prompt for a password on stdin?
  else if (CommandlineInterface::Get().HasParam("--keyask")) {
    // Create a password prompt
    std::string password = PasswordPrompt();

    key = Key::FromPassword(password);

    // Don't forget to zero string memory.
    memset(password.data(), 0, password.size());
    return;
  }

  // Else:
  //   Should we read from a keyfile?
  else if (CommandlineInterface::Get().HasParam("--keyfile")) {
    // Fetch the filename, and read it
    const std::string filepath = CommandlineInterface::Get()["--keyfile"].GetString();
    key = Key::LoadFromFile(filepath);
    return;
  }

  throw std::runtime_error("No key option found. Is the CLI parser configuration correct?.");

  return;
}


const Key& KeyManager::GetKey() {
  return key;
}

std::string KeyManager::PasswordPrompt() {
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
  std::string password;
  std::cin >> password;

// Restore previous config
#if defined _WIN32 || defined _WIN64
  SetConsoleMode(hStdin, mode);

#elif defined __GNUG__
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

  return password;
}

Key KeyManager::key;

