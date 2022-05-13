# GCrypt CLI
Easy text and file encryption via the command line using Ghetto Crypt. Now supporting [*esoteric data formats*](#esoteric-data-formats)... :)  
Again, please only use this as an obfuscator or if the only other option would be no encryption at all.
Do you want to quickly and securely encrypt stuff via the command line? Use openssl-cli with the aes cipher. It's a bit more wordy but much faster and more secure.

Still want to use Ghetto Crypt cli? Here ya go!
Just clone this repository, navigate into this directory and run `cmake . && make`. It shouuuld *\*crosses fingers\** just build.

If you want to use this globally, you could move it to `/usr/bin/`, or some other location in your $PATH.

## How do i use this?
All arguments and flags:
```
$ gecrypt --help
CLI for the ghettocrypt cipher/obfuscator
Copyright (c) 2022 Leon Etienne
Ghettocrypt v0.21
Ghettocrypt CLI v0.124
THIS IS EXPERIMENTAL SOFTWARE AND MUST BE CONSIDERED INSECURE. DO NOT USE THIS TO ENCRYPT SENSITIVE DATA! READ THE README FILES ACCESSIBLE AT "https://github.com/Leonetienne/GhettoCrypt/blob/master/readme.md" AND "https://github.com/Leonetienne/GhettoCrypt/blob/master/GhettoCryptCLI/readme.md"

==== AVAILABLE PARAMETERS ====

--iobase-8   VOID   incompatibilities=[--iobase-2, --iobase-10, --iobase-64, --iobase-uwu, --iobase-ugh]   Interpret and format ciphertexts in base8

--progress   -p   VOID   Print digestion progress to stdout. May be advisable for large files, as the cipher is rather slow.

--cli-version   VOID   Will supply the version of ghettocrypt-cli used.

--keyfile   -kf   STRING   incompatibilities=[--key, --keyask, --hash]   Read in the first {KEYSIZE}(=512) bits of this file and use that as an encryption key. WARNING: Arguments may be logged by the system!

--iobase-2   VOID   incompatibilities=[--iobase-8, --iobase-10, --iobase-64, --iobase-uwu, --iobase-ugh]   Interpret and format ciphertexts in base2

--iobase-10   VOID   incompatibilities=[--iobase-2, --iobase-8, --iobase-64, --iobase-uwu, --iobase-ugh]   Interpret and format ciphertexts in base10

--iobase-ugh   VOID   incompatibilities=[--iobase-2, --iobase-8, --iobase-10, --iobase-64, --iobase-uwu]   Interpret and format ciphertexts in base ugh

--iobase-uwu   VOID   incompatibilities=[--iobase-2, --iobase-8, --iobase-10, --iobase-64, --iobase-ugh]   Interpret and format ciphertexts in base uwu

--iobase-64   VOID   incompatibilities=[--iobase-2, --iobase-8, --iobase-10, --iobase-uwu, --iobase-ugh]   Interpret and format ciphertexts in base64

--ostdout   VOID   incompatibilities=[--ofile, --hash]   Output of digested files will be dumped to stdout instead of a file.

--encrypt   -e   VOID   incompatibilities=[--decrypt, --hash]   Use the encryption routine.

--infile   -if   STRING   incompatibilities=[--intext]   Encrypt this file. Saves as {filename}.crypt, if not specified otherwise.

--version   -v   VOID   Will supply the version of ghettocrypt used.

--decrypt   -d   VOID   incompatibilities=[--encrypt, --hash]   Use decryption routine.

--hash   -h   VOID   incompatibilities=[--encrypt, --decrypt]   Use the ghetto cipher as a hash digest.

--intext   -it   STRING   incompatibilities=[--infile]   Encrypt this string. Dumps to stdout.

--ofile   -o   STRING   incompatibilities=[--ostdout, --hash]   Use this filename for output if --infile is specified. Gets ignored otherwise.

--keyask   -ka   VOID   incompatibilities=[--key, --keyfile, --hash]   Read the encryption key from stdin.

--key   -k   STRING   incompatibilities=[--keyfile, --keyask, --hash]   Use this value as a password to extrapolate the encryption key. WARNING: Arguments may be logged by the system!
```

###  Examples
Please note that commonly used arguments are supplied in their short form (`-e` in place of `--encrypt`).

#### I want to encrypt text!
```sh
$ gecrypt -e --keyask --intext "hello, world!"
efbebc429c8370bf84f00b0d8ccbaf7858b3b87d71ff58cb1cfefa8fb0c68094c0865565873aa8a5254ede59be46e81a4d4917e679b18cb290dbd6669cb6207a
```

#### Now decrypt it
```sh
$ gecrypt -d --keyask --intext "efbebc429c8370bf84f00b0d8ccbaf7858b3b87d71ff58cb1cfefa8fb0c68094c0865565873aa8a5254ede59be46e81a4d4917e679b18cb290dbd6669cb6207a"
hello, world!
```

#### What about not using hex?
> :warning: Custom bases are super imperformant. 
> Encrypting all text in main.cpp took about two seconds.
> Outputting it in base-64 took just over ONE MINUTE. In base-2 over SEVEN MINUTES. The general trend seems to be, the larger the base, the better it performs.

```sh
$ gecrypt -e --keyask --intext "hello, world!" --iobase-2
111001001011100000011111000100010100110011100110000100100101001001110001001000101011110000000011011100001010111010001000110111110110011011100000001100110001001000100111011000101010010001011011111011001000011111100100101001011110011101110001010011000101011001111010000001001100101110000101101101101001110100100001101010111101010000100111101110000110011101100101101011000011101001000011010010011001111010001001101000101001100101010000100010111101101100010000000001001110010001001011001001011011010111001101000100

$ gecrypt -e --keyask --intext "hello, world!" --iobase-8
71134037042463460445116110536003341272106766334014611047305221337310374451363561230531720114560555516441527520475606354553035103223172115051452042755420011621131133271504

$ gecrypt -e --keyask --intext "hello, world!" --iobase-10
2994749439449970047518881970731547473115480925772565399786126223459744370490386223437520234266936877059618216185983047971748564015130703048737306773910340

$ gecrypt -e --keyask --intext "hello, world!" --iobase-64
Co/WjpV5nPrCaz0QMdrXAXzzOH5HODRsBNL22KZowmGMcTLwfmsQpzt7Ik+ViR5vOhUXowFQeR5x2vbcj1X5ae
```
I won't be pasting in stdout for every example. It would become too cluttered.

#### Passing the key as an argument
```sh
$ gecrypt -e --key "secretpassword" --intext "hello, world!"
```
> :warning: Some operating systems will log cli arguments! THIS WOULD BE THE ENTIRE KEY!

#### Using keyfiles
```sh
$ gecrypt -e --keyfile "dog.jpg" --intext "hello, world!"
```
> :warning: Some operating systems will log cli arguments! One might find your keyfile!

#### Encrypting files
```sh
$ gecrypt -e --keyask --infile "cat.jpg"
```
File `cat.jpg.crypt` will be created.

#### Encrypting files to a target file name
```sh
$ gecrypt -e --keyask --infile "cat.jpg" -o "encrypted_cat.jpg"
```
File `encrypted_cat.jpg` will be created.

#### Decrypting files
```sh
$ gecrypt -d --keyask --infile "cat.jpg.crypt"
```
File `cat.jpg.crypt.plain` will be created. Its contents match `cat.jpg`  
> :warning: Since this is a block cipher, decrypted files may be tailpadded with a few nullbytes.

#### Decrypting files to a target file name
```sh
$ gecrypt -d --keyask --infile "cat.jpg.crypt" -o "decrypted_cat.jpg"
```
File `decrypted_cat.jpg` will be created. You can now open it again.

#### Encrypting large files takes time. How's the progress?
```sh
$ gecrypt -e --keyask --infile "cat.jpg" --progress
```
Something along the lines of `Encrypting... (Block 200 / 1148 - 17.4216%)` will be regularly, but not too often, printed to stdout.

#### Any cipher can also compute hashsums
```sh
$ gecrypt -h --intext "hello, world!"
a96f42c9d97e46b9e1ed7de5182770170d4ef9b7b8264f3fbd89b38dc60c1fe06232653f5856013307fc020fb1d35f2bea26bc0f373c5ac35a722c6b03d8254d

$ gecrypt -h --infile "cat.jpg"
fe6bdfb6ec39771c4fdcdc40e52397bcd67fbfef0ad5a15ebbd8b9e4c2a815848b3984eda5ef6f727e9e420c23500c90c42ab80ac5659048be8969357741e3e5

```
The hashsum will always be of size BLOCK_SIZE. That is 512 bits by default.  
> :warning: Calculating a hashsum takes about twice as long as regular encryption.

#### What version am i running?
Depending on wether you want to know the GhettoCrypt version or the CLI's version, use either `--version` or `--cli-version`. It will print out a floating point number.

#### I want to stream the output of file en/decryption.
You can dump the binary result to stdout. In this example, we'll stream it into a jpg file,
but you could stream it to whatever you'd like.
```sh
$ gecrypt -d --keyask --infile "cat.jpg.crypt" --ostdout > "decrypted_cat.jpg"
```

## Esoteric data formats
#### Base *UwU*
```sh
$ gecrypt -e --keyask --intext "hello, world!" --iobase-uwu
:) sewnpaiii tastieee uhh?! nappies cutewr twe best cutieee :O tastieee senpaiiiw favowite toesy-woesies ^.^ :3 best chomp whiffle uwu Awww sewnpaiii comfy-womfy :p keewl Awww youuu nyeko :O tasties hiiiii heeeey (*^_^*) youuu toot uhh..? smush (*^_^*) *bites-lip* whiffle haaaay nyah! comfy-womfy :) cutsie Owww haaaay snaffle haaaai haaaai nyeko *sweats* :) uhh..? boop toot *bites-lip* <3 whiiiich whiffskaws ^.^ twe whiffskaws hiiiii *sweats* Owww dewicious i tasties :P awe hewwo boop rawr uwu dewicious eughh twe cutsie xD
```

#### Base **UGH!**
```sh
$ gecrypt -e --keyask --intext "hello, world!" --iobase-ugh
Grr... Wha-? Aah! Aah! Uh-huh... Aah! Grr... Aah! Aah! Uh-huh... Ah... Ugh Grr... Ugh Pft! Nu-uh... Gah! Bah! Huh...? Ah... Uh-huh... Wha-? Pft! Nu-uh... Ugh Wha-? Psh! Agh! Ah... Aah! Nu-uh... Psh! Pft! Nu-uh... Psh! Shh! Gah! Ah... Pft! Gah! Shh! Bah! Gah! Uh-huh... Gah! Duh! Aah! Uh-huh... Er- Nu-uh... Gah! Wha-? Pft! Er- Shh! Ah... Huh...? Er- Wha-? Uh-huh... Ah... Shh! Ugh Bah! Wha-? Uaah! Ah... Nu-uh... Uh-huh... Ugh Pft! Pft! Gah! Shh! Shh! Wha-? Bah! Ugh Grr... Aah! Pft! Nu-uh... Ah... Aah! Agh! Er- Psh! Uaah! Nu-uh... Ugh Wha-? Uh-huh... Shh! Pft! Aah! Agh! Grr... Agh! Agh! Grr... Pft! Wha-? Wha-? Uh-huh... Aah! Ugh Aah! Pft! Gah! Bah! Huh...? Ugh Bah! Uaah! Gah! Bah! Duh! Duh! Uh-huh... Grr... Ah... Grr... Ugh Ah... Pft!
```

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
