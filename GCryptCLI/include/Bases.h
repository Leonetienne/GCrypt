#ifndef GCRYPTCLI_BASES_H
#define GCRYPTCLI_BASES_H

#include <vector>
#include <string>
#include <map>

// This lookup table holds how many digits a block is long
// in any iobase.
// This cannot be calculated on the fly, as it involves
// arithmetic with involving REALLY big numbers (like, 2^512).
// Here's how to calculate these numbers:
// Print an all 1's block in this format, and check the string size.
// That's it.
static auto blockLengthByBase =
  std::map<Configuration::IOBASE_FORMAT, std::size_t>({
      std::make_pair(Configuration::IOBASE_FORMAT::BASE_BYTES, 64),
      std::make_pair(Configuration::IOBASE_FORMAT::BASE_2, 512),
      std::make_pair(Configuration::IOBASE_FORMAT::BASE_8, 171),
      std::make_pair(Configuration::IOBASE_FORMAT::BASE_10, 155),
      std::make_pair(Configuration::IOBASE_FORMAT::BASE_16, 128),
      std::make_pair(Configuration::IOBASE_FORMAT::BASE_64, 86),
      std::make_pair(Configuration::IOBASE_FORMAT::BASE_UWU, 125),
      std::make_pair(Configuration::IOBASE_FORMAT::BASE_UGH, 125)
  });

// Actually useful bases
static const std::vector<std::string> BASE_2  = { "0","1" };
static const std::vector<std::string> BASE_8  = { "0","1","2","3","4","5","6","7"};
static const std::vector<std::string> BASE_10 = { "0","1","2","3","4","5","6","7","8","9" };
static const std::vector<std::string> BASE_64 = { "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9","+","/" };

// Fun bases
static const std::vector<std::string> BASE_UWU = {
  "uwu",
  "UwU",
  "<3",
  ":P",
  ":p",
  ":o",
  ":O",
  ":3",
  ":)",
  "^.^",
  "^_^",
  "^^",
  "XD",
  "XDD",
  "XDDD",
  "xD",
  "xDD",
  "xDDD",
  "(*^.^*)",
  "(*^_^*)",
  "devewopa",
  "cutieee",
  "cutieees",
  "cutewr",
  "whiiiich",
  "masta",
  "mastaaaa",
  "hiiiii",
  "hewwo",
  "soopa",
  "doopa",
  "favowite",
  "kawaii",
  "rawr",
  "keewl",
  "keeeewl",
  "Owww",
  "Awww",
  "haaaai",
  "haaaay",
  "heeeey",
  "heeeei",
  "senpaiiii",
  "i",
  "nyeed",
  "awe",
  "youuu",
  "twe",
  "best",
  "ruff",
  "me",
  "nyeko",
  "eughh",
  "snaffle",
  "toot",
  "whiffle",
  "nappies",
  "chiffchaff",
  "kawaiisu",
  "*winks*",
  "*bites-lip*",
  "*blushes*",
  "*stutters*",
  "*sweats*",
  "uhh?!",
  "uhh..?",
  "nu-uh...",
  "sweatr",
  "comfy-womfy",
  "oopsie-whoopsie",
  "dewicious",
  "tastieee",
  "tasties",
  "boop",
  "smoochies",
  "whiffskaws",
  "chomp",
  "toesy-woesies",
  "smush",
  "snuff",
  "sniff",
  "snaff",
  "nyoo!",
  "nyah!",
};

// Source: https://lakinkonieczny.wordpress.com/2012/12/06/dialogue-noises-agh-ugh-shh-and-so-much-more/
static const std::vector<std::string> BASE_UGH = {
  "Agh!",
  "Ugh",
  "Shh!",
  "Pft!",
  "Aah!",
  "Uaah!",
  "Gah!",
  "Grr...",
  "Duh!",
  "Psh!",
  "Ah...",
  "Er-",
  "Huh...?",
  "Uh-huh...",
  "Nu-uh...",
  "Bah!",
  "Wha-?"
};

#endif

