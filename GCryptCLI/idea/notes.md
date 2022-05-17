* Add a new iobase: bytes.
  For cleartext:
    Format is ALWAYS raw bytes. That works fine with text, and files.
    The iobase ONLY affects ciphertext!

  For ciphertext:
    format depends..:

    If none specified:
      - raw bytes if an outputfile is given
      - hex if output to stdout

    If specified:
      Just use the specified iobase. Even if it means dumping bytes to stdout, or writing base-2 to files.


* --intext or --infile should no longer be required. Default behaviour should be reading from stdin.
* No guessing where to output (like file-in got saved to another file beforehand.) Default behaviour should be stdout.

* --progress should output reports to stderr. This way it won't disturb piping the ciphertext to another program.

* Add a module to generate keyfiles.
  Should seed a gcrypt prng with std::random_device, and then dump BLOCK_SIZE bits to the specified output.

