These are tools I've written to practice breaking ciphers.

_CrappyCrypto_ contains my own versions of cryptographic algorithms that I have written to attack.
They are not intentionally insecure, but I am not confident in stating that they are hardened against
modern attacks. I have had security training in the past, have spent a number of years building threat
models and fixing security issues, and have studied cryptography since 1991. I am quite confident in
not being confident.

I first wrote the [Skipjack](http://en.wikipedia.org/wiki/Skipjack_\(cipher\)) code in 1998.
Surprisingly, it built without errors in Visual Studio 2010. I have since tried to modernize it
to modern C++ in VS2013.

Caveat: This code is portable, but due to variances in how arguments are passed to `main()` in C++, encoding
matters.  For example, by default, file names are passed as ANSI (in the current code page) to `main()` in
Windows, and passed directly to `fopen()`.  Linux will use UTF-8 by default.  In general this will do what is
expected.  However, keys are different.  Since _CrappyCrypto_ operates directly on the `argv` data,
non-ASCII keys will cause problems.  For example, on Windows, the Euro '€' will be passed as a two byte
ANSI character to `main()`, and the key built from that.  On Linux, the UTF-8 encoding of U+20AC is passed,
which is three bytes.  The result is that data encoded on one platform might not be correctly decodable
on another without some care.

UTF-8 correctness is something that I am actively thinking about, and have an idea on the general
portable solution for the above issue, but I want to reflect on whether it is the best approach for all
of my projects before I commit to it.

_CrappyCrypto_ has a dependency on the _PortableRuntime_ library.

Toby Jones \([www.turbohex.com](http://www.turbohex.com), [ace.roqs.net](http://ace.roqs.net)\)
