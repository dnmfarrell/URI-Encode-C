encode_uri
==========
A C library for percent encoding/decoding text

Synopsis
--------

    // encode
    char * uri = "Some data!That Needs Encoding/";
    char buffer[ calc_buffer_size(uri) ];
    buffer[0] = '\0';
    uri_encode(uri, buffer);

    // decode
    char * encoded_string = "Some%20data%21That%20Needs%20Decoding%2F";
    char decoded_string[ strlen(encoded_string) + 1 ];
    decoded_string[0] = '\0';
    uri_decode(encoded_string, decoded_string);

Testing
-------

    clone https://github.com/dnmfarrell/Encode-URI-C.git
    cd Encode-URI-C
    make
    ./encode_uri

Author
------
&copy; 2016 David Farrell

License
-------
See LICENSE
