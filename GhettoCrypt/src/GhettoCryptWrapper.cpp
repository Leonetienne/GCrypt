#include "GhettoCryptWrapper.h"
#include "Cipher.h"
#include "Util.h"

std::string GhettoCipher::GhettoCryptWrapper::EncryptString(const std::string& cleartext, const std::string& password)
{
	// Instanciate our cipher and supply a key
	const Block key = PasswordToKey(password);
	Cipher cipher(key);

	// Recode the ascii-string to bits
	const Flexblock cleartext_bits = StringToBits(cleartext);

	// Encrypt our cleartext bits
	const Flexblock ciphertext_bits = cipher.Encipher(cleartext_bits);

	// Recode the ciphertext bits to a hex-string
	const std::string ciphertext = BitsToHexstring(ciphertext_bits);

	// Return it
	return ciphertext;
}

std::string GhettoCipher::GhettoCryptWrapper::DecryptString(const std::string& ciphertext, const std::string& password)
{
	// Instanciate our cipher and supply a key
	const Block key = PasswordToKey(password);
	Cipher cipher(key);

	// Recode the hex-string to bits
	const Flexblock ciphertext_bits = HexstringToBits(ciphertext);

	// Decrypt the ciphertext bits
	const std::string cleartext_bits = cipher.Decipher(ciphertext_bits);

	// Recode the cleartext bits to an ascii-string
	const std::string cleartext = BitsToString(cleartext_bits);

	// Return it
	return cleartext;
}

bool GhettoCipher::GhettoCryptWrapper::EncryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password, bool printProgressReport)
{
	try
	{
		// Read the file to bits
		const Flexblock cleartext_bits = ReadFileToBits(filename_in);

		// Instanciate our cipher and supply a key
		const Block key = PasswordToKey(password);
		Cipher cipher(key);

		// Encrypt our cleartext bits
		const Flexblock ciphertext_bits = cipher.Encipher(cleartext_bits, printProgressReport);

		// Write our ciphertext bits to file
		WriteBitsToFile(filename_out, ciphertext_bits);

		return true;
	}
	catch (std::runtime_error&)
	{
		return false;
	}
}

bool GhettoCipher::GhettoCryptWrapper::DecryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password, bool printProgressReport)
{
	try
	{
		// Read the file to bits
		const Flexblock ciphertext_bits = ReadFileToBits(filename_in);

		// Instanciate our cipher and supply a key
		const Block key = PasswordToKey(password);
		Cipher cipher(key);

		// Decrypt the ciphertext bits
		const Flexblock cleartext_bits = cipher.Decipher(ciphertext_bits, printProgressReport);

		// Write our cleartext bits to file
		WriteBitsToFile(filename_out, cleartext_bits);

		return true;
	}
	catch (std::runtime_error&)
	{
		return false;
	}
}
