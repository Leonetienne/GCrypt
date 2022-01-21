#include "CommandlineInterface.h"
#include <iostream>
#include "../GhettoCrypt/Version.h"
#include "../GhettoCrypt/Config.h"
#include "Version.h"

using namespace Hazelnp;

void CommandlineInterface::Init(int argc, const char* const* argv)
{
	/* General information */
	std::stringstream ss;
	ss << "CLI for the ghettocrypt cipher/obfuscator" << std::endl
		<< "Copyright (c) 2022 Leon Etienne" << std::endl
		<< "Ghettocrypt v" << GHETTOCRYPT_VERSION << std::endl
		<< "Ghettocrypt CLI v" << GHETTOCRYPTCLI_VERSION << std::endl
		<< "THIS IS EXPERIMENTAL SOFTWARE AND MUST BE CONSIDERED INSECURE. DO NOT USE THIS TO ENCRYPT SENSITIVE DATA! READ THE README FILE ACCESSIBLE AT \"https://github.com/Leonetienne/GhettoCrypt/blob/master/readme.md\"";
	nupp.SetBriefDescription(ss.str());
	ss.str("");
	nupp.SetCatchHelp("true");
	nupp.SetCrashOnFail("true");

	/* Builtin documentation */
	nupp.RegisterDescription("--encrypt", "Use the encryption routine.");
	nupp.RegisterConstraint("--encrypt", ParamConstraint(true, DATA_TYPE::VOID, {}, false, {"--decrypt"}));
	nupp.RegisterAbbreviation("-e", "--encrypt");

	nupp.RegisterDescription("--decrypt", "Use decryption routine.");
	nupp.RegisterConstraint("--decrypt", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--encrypt" }));
	nupp.RegisterAbbreviation("-d", "--decrypt");

	nupp.RegisterDescription("--intext", "Encrypt this string. Dumps to stdout.");
	nupp.RegisterConstraint("--intext", ParamConstraint(true, DATA_TYPE::STRING, {}, false, { "--infile" }));
	nupp.RegisterAbbreviation("-it", "--intext");

	nupp.RegisterDescription("--infile", "Encrypt this file. Saves as {filename}.crypt, if not specified otherwise.");
	nupp.RegisterConstraint("--infile", ParamConstraint(true, DATA_TYPE::STRING, {}, false, { "--intext" }));
	nupp.RegisterAbbreviation("-if", "--infile");

	nupp.RegisterDescription("--ofile", "Use this filename for output if --infile is specified. Gets ignored otherwise.");
	nupp.RegisterConstraint("--ofile", ParamConstraint(true, DATA_TYPE::STRING, {}, false, { "--ostdout" }));
	nupp.RegisterAbbreviation("-of", "--ofile");
	nupp.RegisterAbbreviation("-o", "--ofile");

	nupp.RegisterDescription("--ostdout", "Output of digested files will be dumped to stdout instead of a file.");
	nupp.RegisterConstraint("--ostdout", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--ofile" }));

	nupp.RegisterDescription("--key", "Use this value as a password to extrapolate the encryption key. WARNING: Arguments may be logged by the system!");
	nupp.RegisterConstraint("--key", ParamConstraint(true, DATA_TYPE::STRING, {}, false, { "--keyfile", "--keyask" }));
	nupp.RegisterAbbreviation("-k", "--key");

	ss << "Read in the first {KEYSIZE}(=" << GhettoCipher::BLOCK_SIZE << ") bits of this file and use that as an encryption key. WARNING: Arguments may be logged by the system!";
	nupp.RegisterDescription("--keyfile", ss.str());
	ss.str("");
	nupp.RegisterConstraint("--keyfile", ParamConstraint(true, DATA_TYPE::STRING, {}, false, { "--key", "--keyask" }));
	nupp.RegisterAbbreviation("-kf", "--keyfile");

	nupp.RegisterDescription("--keyask", "Read the encryption key from stdin.");
	nupp.RegisterConstraint("--keyask", ParamConstraint(true, DATA_TYPE::VOID, {}, false, { "--key", "--keyfile" }));
	nupp.RegisterAbbreviation("-ka", "--keyask");

	nupp.RegisterDescription("--progress", "Print digestion progress to stdout. May be advisable for large files, as the cipher is rather slow.");
	nupp.RegisterConstraint("--progress", ParamConstraint(true, DATA_TYPE::VOID, {}, false, {}));
	nupp.RegisterAbbreviation("-p", "--progress");

	nupp.RegisterDescription("--version", "Will supply the version of ghettocrypt used.");
	nupp.RegisterConstraint("--version", ParamConstraint(true, DATA_TYPE::VOID, {}, false, {}));
	nupp.RegisterAbbreviation("-v", "--version");

	nupp.RegisterDescription("--cli-version", "Will supply the version of ghettocrypt-cli used.");
	nupp.RegisterConstraint("--cli-version", ParamConstraint(true, DATA_TYPE::VOID, {}, false, {}));

	/* Now parse */
	nupp.Parse(argc, argv);

	CatchVersionQueries();
	SpecialCompatibilityChecking();

	return;
}

Hazelnp::CmdArgsInterface& CommandlineInterface::Get()
{
	return nupp;
}

void CommandlineInterface::SpecialCompatibilityChecking()
{
	// Encryption key
	// Do we have EITHER --key, --keyask or --keyfile given?
	if (
		(!nupp.HasParam("--key")) &&
		(!nupp.HasParam("--keyfile")) &&
		(!nupp.HasParam("--keyask"))
		)
		CrashWithMsg("No encryption key supplied! Please supply either --key, --keyfile, or --keyask!");

	// Encryption input
	// Do we have EITHER --intext or --infile?
	if (
		(!nupp.HasParam("--intext")) &&
		(!nupp.HasParam("--infile"))
		)
		CrashWithMsg("No encryption input supplied! Please supply either --intext or --infile!");

	// Encryption mode
	// Do we have EITHER --encrypt or --decrypt?
	if (
		(!nupp.HasParam("--encrypt")) &&
		(!nupp.HasParam("--decrypt"))
		)
		CrashWithMsg("No encryption mode supplied! Please supply either --encrypt or --decrypt!");

	return;
}

void CommandlineInterface::CrashWithMsg(const std::string& msg)
{
	std::cerr
		<< nupp.GetBriefDescription()
		<< std::endl
		<< "Fatal error! Unable to continue! More information:" << std::endl
		<< msg << std::endl;

	exit(-1);
}

void CommandlineInterface::CatchVersionQueries()
{
	if (nupp.HasParam("--version"))
	{
		std::cout << GHETTOCRYPT_VERSION << std::endl;
		exit(0);
	}
	else if (nupp.HasParam("--cli-version"))
	{
		std::cout << GHETTOCRYPTCLI_VERSION << std::endl;
		exit(0);
	}

	return;
}

CmdArgsInterface CommandlineInterface::nupp;
