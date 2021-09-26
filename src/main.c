/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: main.c
 * Created Date: 25/09/2021 18:44:37
 * Last Modified: 26/09/2021 16:50:16
 * -----
 * Copyright (c) 2021
 */

#include "md4.h"
#include "rainbow.h"
#include "sha1.h"
#include <stdio.h>
#include <string.h>

char *SHA1_testarray[4] = {
    "", "abc", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjk"
    "lmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"};
char *SHA1_resultarray[4] = {
    "DA 39 A3 EE 5E 6B 4B 0D 32 55 BF EF 95 60 18 90 AF D8 07 09",
    "A9 99 3E 36 47 06 81 6A BA 3E 25 71 78 50 C2 6C 9C D0 D8 9D",
    "84 98 3E 44 1C 3B D2 6E BA AE 4A A1 F9 51 29 E5 E5 46 70 F1",
    "A4 9B 24 46 A0 2C 64 5B F4 19 F9 95 B6 70 91 25 3A 04 A2 59"};

char *MD4_testarray[4] = {
    "", "a", "abc",
    "12345678901234567890123456789012345678901234567890123"
    "456789012345678901234567890"};
char *MD4_resultarray[4] = {"31 D6 CF E0 D1 6A E9 31 B7 3C 59 D7 E0 C0 89 C0",
                            "BD E5 2C B3 1D E3 3E 46 24 5E 05 FB DB D6 FB 24",
                            "A4 48 01 7A AF 21 D8 52 5F C1 0A E8 7A A6 72 9D",
                            "E3 3B 4D DC 9C 38 F2 19 9C 3E 7B 16 4F CC 05 36"};

uint8_t *HASH(char *algorithm, void *context, char *input, uint32_t input_len,
              uint32_t output_len, int (*reset_ptr)(void *),
              int (*update_ptr)(void *, const uint8_t *, uint32_t),
              int (*result_ptr)(void *, uint8_t *output));
static void HASHPrint(uint8_t *digest, uint32_t len);
static void HASHString(char *string);
static void HASHTestSuite();
static void HASHRainbowTable();

/*
    Arguments:
     -sstring - digests string
     -x       - runs test suite
     -r       - runs rainbow table script
 */
int main(int argc, char const *argv[]) {

  int i;

  if (argc > 1)
    for (i = 1; i < argc; i++)
      if (argv[i][0] == '-' && argv[i][1] == 's')
        HASHString(argv[i] + 2);
      else if (strcmp(argv[i], "-x") == 0)
        HASHTestSuite();
      else if (strcmp(argv[i], "-r") == 0)
        HASHRainbowTable();
      else
        HASHRainbowTable();
  else
    HASHRainbowTable();

  return 0;
}

/*
 * Call hash on every algorithm.
 */
uint8_t *HASH(char *algorithm, void *context, char *input, uint32_t input_len,
              uint32_t output_len, int (*reset_ptr)(void *),
              int (*update_ptr)(void *, const uint8_t *, uint32_t),
              int (*result_ptr)(void *, uint8_t *output)) {

  uint8_t digest[output_len];

  (*reset_ptr)(context);
  (*update_ptr)(context, input, input_len);
  (*result_ptr)(context, digest);

  printf("%s (\"%s\") = ", algorithm, input);
  HASHPrint(digest, output_len);

  return digest;
}

/*
 * Digests a string and prints the result.
 */
static void HASHString(char *string) {
  uint32_t len = strlen(string);

  /* SHA1 */
  SHA1Context sha;
  HASH("SHA1", &sha, string, len, SHA1HashSize, &SHA1Reset, &SHA1Update,
       &SHA1Result);

  /* MD4 */
  MD4Context md;
  HASH("MD4", &md, string, len, MD4HashSize, &MD4Reset, &MD4Update, &MD4Result);
}

static void HASHTestSuite() {
  int j;

  printf("\n:: SHA1 Test Vectors ::\n\n");

  /* SHA1 */
  SHA1Context sha;
  for (j = 0; j < 4; ++j) {
    HASH("SHA1", &sha, SHA1_testarray[j], strlen(SHA1_testarray[j]),
         SHA1HashSize, &SHA1Reset, &SHA1Update, &SHA1Result);

    printf("Should match:\n");
    printf("\t%s\n", SHA1_resultarray[j]);
  }

  printf("\n:: MD4 Test Vectors ::\n\n");

  /* MD4 */
  MD4Context md;
  for (j = 0; j < 4; ++j) {
    HASH("MD4", &md, MD4_testarray[j], strlen(MD4_testarray[j]), MD4HashSize,
         &MD4Reset, &MD4Update, &MD4Result);

    printf("Should match:\n");
    printf("\t%s\n", MD4_resultarray[j]);
  }
}

static void HASHRainbowTable() {
  int i;

  printf("\n:: SHA1 Rainbow Table Test ::\n\n");

  SHA1Context sha;
  Table rainbow_tables1[TABLE_COUNT];

  precompute(rainbow_tables1, &sha, SHA1HashSize, &SHA1Reset, &SHA1Update,
             &SHA1Result);

  for (i = 0; i < TABLE_COUNT; i++) {
    print_table(rainbow_tables1[i]);
    printf("\n");
  }

  for (i = 0; i < TABLE_COUNT; i++) {
    delete_table(rainbow_tables1[i]);
  }

  printf("\n:: MD4 Rainbow Table Test ::\n\n");

  MD4Context md;
  Table rainbow_tables2[TABLE_COUNT];

  precompute(rainbow_tables2, &md, MD4HashSize, &MD4Reset, &MD4Update,
             &MD4Result);

  for (i = 0; i < TABLE_COUNT; i++) {
    print_table(rainbow_tables2[i]);
    printf("\n");
  }

  for (i = 0; i < TABLE_COUNT; i++) {
    delete_table(rainbow_tables2[i]);
  }
}

/*
 * Prints a message digest in hexadecimal.
 */
static void HASHPrint(uint8_t *digest, uint32_t len) {
  uint32_t i;
  printf("\n\t");
  for (i = 0; i < len; i++) {
    printf("%02X ", digest[i]);
  }
  printf("\n");
}