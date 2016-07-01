#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

/*
 uri_encode.c - functions for URI percent encoding / decoding
*/

#define _______ "\0\0\0\0"
static const char uri_encode_tbl[ sizeof(int32_t) * 0x100 ] = {
/*  0       1       2       3       4       5       6       7       8       9       a       b       c       d       e       f                        */
    "%00\0" "%01\0" "%02\0" "%03\0" "%04\0" "%05\0" "%06\0" "%07\0" "%08\0" "%09\0" "%0A\0" "%0B\0" "%0C\0" "%0D\0" "%0E\0" "%0F\0"  /* 0:   0 ~  15 */
    "%10\0" "%11\0" "%12\0" "%13\0" "%14\0" "%15\0" "%16\0" "%17\0" "%18\0" "%19\0" "%1A\0" "%1B\0" "%1C\0" "%1D\0" "%1E\0" "%1F\0"  /* 1:  16 ~  31 */
    "%20\0" "%21\0" "%22\0" "%23\0" "%24\0" "%25\0" "%26\0" "%27\0" "%28\0" "%29\0" "%2A\0" "%2B\0" "%2C\0" _______ _______ "%2F\0"  /* 2:  32 ~  47 */
    _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ "%3A\0" "%3B\0" "%3C\0" "%3D\0" "%3E\0" "%3F\0"  /* 3:  48 ~  63 */
    "%40\0" _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______  /* 4:  64 ~  79 */
    _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ "%5B\0" "%5C\0" "%5D\0" "%5E\0" _______  /* 5:  80 ~  95 */
    "%60\0" _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______  /* 6:  96 ~ 111 */
    _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ _______ "%7B\0" "%7C\0" "%7D\0" _______ "%7F\0"  /* 7: 112 ~ 127 */
    "%80\0" "%81\0" "%82\0" "%83\0" "%84\0" "%85\0" "%86\0" "%87\0" "%88\0" "%89\0" "%8A\0" "%8B\0" "%8C\0" "%8D\0" "%8E\0" "%8F\0"  /* 8: 128 ~ 143 */
    "%90\0" "%91\0" "%92\0" "%93\0" "%94\0" "%95\0" "%96\0" "%97\0" "%98\0" "%99\0" "%9A\0" "%9B\0" "%9C\0" "%9D\0" "%9E\0" "%9F\0"  /* 9: 144 ~ 159 */
    "%A0\0" "%A1\0" "%A2\0" "%A3\0" "%A4\0" "%A5\0" "%A6\0" "%A7\0" "%A8\0" "%A9\0" "%AA\0" "%AB\0" "%AC\0" "%AD\0" "%AE\0" "%AF\0"  /* A: 160 ~ 175 */
    "%B0\0" "%B1\0" "%B2\0" "%B3\0" "%B4\0" "%B5\0" "%B6\0" "%B7\0" "%B8\0" "%B9\0" "%BA\0" "%BB\0" "%BC\0" "%BD\0" "%BE\0" "%BF\0"  /* B: 176 ~ 191 */
    "%C0\0" "%C1\0" "%C2\0" "%C3\0" "%C4\0" "%C5\0" "%C6\0" "%C7\0" "%C8\0" "%C9\0" "%CA\0" "%CB\0" "%CC\0" "%CD\0" "%CE\0" "%CF\0"  /* C: 192 ~ 207 */
    "%D0\0" "%D1\0" "%D2\0" "%D3\0" "%D4\0" "%D5\0" "%D6\0" "%D7\0" "%D8\0" "%D9\0" "%DA\0" "%DB\0" "%DC\0" "%DD\0" "%DE\0" "%DF\0"  /* D: 208 ~ 223 */
    "%E0\0" "%E1\0" "%E2\0" "%E3\0" "%E4\0" "%E5\0" "%E6\0" "%E7\0" "%E8\0" "%E9\0" "%EA\0" "%EB\0" "%EC\0" "%ED\0" "%EE\0" "%EF\0"  /* E: 224 ~ 239 */
    "%F0\0" "%F1\0" "%F2\0" "%F3\0" "%F4\0" "%F5\0" "%F6\0" "%F7\0" "%F8\0" "%F9\0" "%FA\0" "%FB\0" "%FC\0" "%FD\0" "%FE\0" "%FF"    /* F: 240 ~ 255 */
};
#undef _______

#define __ 0xFF
static const unsigned char hexval[0x100] = {
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 00-0F */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 10-1F */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 20-2F */
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,__,__,__,__,__,__, /* 30-3F */
  __,10,11,12,13,14,15,__,__,__,__,__,__,__,__,__, /* 40-4F */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 50-5F */
  __,10,11,12,13,14,15,__,__,__,__,__,__,__,__,__, /* 60-6F */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 70-7F */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 80-8F */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 90-9F */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* A0-AF */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* B0-BF */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* C0-CF */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* D0-DF */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* E0-EF */
  __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* F0-FF */
};
#undef __

size_t uri_encode (const char *src, const size_t len, char *dst)
{
  size_t i = 0, j = 0;
  while (i < len)
  {
    const char octet = src[i++];
    const int32_t code = ((int32_t*)uri_encode_tbl)[ (unsigned char)octet ];
    if (code) {
        *((int32_t*)&dst[j]) = code;
        j += 3;
    }
    else dst[j++] = octet;
  }
  dst[j] = '\0';
  return j;
}

