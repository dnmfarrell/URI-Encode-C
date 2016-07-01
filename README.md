encode_uri
==========
An optimized C library for percent encoding/decoding text

Description
-----------
This is a URI encoder/decoder written in C based on [RFC3986](https://tools.ietf.org/html/rfc3986).
This module always encodes characters that are not unreserved. When decoding,
invalid escape sequences are preserved.

The code is optimized for speed and has a reasonable test suite.

Synopsis
--------

    /* encode text */
    const char *uri = "Some data!That Needs Encoding/";
    size_t len = strlen(uri);
    char buffer[ calc_buffer_size(uri) ];
    buffer[0] = '\0';
    uri_encode(uri, len, buffer);

    /* decode test */
    const char *encoded_uri = "Some%20data%21That%20Needs%20Decoding%2F";
    size_t len = strlen(encoded_uri);
    char decoded_uri[ len + 1 ];
    decoded_uri[0] = '\0';
    uri_decode(encoded_uri, len, decoded_uri);

Testing
-------

    clone https://github.com/dnmfarrell/Encode-URI-C.git
    cd Encode-URI-C
    make test
    ./run-tests

See Also
--------
[URI-Encode-XS](https://github.com/dnmfarrell/URI-Encode-XS) is a Perl XS module
that uses the same C code.

Authors
-------
&copy; 2016

* [David Farrell](https://github.com/dnmfarrell)
* [Aristotle Pagaltzis](https://github.com/ap)
* [Christian Hansen](https://github.com/chansen)
* [Jesse DuMond](https://github.com/JesseCanary)

Version
-------
0.02

License
-------
See LICENSE
