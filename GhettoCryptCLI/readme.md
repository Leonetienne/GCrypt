# GhettoCrypt CLI
Easy text and file encryption via the command line using Ghetto Crypt.
Again, please only use this as an obfuscator or if the only other option would be no encryption at all.
Do you want to quickly and securely encrypt stuff via the command line? Use openssl-cli with the aes cipher. It's a bit more wordy but much faster and more secure.

Still want to use Ghetto Crypt cli? Here ya go!
Either download the latest [release](https://github.com/Leonetienne/GhettoCrypt/releases), or directly from here: [gecrypt](https://github.com/Leonetienne/GhettoCrypt/blob/master/GhettoCryptCLI/gecrypt).

If you want to use this globally, you could move it to `/usr/bin/`, or some other location in your $PATH.

## How do i use this?
All arguments and flags:
```
$ gecrypt --help
CLI for the ghettocrypt cipher/obfuscator
Copyright (c) 2022 Leon Etienne
Ghettocrypt v0.13
Ghettocrypt CLI v0.1
THIS IS EXPERIMENTAL SOFTWARE AND MUST BE CONSIDERED INSECURE. DO NOT USE THIS TO ENCRYPT SENSITIVE DATA! READ THE README FILE ACCESSIBLE AT "https://github.com/Leonetienne/GhettoCrypt/blob/master/readme.md"

==== AVAILABLE PARAMETERS ====

--encrypt   -e   VOID   incompatibilities=[--decrypt]   Use the encryption routine.

--keyask   -ka   VOID   incompatibilities=[--key, --keyfile]   Read the encryption key from stdin.

--version   -v   VOID   Will supply the version of ghettocrypt used.

--decrypt   -d   VOID   incompatibilities=[--encrypt]   Use decryption routine.

--cli-version   VOID   Will supply the version of ghettocrypt-cli used.

--intext   -it   STRING   incompatibilities=[--infile]   Encrypt this string. Dumps to stdout.

--key   -k   STRING   incompatibilities=[--keyfile, --keyask]   Use this value as a password to extrapolate the encryption key. WARNING: Arguments may be logged by the system!

--infile   -if   STRING   incompatibilities=[--intext]   Encrypt this file. Saves as {filename}.crypt, if not specified otherwise.

--ofile   -of   STRING   incompatibilities=[--ostdout]   Use this filename for output if --infile is specified. Gets ignored otherwise.

--ostdout   VOID   incompatibilities=[--ofile]   Output of digested files will be dumped to stdout instead of a file.

--keyfile   -kf   STRING   incompatibilities=[--key, --keyask]   Read in the first {KEYSIZE}(=512) bits of this file and use that as an encryption key. WARNING: Arguments may be logged by the system!

--progress   -p   VOID   Print digestion progress to stdout. May be advisable for large files, as the cipher is rather slow.
```

###  Examples
Please note that commonly used arguments are supplied in their short form (`-e` in place of `--encrypt`).

#### I want to encrypt text!
```sh
$ gecrypt -e --keyask --intext "hello, world!"
```

#### Now decrypt it
```sh
$ gecrypt -d --keyask --intext "efbebc429c8370bf84f00b0d8ccbaf7858b3b87d71ff58cb1cfefa8fb0c68094c0865565873aa8a5254ede59be46e81a4d4917e679b18cb290dbd6669cb6207a"
```
I won't be pasting in complete ciphertexts anymore here. I will abbreviatre them as `efbebc...b6207a`. This way it just looks tidier.

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
File `cat.jpg.crypt.plain` will be created. It's contents match `cat.jpg`  
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

#### What version am i running?
Depending on wether you want to know the GhettoCrypt version or the CLI's version, use either `--version` or `--cli-version`. It will print out a floating point number.

#### I want to stream the output of file en/decryption.
You can dump the binary result to stdout. In this example, we'll stream it into a jpg file,
but you could stream it to whatever you'd like.
```sh
$ gecrypt -d --keyask --infile "cat.jpg.crypt" --ostdout > "decrypted_cat.jpg"
```
