#pragma once
#include <string>

namespace GhettoCipher
{
	/** This class is a wrapper to make working with the GhettoCipher super easy with a python-like syntax
	*/
	class GhettoCryptWrapper
	{
	public:
		//! Will encrypt a string and return it hexadecimally encoded.
		static std::string EncryptString(const std::string& cleartext, const std::string& password);

		//! Will decrypt a hexadecimally encoded string.
		static std::string DecryptString(const std::string& ciphertext, const std::string& password);

		//! Will encrypt a file.
		//! Returns false if anything goes wrong (like, file-access).
		//! @filename_in The file to be read.
		//! @filename_out The file the encrypted version should be saved in.
		static bool EncryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password);

		//! Will decrypt a file.
		//! Returns false if anything goes wrong (like, file-access).
		//! @filename_in The file to be read.
		//! @filename_out The file the decrypted version should be saved in.
		static bool DecryptFile(const std::string& filename_in, const std::string& filename_out, const std::string& password);

	private:
		// No instanciation! >:(
		GhettoCryptWrapper();
	};
}
