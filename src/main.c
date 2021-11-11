/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: main.c
 * Created Date: 25/09/2021 18:44:37
 * Last Modified: 11/11/2021 13:49:11
 * -----
 * Copyright (c) 2021
 */

#include "benchmark.h"
#include "global.h"
#include "md4.h"
#include "nthash.h"
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

char *NT_testarray[4] = {"", "a", "abc", "OLDPASSWORD"};
char *NT_resultarray[4] = {"31 D6 CF E0 D1 6A E9 31 B7 3C 59 D7 E0 C0 89 C0",
                           "18 6C B0 91 81 E2 C2 EC AA C7 68 C4 7C 72 99 04",
                           "E0 FB A3 82 68 D0 EC 66 EF 1C B4 52 D5 88 5E 53",
                           "66 77 B2 C3 94 31 13 55 B5 4F 25 EE C5 BF AC F5"};

uint8_t *HASH(char *algorithm, void *context, char *input, uint32_t input_len,
              uint32_t output_len, int (*reset_ptr)(void *),
              int (*update_ptr)(void *, const uint8_t *, uint32_t),
              int (*result_ptr)(void *, uint8_t *output));
static void HASHPrint(uint8_t *digest, uint32_t len);
static void HASHString(char *string);
static void HASHTestSuite();
static void HASHBenchmark();
static void HASHRainbowTable();
static void Help();

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
      else if (strcmp(argv[i], "-b") == 0)
        HASHBenchmark();
      else if (strcmp(argv[i], "-r") == 0)
        HASHRainbowTable();
      else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        Help();
      else
        HASHRainbowTable();
  else
    HASHRainbowTable();

  return 0;
}

void Help() {
  char *name = "./main.out";
  printf("Usage: %s [OPTION]\n", name);
  printf("\n");
  printf("  -sstring - digests string\n");
  printf("  -x       - runs test suite\n");
  printf("  -b       - runs benchmark\n");
  printf("  -r       - runs rainbow table script\n");
  printf("  -h       - prints this help\n");
  printf("\n");
  printf("Examples:\n");
  printf("  %s -sabc\n", name);
  printf("  %s -x\n", name);
  printf("  %s -b\n", name);
  printf("  %s -r\n", name);
  printf("  %s -h\n", name);
}

uint8_t *HASH(char *algorithm, void *context, char *input, uint32_t input_len,
              uint32_t output_len, int (*reset_ptr)(void *),
              int (*update_ptr)(void *, const uint8_t *, uint32_t),
              int (*result_ptr)(void *, uint8_t *output)) {
  uint8_t digest[output_len];

  hash(context, input, input_len, digest, output_len, (*reset_ptr),
       (*update_ptr), (*result_ptr));

  printf("%s (\"%s\") =\n\t", algorithm, input);
  print_hash(digest, output_len);

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

  /* NTHash */
  NTContext nt;
  HASH("NT", &nt, string, len, NTHashSize, &NTReset, &NTUpdate, &NTResult);
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

  printf("\n:: NTHash Test Vectors ::\n\n");

  /* NT */
  NTContext nt;
  for (j = 0; j < 4; ++j) {
    HASH("NT", &nt, NT_testarray[j], strlen(NT_testarray[j]), NTHashSize,
         &NTReset, &NTUpdate, &NTResult);

    printf("Should match:\n");
    printf("\t%s\n", NT_resultarray[j]);
  }
}

static void HASHBenchmark() { benchmark(1000000); }

static void HASHRainbowTable() {
  int i;

  printf("\n:: SHA1 Rainbow Table Test ::\n\n");

  SHA1Context sha;
  Table rainbow_tables1[TABLE_COUNT];

  precompute(rainbow_tables1, &sha, SHA1HashSize, &SHA1Reset, &SHA1Update,
             &SHA1Result);

  // for (i = 0; i < TABLE_COUNT; i++) {
  //   print_table(rainbow_tables1[i]);
  //   printf("\n");
  // }
  print_table(rainbow_tables1[0]);

  char *password = "pls";
  uint8_t digest[SHA1HashSize];
  hash(&sha, password, strlen(password), digest, SHA1HashSize, &SHA1Reset,
       &SHA1Update, &SHA1Result);

  printf("Looking for password '%s', hashed as ", password);
  print_hash(digest, SHA1HashSize);
  printf(".\n\n");

  char found[MAX_PASSWORD_LENGTH + 1];

  printf("Starting attack...\n");
  attack(rainbow_tables1, digest, found, SHA1HashSize, &sha, &SHA1Reset,
         &SHA1Update, &SHA1Result);

  if (!strcmp(found, "")) {
    printf("No password found for the given hash.\n");
  } else {
    printf("Password '%s' found for the given hash!\n", found);
  }

  // Clear

  for (i = 0; i < TABLE_COUNT; i++) {
    delete_table(rainbow_tables1[i]);
  }
}