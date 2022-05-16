/*
#include "CppUnitTest.h"
#include "../GhettoCrypt/Util.h"
#include "../GhettoCrypt/Config.h"
#include <unordered_map>
#include <codecvt>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GhettoCipher;

// We can generate passwords by just translating a decimal number to base "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
inline std::string Base10_2_X(const unsigned long long int i, const std::string set, unsigned int padding)
{
	if (set.length() == 0)
		return ""; // Return empty string, if set is empty. Play stupid games, win stupid prizes.

	std::stringstream ss;

	if (i != 0)
	{
		{
			unsigned long long int buf = i;
			while (buf != 0)
			{
				const unsigned long long int mod = buf % set.length();
				buf /= set.length();
				ss << set[(std::size_t)mod];
			}
		}
		{
			const std::string buf = ss.str();
			ss.str("");
			for (long long int i = buf.length() - 1; i >= 0; i--)
				ss << buf[(std::size_t)i];
		}
	}
	else
	{
		ss << set[0]; // If i is 0, just pass a null-value. The algorithm would hang otherwise.
	}

	// Add as much null-values to the left as requested.
	if (ss.str().length() < padding)
	{
		const std::size_t cachedLen = ss.str().length();
		const std::string cachedStr = ss.str();
		ss.str("");
		for (std::size_t i = 0; i < padding - cachedLen; i++)
			ss << set[0];
		ss << cachedStr;
	}

	return ss.str();
}

using convert_t = std::codecvt_utf8<wchar_t>;

namespace SimpleTests
{
	TEST_CLASS(Password2Key)
	{
	public:

		// Run a few thousand random passwords through the keygen and see if we'll find a collision.
		// This test passing does NOT mean that it's resistant! Maybe good, maybe shit! But if it fails, it's definitely shit.
		// Already validated range: Password 0 - 1.000.000
		TEST_METHOD(CollisionResistance)
		{
			// To test resistence set this to a high number around a million.
			// This will take a LONG while to execute though (about 2.5hrs on my machine), hence why it's set so low.
			constexpr std::size_t NUM_RUN_TESTS = 1000;

			std::unordered_map<std::bitset<BLOCK_SIZE>, std::string> keys; // <key, password>

			// Try NUM_RUN_TESTS passwords
			const std::string charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

			std::wstring_convert<convert_t, wchar_t> strconverter;

			for (std::size_t i = 0; i < NUM_RUN_TESTS; i++)
			{
				// Get password
				const std::string password = Base10_2_X(i, charset, 0);
				
				// Generate key
				const std::bitset<BLOCK_SIZE> newKey = PasswordToKey(password).Get();

				// Check if this block is already in our map
				if (keys.find(newKey) != keys.cend())
				{
					std::wstringstream wss;
					wss << "Collision found between password \""
						<< strconverter.from_bytes(password)
						<< "\" and \""
						<< strconverter.from_bytes(keys[newKey])
						<< "\". The key is \""
						<< newKey
						<< "\".";

					Assert::Fail(wss.str().c_str());
				}

				// All good? Insert it into our map
				keys[newKey] = password;
			}

			return;
		}
	};
}
*/
