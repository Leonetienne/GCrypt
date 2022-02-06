#include "CppUnitTest.h"
#include "../GhettoCrypt/Cipher.h"
#include "../GhettoCrypt/Util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GhettoCipher;

namespace SimpleTests
{
	TEST_CLASS(EncryptEqualsDecrypt)
	{
	public:
		
		// Tests that a decrypted ciphertext equals its plaintrext version
		TEST_METHOD(tEncryptEqualsDecrypt)
		{
			// Yes, this unit test should ideally exclude string conversions,
			// But like this it's easier to see what it's doing

			// Define basic input
			const std::string cleartext = "Hello, World!";
			const std::string password  = "1234";


			// Instanciate our cipher and supply a key
			const Cipher cipher(password);

			// Recode the ascii-string to bits
			const Flexblock cleartext_bits = StringToBits(cleartext);

			// Encrypt our cleartext bits
			const Flexblock ciphertext_bits = cipher.Encipher(cleartext_bits);
			
			// Decipher it again
			const Flexblock decryptedBits = cipher.Decipher(ciphertext_bits);

			// Decode it back to ascii
			const std::string decryptedText = BitsToString(decryptedBits);

			// Assert that the decrypted text equals the plaintext
			Assert::AreEqual(cleartext.length(), decryptedText.length());
		}
	};
}
