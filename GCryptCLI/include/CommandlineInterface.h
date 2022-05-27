#ifndef GCRYPTCLI_CLIINTERFACE_H
#define GCRYPTCLI_CLIINTERFACE_H

#include <Hazelnupp/CmdArgsInterface.h>

class CommandlineInterface
{
public:
  static void Init(const int argc, const char* const* argv);

  static Hazelnp::CmdArgsInterface& Get();

private:
  //! Special command compatibility checking unique to this app
  static void SpecialCompatibilityChecking();
  static void CrashWithMsg(const std::string& msg);
  static void CatchVersionQueries();

  CommandlineInterface() {};

  static Hazelnp::CmdArgsInterface nupp;
};

#endif

