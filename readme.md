# Ghettocrypt
*because frankly i have no idea what the fuck i'm doing*  
<sub>*Please don't use this for anything critical*</sub>

## What the hell is this?
An educational project on implementing a block cipher using a feistel network.  
To provide at least some security this is using some DES-inspired modes of operation like *cipher block chaining*.
This way this provides relatively good diffusion.

## Features
* It has very easy syntax
* It's slow
* It's probably super insecure
* It leaves your keys sprinkled in ram✨
* 512-bit keys <sup>\*</sup>
* But the syntax is pythonlike easy🙇  

It's pretty ghetto, you know?

## What are the actual advantages?
* It's two files to import into your project
* 1 Line to use
* 100% cross plattform

## What could I use it for?
* For data obfuscation
* If your only other option would be no encryption at all

### I am not kidding, don't use this for critical stuff! Homebrew ciphers tend to be shit!
Especially mine!🗡️

## How do I use this?
### *"I don't care about the library. Just let me use it from the command line!"*
There is a CLI version availabile [here](https://github.com/Leonetienne/GhettoCrypt/tree/master/GhettoCryptCLI).

### Installation as a library
Download the `.h` and `.cpp` file from [INCLUDE/](https://github.com/Leonetienne/GhettoCrypt/tree/master/INCLUDE) and add them to your projects files. *Single-header-magic*.

### Working with strings
```c++
using namespace GhettoCipher;

// Get some string
const std::string input = "I am a super secret message!";
std::cout << input << std::endl;

// Encrypt
const std::string encrypted = GhettoCryptWrapper::EncryptString(input, "password1");
std::cout << encrypted << std::endl;

// Decrypt
const std::string decrypted = GhettoCryptWrapper::DecryptString(encrypted, "password1");
std::cout << decrypted << std::endl;
```

### Working with files
```c++
using namespace GhettoCipher;

// Encrypt
GhettoCryptWrapper::EncryptFile("main.cpp", "main.cpp.crypt", "password1");

// Decrypt
GhettoCryptWrapper::DecryptFile("main.cpp.crypt", "main.cpp.clear", "password1");
```

If you want to do more complex stuff, use the cipher-class [`GhettoCipher::Cipher`](https://github.com/Leonetienne/GhettoCrypt/blob/master/GhettoCrypt/Cipher.h) aswell as the conversion methods in [Util.h](https://github.com/Leonetienne/GhettoCrypt/blob/master/GhettoCrypt/Util.h). This way you can cipher on bitlevel. Examples on how to do this are in [GhettoCryptWrapper.cpp](https://github.com/Leonetienne/GhettoCrypt/blob/master/GhettoCrypt/GhettoCryptWrapper.cpp).
This way you could, for example, decrypt an ecrypted file directly into memory. Or use a full-length key instead of a password.
Without saying, this is more advanced and not as-easy as the methods supplied in the wrapper.

---
<sup>\*</sup> A key is always of size `BLOCK_SIZE`. The default block size is 512 (bit), but you can easily change it in [Config.h](https://github.com/Leonetienne/GhettoCrypt/blob/master/GhettoCrypt/Config.h) or wherever it'll be put in the INCLUDE/*.cpp. `BLOCK_SIZE` is also the minimal output length!

## LICENSE
```
BSD 2-Clause License

Copyright (c) 2021, Leon Etienne
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
