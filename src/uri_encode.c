#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 uri_encode.c - functions for URI percent encoding / decoding
*/

//const char *reserved_chars = "!*'();:@&=+$,/?#[]%";

const char *unreserved_chars
  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";

// calc a large enough buffer to store encoding
int calc_buffer_size (char *uri)
{
  int buffer_size = strlen(uri) * 3 + 1;
  return buffer_size;
}

// generic percent-encoding algorithm
char *encode (char *uri, const char *special_chars, char *buffer)
{
  int i = 0;
  while (uri[i] != '\0')
  {
    int encode_char = 1;

    int j = 0;
    while (special_chars[j] != '\0')
    {
      if (uri[i] == special_chars[j])
      {
        // do not encode char as it is in the special_chars set
        encode_char = 0;
        break;
      }
      j++;
    }
    if (encode_char == 1)
    {
      char code[4];
      sprintf(code, "%%%02X", uri[i]);
      strcat(buffer, code);
    }
    else
    {
      char code[2];
      code[0] = uri[i];
      code[1] = '\0';
      strcat(buffer, code);
    }
    i++;
  }
  return buffer;
}

char *decode (char *uri, char *buffer)
{
  int i = 0;

  while(uri[i] != '\0')
  {
    if(uri[i] == '%')
    {
      i++;
      char code[3];
      code[0] = uri[i++];
      code[1] = uri[i++];
      code[2] = '\0';

      int decimal = (int)strtol(code, NULL, 16);

      char decode[2];
      decode[0] = decimal;
      decode[1] = '\0';
      strcat(buffer, decode);
    }
    else
    {
      char code[2];
      code[0] = uri[i++];
      code[1] = '\0';
      strcat(buffer, code);
    }
  }
  return buffer;
}

char *uri_encode (char *uri, char *buffer)
{
  return encode(uri, unreserved_chars, buffer);
}

char *uri_decode (char *uri, char *buffer)
{
  return decode(uri, buffer);
}

#ifdef Testing

#include "minunit.h"

int tests_run = 0;

static char * test_uri_encode(char *uri, char *expected) {
  char buffer[ calc_buffer_size(uri) ];
  buffer[0] = '\0';
  uri_encode(uri, buffer);
  int match = strcmp(expected, buffer);
  printf("Got: %s Expected: %s\n", buffer, expected);
  mu_assert("Strings don't match", match == 0);
  return 0;
}

static char * test_uri_decode(char *uri, char *expected) {
  char buffer[ strlen(uri) + 1 ];
  buffer[0] = '\0';
  uri_decode(uri, buffer);
  int match = strcmp(expected, buffer);
  printf("Got: %s Expected: %s\n", buffer, expected);
  mu_assert("Strings don't match", match == 0);
  return 0;
}

static char * test_encode() {
  char * msg = test_uri_encode("ABCabc !;/","ABCabc%20%21%3B%2F");
  return msg ? msg : 0;
}

static char * test_decode() {
  char * msg = test_uri_decode("ABCabc%20%21%3B%2F","ABCabc !;/");
  return msg ? msg : 0;
}

static char * all_tests() {
   mu_run_test(test_encode);
   mu_run_test(test_decode);
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
