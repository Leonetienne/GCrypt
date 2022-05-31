#include "CommandlineInterface.h"
#include <iostream>
#include <sstream>
#include <GCrypt/Version.h>
#include <GCrypt/Block.h>
#include "Version.h"

using namespace Hazelnp;
using namespace Leonetienne::GCrypt;

void CommandlineInterface::Init(int argc, const char* const* argv) {
  /* General information */
  std::stringstream ss;
  ss << "CLI for the GCrypt cipher/obfuscator" << std::endl
    << "Copyright (c) 2022 Leon Etienne" << std::endl
    << "GCrypt v" << GCRYPT_VERSION << std::endl
    << "GCrypt CLI v" << GCRYPTCLI_VERSION << std::endl
    << "THIS IS EXPERIMENTAL SOFTWARE AND MUST BE CONSIDERED INSECURE. DO NOT USE THIS TO ENCRYPT SENSITIVE DATA! READ THE README FILES ACCESSIBLE AT \"https://gitea.leonetienne.de/leonetienne/GCrypt\"";
  nupp.SetBriefDescription(ss.str());
  ss.str("");
  nupp.SetCatchHelp("true");
  nupp.SetCrashOnFail("true");

  /* Builtin documentation */
  nupp.RegisterDescription("--encrypt", "Use the encryption module.");
  nupp.RegisterConstraint("--encrypt", ParamConstraint(true, DATA_TYPE::VOID, {}, false,  {"--decrypt", "--hash" }));
  nupp.RegisterAbbreviation("-e", "--encrypt");

  nupp.RegisterDescription("--decrypt", "Use decryption module.");
  nupp.RegisterConstraint("--decrypt", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--encrypt", "--hash", "--generate-key" }));
  nupp.RegisterAbbreviation("-d", "--decrypt");

  nupp.RegisterDescription("--hash", "Use the GHash hash module to calculate a hashsum.");
  nupp.RegisterConstraint("--hash", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--encrypt", "--decrypt", "--generate-key" }));
  nupp.RegisterAbbreviation("-h", "--hash");

  nupp.RegisterDescription("--generate-key", "Use the key generation module. Will generate a random key based on hardware events, output it, and exit.");
  nupp.RegisterConstraint("--generate-key", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--encrypt", "--decrypt", "--hash" }));

  nupp.RegisterDescription("--intext", "Encrypt this string.");
  nupp.RegisterConstraint("--intext", ParamConstraint(true, DATA_TYPE::STRING, {}, false, { "--infile" }));
  nupp.RegisterAbbreviation("-it", "--intext");

  nupp.RegisterDescription("--infile", "Encrypt this file.");
  nupp.RegisterConstraint("--infile", ParamConstraint(true, DATA_TYPE::STRING, {}, false, { "--intext" }));
  nupp.RegisterAbbreviation("-if", "--infile");

  nupp.RegisterDescription("--ofile", "Write output in this file.");
  nupp.RegisterConstraint("--ofile", ParamConstraint(true, DATA_TYPE::STRING, {}, false, { "--ostdout", "--hash" }));
  nupp.RegisterAbbreviation("-of", "--ofile");
  nupp.RegisterAbbreviation("-o", "--ofile");

  nupp.RegisterDescription("--key", "Use this value as a password to extrapolate the encryption key. WARNING: Arguments may be logged by the system!");
  nupp.RegisterConstraint("--key", ParamConstraint(true, DATA_TYPE::STRING, {}, false, { "--keyfile", "--keyask", "--hash" }));
  nupp.RegisterAbbreviation("-k", "--key");

  ss << "Read in the first {KEYSIZE}(=" << Block::BLOCK_SIZE_BITS << ") bits of this file and use that as an encryption key. WARNING: Arguments may be logged by the system!";
  nupp.RegisterDescription("--keyfile", ss.str());
  ss.str("");
  nupp.RegisterConstraint("--keyfile", ParamConstraint(true, DATA_TYPE::STRING, {}, false, { "--key", "--keyask", "--hash" }));
  nupp.RegisterAbbreviation("-kf", "--keyfile");

  nupp.RegisterDescription("--keyask", "Read the encryption key from stdin.");
  nupp.RegisterConstraint("--keyask", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--key", "--keyfile", "--hash" }));
  nupp.RegisterAbbreviation("-ka", "--keyask");

  nupp.RegisterDescription("--progress", "Print digestion progress to stdout. May be advisable for large files, as the cipher is rather slow.");
  nupp.RegisterConstraint("--progress", ParamConstraint(true, DATA_TYPE::VOID, {}, false, {}));
  nupp.RegisterAbbreviation("-p", "--progress");

  nupp.RegisterDescription("--iobase-bytes", "Interpret and output ciphertexts as raw bytes.");
  nupp.RegisterConstraint("--iobase-bytes", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--iobase-2", "--iobase-8", "--iobase-10", "--iobase-16", "--iobase-64", "--iobase-uwu", "--iobase-ugh" }));

  nupp.RegisterDescription("--iobase-2", "Interpret and format ciphertexts in base2");
  nupp.RegisterConstraint("--iobase-2", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--iobase-bytes", "--iobase-8", "--iobase-10", "--iobase-16", "--iobase-64", "--iobase-uwu", "--iobase-ugh" }));

  nupp.RegisterDescription("--iobase-8", "Interpret and format ciphertexts in base8");
  nupp.RegisterConstraint("--iobase-8", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--iobase-bytes", "--iobase-2", "--iobase-10", "--iobase-16", "--iobase-64", "--iobase-uwu", "--iobase-ugh" }));

  nupp.RegisterDescription("--iobase-10", "Interpret and format ciphertexts in base10");
  nupp.RegisterConstraint("--iobase-10", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--iobase-bytes", "--iobase-2", "--iobase-8", "--iobase-16", "--iobase-64", "--iobase-uwu", "--iobase-ugh" }));

  nupp.RegisterDescription("--iobase-16", "Interpret and format ciphertexts in base16 (hex)");
  nupp.RegisterConstraint("--iobase-16", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--iobase-bytes", "--iobase-2", "--iobase-8", "--iobase-64", "--iobase-uwu", "--iobase-ugh" }));

  nupp.RegisterDescription("--iobase-64", "Interpret and format ciphertexts in base64");
  nupp.RegisterConstraint("--iobase-64", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--iobase-bytes", "--iobase-2", "--iobase-8", "--iobase-10", "--iobase-16", "--iobase-uwu", "--iobase-ugh" }));

  nupp.RegisterDescription("--iobase-uwu", "Interpret and format ciphertexts in base uwu");
  nupp.RegisterConstraint("--iobase-uwu", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--iobase-bytes", "--iobase-2", "--iobase-8", "--iobase-10", "--iobase-16", "--iobase-64", "--iobase-ugh" }));

  nupp.RegisterDescription("--iobase-ugh", "Interpret and format ciphertexts in base ugh");
  nupp.RegisterConstraint("--iobase-ugh", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--iobase-bytes", "--iobase-2", "--iobase-8", "--iobase-10", "--iobase-16", "--iobase-64", "--iobase-uwu" }));

  nupp.RegisterDescription("--lib-version", "Will supply the version of GCryptLib used.");
  nupp.RegisterConstraint("--lib-version", ParamConstraint(true, DATA_TYPE::VOID, {}, false, {}));

  nupp.RegisterDescription("--cli-version", "Will supply the version of GCryptCLI used.");
  nupp.RegisterConstraint("--cli-version", ParamConstraint(true, DATA_TYPE::VOID, {}, false, {}));
  nupp.RegisterAbbreviation("-v", "--cli-version");

  nupp.RegisterDescription("--puffer-input", "Will read the entire input before beginning any digestion.");
  nupp.RegisterConstraint("--puffer-input", ParamConstraint(true, DATA_TYPE::VOID, {}, false, {}));

  nupp.RegisterDescription("--puffer-output", "Will digest the entire data before initiating any output.");
  nupp.RegisterConstraint("--puffer-output", ParamConstraint(true, DATA_TYPE::VOID, {}, false, {}));

  nupp.RegisterDescription("--no-newline", "Don't postfix stdout output with a newline");
  nupp.RegisterConstraint("--no-newline", ParamConstraint(true, DATA_TYPE::VOID, {}, false, {}));

  /* Now parse */
  nupp.Parse(argc, argv);

  CatchVersionQueries();
  SpecialCompatibilityChecking();

  return;
}

Hazelnp::CmdArgsInterface& CommandlineInterface::Get() {
  return nupp;
}

void CommandlineInterface::SpecialCompatibilityChecking() {

  // Active module
  // Do we have EITHER --encrypt or --decrypt or --hash?
  if (
    (!nupp.HasParam("--generate-key")) &&
    (!nupp.HasParam("--hash")) &&
    (!nupp.HasParam("--encrypt")) &&
    (!nupp.HasParam("--decrypt"))
  ) {
    CrashWithMsg("No module supplied! Please supply either --encrypt, --decrypt, --hash, or --generate-key!");
  }

  // Encryption key
  // Do we have EITHER --hash (no key required), --generate-key (no key required), --key, --keyask or --keyfile given?
  if (
    (!nupp.HasParam("--hash")) &&
    (!nupp.HasParam("--generate-key")) &&
    (!nupp.HasParam("--key")) &&
    (!nupp.HasParam("--keyfile")) &&
    (!nupp.HasParam("--keyask"))
  ) {
    CrashWithMsg("No encryption key supplied! Please supply either --key, --keyfile, or --keyask!");
  }

  // Check that, if supplied, filename strings are not empty.
  if (
    (nupp.HasParam("--ofile")) &&
    (nupp["--ofile"].GetString().length() == 0)
  ) {
    CrashWithMsg("Length of --ofile is zero! That can't be a valid path!");
  }

  if (
    (nupp.HasParam("--ifile")) &&
    (nupp["--ifile"].GetString().length() == 0)
  ) {
    CrashWithMsg("Length of --ifile is zero! That can't be a valid path!");
  }

  if (
    (nupp.HasParam("--keyfile")) &&
    (nupp["--keyfile"].GetString().length() == 0)
  ) {
    CrashWithMsg("Length of --keyfile is zero! That can't be a valid path!");
  }

  return;
}

void CommandlineInterface::CrashWithMsg(const std::string& msg) {
  std::cerr
    << nupp.GetBriefDescription()
    << std::endl
    << "Fatal error! Unable to continue! More information:" << std::endl
    << msg << std::endl;

  exit(-1);
}

void CommandlineInterface::CatchVersionQueries() {
  if (
      (nupp.HasParam("--version")) ||
      (nupp.HasParam("--cli-version"))
  ) {
    std::cout << GCRYPTCLI_VERSION << std::endl;
    exit(0);
  }
  else if (nupp.HasParam("--lib-version"))
  {
    std::cout << GCRYPT_VERSION << std::endl;
    exit(0);
  }

  return;
}

CmdArgsInterface CommandlineInterface::nupp;

