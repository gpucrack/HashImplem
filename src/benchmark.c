/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: benchmark.c
 * Created Date: 03/10/2021 16:43:26
 * Last Modified: 11/11/2021 14:06:16
 * -----
 * Copyright (c) 2021
 */

#include "benchmark.h"

void benchmarkSHA1(uint32_t iterations) {
  char *string = "abc";
  uint32_t len = 3;

  SHA1Context sha;
  uint32_t i, j;
  uint8_t digest[SHA1HashSize];

  clock_t begin = clock();
  for (i = 0; i < iterations; i++) {
    hash(&sha, string, len, digest, SHA1HashSize, &SHA1Reset, &SHA1Update,
         &SHA1Result);
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("SHA1 (HashImplem) took %g s and last hash is: ", time_spent);
  print_hash(digest, SHA1HashSize);
  printf("\n");

  begin = clock();
  for (j = 0; j < iterations; j++) {
    SHA1(string, len, digest);
  }
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("SHA1 (OpenSSL) took %g s and last hash is: ", time_spent);
  print_hash(digest, SHA1HashSize);
  printf("\n");
}

void benchmarkMD4(uint32_t iterations) {
  char *string = "abc";
  uint32_t len = 3;

  MD4Context md;
  uint32_t i, j;
  uint8_t digest[MD4HashSize];

  clock_t begin = clock();
  for (i = 0; i < iterations; i++) {
    hash(&md, string, len, digest, MD4HashSize, &MD4Reset, &MD4Update,
         &MD4Result);
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("MD4 (HashImplem) took %g s and last hash is: ", time_spent);
  print_hash(digest, MD4HashSize);
  printf("\n");

  begin = clock();
  for (j = 0; j < iterations; j++) {
    MD4(string, len, digest);
  }
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("MD4 (OpenSSL) took %g s and last hash is: ", time_spent);
  print_hash(digest, MD4HashSize);
  printf("\n");
}

void benchmarkNTHASH(uint32_t iterations) {
  char *string = "abc";
  uint32_t len = 3;

  MD4Context md;
  uint32_t i, j;
  uint8_t digest[NTHashSize];

  clock_t begin = clock();
  for (i = 0; i < iterations; i++) {
    hash(&md, string, len, digest, NTHashSize, &NTReset, &NTUpdate, &NTResult);
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("NTLM (HashImplem) took %g s and last hash is: ", time_spent);
  print_hash(digest, NTHashSize);
  printf("\n");

  begin = clock();
  for (j = 0; j < iterations; j++) {
    int bufsize = len * 2;
    uint8_t buf[bufsize];
    to_utf16le(string, len, buf, bufsize);
    MD4(buf, bufsize, digest);
  }
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("NTLM (OpenSSL) took %g s and last hash is: ", time_spent);
  print_hash(digest, NTHashSize);
  printf("\n");
}

void benchmark(uint32_t iterations) {
  printf("Benchmarking over %d iterations\n", iterations);
  benchmarkSHA1(iterations);
  benchmarkMD4(iterations);
  benchmarkNTHASH(iterations);
}