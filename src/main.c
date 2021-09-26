/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: main.c
 * Created Date: 25/09/2021 18:44:37
 * Last Modified: 26/09/2021 14:50:54
 * -----
 * Copyright (c) 2021
 */

#include "md4.h"
#include "sha1.h"
#include <stdio.h>
#include <string.h>

/*
 *  Define patterns for testing
 */
#define TEST1 "abc"
#define TEST2a "abcdbcdecdefdefgefghfghighijhi"
#define TEST2b "jkijkljklmklmnlmnomnopnopq"
#define TEST2 TEST2a TEST2b
#define TEST3 "a"
#define TEST4a "01234567012345670123456701234567"
#define TEST4b "01234567012345670123456701234567"
/* an exact multiple of 512 bits */
#define TEST4 TEST4a TEST4b

char *SHA1testarray[4] = {TEST1, TEST2, TEST3, TEST4};
long int SHA1repeatcount[4] = {1, 1, 1000000, 10};
char *SHA1resultarray[4] = {
    "A9 99 3E 36 47 06 81 6A BA 3E 25 71 78 50 C2 6C 9C D0 D8 9D",
    "84 98 3E 44 1C 3B D2 6E BA AE 4A A1 F9 51 29 E5 E5 46 70 F1",
    "34 AA 97 3C D4 C4 DA A4 F6 1E EB 2B DB AD 27 31 65 34 01 6F",
    "DE A3 56 A2 CD DD 90 C7 A7 EC ED C5 EB B5 63 93 4F 46 04 52"};

char *MD4testarray[4] = {"", TEST3, TEST1,
                         "12345678901234567890123456789012345678901234567890123"
                         "456789012345678901234567890"};
long int MD4repeatcount[4] = {1, 1, 1, 1};
char *MD4resultarray[4] = {"31 D6 CF E0 D1 6A E9 31 B7 3C 59 D7 E0 C0 89 C0",
                           "BD E5 2C B3 1D E3 3E 46 24 5E 05 FB DB D6 FB 24",
                           "A4 48 01 7A AF 21 D8 52 5F C1 0A E8 7A A6 72 9D",
                           "E3 3B 4D DC 9C 38 F2 19 9C 3E 7B 16 4F CC 05 36"};

void sha1() {
  SHA1Context sha;
  int i, j, err;
  uint8_t Message_Digest[SHA1HashSize];

  /*
   *  Perform SHA-1 tests
   */
  for (j = 0; j < 4; ++j) {
    printf("\nTest %d: %ld, '%s'\n", j + 1, SHA1repeatcount[j],
           SHA1testarray[j]);

    err = SHA1Reset(&sha);
    if (err) {
      fprintf(stderr, "SHA1Reset Error %d.\n", err);
      break; /* out of for j loop */
    }

    for (i = 0; i < SHA1repeatcount[j]; ++i) {
      err = SHA1Update(&sha, (const unsigned char *)SHA1testarray[j],
                       strlen(SHA1testarray[j]));
      if (err) {
        fprintf(stderr, "SHA1Update Error %d.\n", err);
        break; /* out of for i loop */
      }
    }

    err = SHA1Result(&sha, Message_Digest);
    if (err) {
      fprintf(stderr,
              "SHA1Result Error %d, could not compute message digest.\n", err);
    } else {
      printf("\t");
      for (i = 0; i < SHA1HashSize; ++i) {
        printf("%02X ", Message_Digest[i]);
      }
      printf("\n");
    }
    printf("Should match:\n");
    printf("\t%s\n", SHA1resultarray[j]);
  }

  /* Test some error returns */
  err = SHA1Update(&sha, (const unsigned char *)SHA1testarray[1], 1);
  printf("\nError %d. Should be %d.\n", err, StateError);
  err = SHA1Reset(0);
  printf("\nError %d. Should be %d.\n", err, Null);
}

int main() {
  printf("SHA1 Test Vectors");
  sha1();
  return 0;
}