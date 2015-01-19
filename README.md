These are tools I've written to practice breaking ciphers.

_CrappyCrypto_ contains my own versions of cryptographic algorithms that I have written to attack.
They are not intentionally insecure, but I am not confident in stating that they are hardened against
modern attacks. I have had security training in the past, have spent a number of years building threat
models and fixing security issues, and have studied cryptography since 1991. I am quite confident in
not being confident.

I first wrote the [Skipjack](http://en.wikipedia.org/wiki/Skipjack_\(cipher\)) code in 1998.
Surprisingly, it built without errors in Visual Studio 2010. I have since tried to modernize it
to modern C++ in VS2013.

_CrappyCrypto_ has a dependency on the _PortableRuntime_ library.

Toby Jones \([www.turbohex.com](http://www.turbohex.com), [ace.roqs.net](http://ace.roqs.net)\)