size_t uri_decode (const char *src, const size_t len, char *dst)
{
  size_t i = 0, j = 0;
  while(i < len)
  {
    int copy_char = 1;
    if(src[i] == '%' && i + 2 < len)
    {
      const unsigned char v1 = hexval[ (unsigned char)src[i+1] ];
      const unsigned char v2 = hexval[ (unsigned char)src[i+2] ];

      /* skip invalid hex sequences */
      if ((v1 | v2) != 0xFF)
      {
        dst[j] = (v1 << 4) | v2;
        j++;
        i += 3;
        copy_char = 0;
      }
    }
    if (copy_char)
    {
      dst[j] = src[i];
      i++;
      j++;
    }
  }
  dst[j] = '\0';
  return j;
}

#ifdef Testing

#include "minunit.h"

int tests_run = 0;

static char * test_uri_encode(const char *uri, const char *expected) {
  const size_t len = strlen(uri);
  char buffer[ len * 3 + 1 ];
  buffer[0] = '\0';
  uri_encode(uri, len,  buffer);
  const size_t match = strcmp(expected, buffer);
  printf("uri_encode() got: \"%s\" expected: \"%s\"\n", buffer, expected);
  mu_assert("Strings don't match", match == 0);
  return 0;
}

static char * test_uri_decode(char *uri, char *expected) {
  const size_t len = strlen(uri);
  char buffer[ len + 1 ];
  buffer[0] = '\0';
  uri_decode(uri, len, buffer);
  const size_t match = strcmp(expected, buffer);
  printf("uri_decode() got: \"%s\" expected: \"%s\"\n", buffer, expected);
  mu_assert("Strings don't match", match == 0);
  return 0;
}

/* tests for encode_uri */
static char * test_encode_empty() {
  char * msg = test_uri_encode("","");
  return msg ? msg : 0;
}
static char * test_encode_something() {
  char * msg = test_uri_encode("something","something");
  return msg ? msg : 0;
}
static char * test_encode_space() {
  char * msg = test_uri_encode(" ","%20");
  return msg ? msg : 0;
}
static char * test_encode_percent() {
  char * msg = test_uri_encode("%%20","%25%2520");
  return msg ? msg : 0;
}
static char * test_encode_latin1() {
  char * msg = test_uri_encode("|abcå", "%7Cabc%C3%A5");
  return msg ? msg : 0;
}
static char * test_encode_symbols() {
  char * msg = test_uri_encode("~*'()", "~%2A%27%28%29");
  return msg ? msg : 0;
}
static char * test_encode_angles() {
  char * msg = test_uri_encode("<\">", "%3C%22%3E");
  return msg ? msg : 0;
}
static char * test_encode_middle_null() {
  char * msg = test_uri_encode("ABC\0DEF", "ABC");
  return msg ? msg : 0;
}
static char * test_encode_latin1_utf8() {
  char * msg = test_uri_encode("åäö", "%C3%A5%C3%A4%C3%B6");
  return msg ? msg : 0;
}
static char * test_encode_utf8() {
  char * msg = test_uri_encode("❤","%E2%9D%A4");
  return msg ? msg : 0;
}

/* tests for decode_uri */
static char * test_decode_empty() {
  char * msg = test_uri_decode("","");
  return msg ? msg : 0;
}
static char * test_decode_something() {
  char * msg = test_uri_decode("something","something");
  return msg ? msg : 0;
}
static char * test_decode_something_percent() {
  char * msg = test_uri_decode("something%", "something%");
  return msg ? msg : 0;
}
static char * test_decode_something_percenta() {
  char * msg = test_uri_decode("something%a", "something%a");
  return msg ? msg : 0;
}
static char * test_decode_something_zslash() {
  char * msg = test_uri_decode("something%Z/", "something%Z/");
  return msg ? msg : 0;
}
static char * test_decode_space() {
  char * msg = test_uri_decode("%20", " ");
  return msg ? msg : 0;
}
static char * test_decode_percents() {
  char * msg = test_uri_decode("%25%2520", "%%20");
  return msg ? msg : 0;
}
static char * test_decode_latin1() {
  char * msg = test_uri_decode("%7Cabc%C3%A5", "|abcå");
  return msg ? msg : 0;
}
static char * test_decode_symbols() {
  char * msg = test_uri_decode("~%2A%27%28%29", "~*'()");
  return msg ? msg : 0;
}
static char * test_decode_angles() {
  char * msg = test_uri_decode("%3C%22%3E", "<\">");
  return msg ? msg : 0;
}
static char * test_decode_middle_null() {
  char * msg = test_uri_decode("ABC%00DEF", "ABC\0");
  return msg ? msg : 0;
}

static char * all_tests() {
  mu_run_test(test_encode_empty);
  mu_run_test(test_encode_something);
  mu_run_test(test_encode_percent);
  mu_run_test(test_encode_space);
  mu_run_test(test_encode_empty);
  mu_run_test(test_encode_latin1);
  mu_run_test(test_encode_symbols);
  mu_run_test(test_encode_angles);
  mu_run_test(test_encode_middle_null);
  mu_run_test(test_encode_latin1_utf8);
  mu_run_test(test_encode_utf8);
  mu_run_test(test_decode_empty);
  mu_run_test(test_decode_something);
  mu_run_test(test_decode_something_percent);
  mu_run_test(test_decode_something_percenta);
  mu_run_test(test_decode_something_zslash);
  mu_run_test(test_decode_space);
  mu_run_test(test_decode_percents);
  mu_run_test(test_decode_latin1);
  mu_run_test(test_decode_symbols);
  mu_run_test(test_decode_angles);
  mu_run_test(test_decode_middle_null);
  return 0;
}

int main(int argc, char **argv) {
   char *result = all_tests();
   if (result != 0) {
       printf("%s\n", result);
   }
   else {
       printf("ALL TESTS PASSED\n");
   }
   printf("Tests run: %d\n", tests_run);

   return result != 0;
}

#endif
