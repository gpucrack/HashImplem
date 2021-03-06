/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: sha1.h
 * Created Date: 23/09/2021 16:22:51
 * Last Modified: 26/09/2021 15:44:57
 * -----
 * Copyright (c) 2021
 */

#ifndef _SHA1_H_

#include "global.h"

#define _SHA1_H_
#define SHA1HashSize 20

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation
 */
typedef struct SHA1Context {
  uint32_t Intermediate_Hash[SHA1HashSize / 4]; /* Message Digest  */

  uint32_t Length_Low;  /* Message length in bits      */
  uint32_t Length_High; /* Message length in bits      */

  /* Index into message block array   */
  int_16t Message_Block_Index;
  uint8_t Message_Block[64]; /* 512-bit message blocks      */

  int Computed;  /* Is the digest computed?         */
  int Corrupted; /* Is the message digest corrupted? */
} SHA1Context;

/*
 *  Function Prototypes
 */
int SHA1Reset(SHA1Context *);
int SHA1Update(SHA1Context *, const uint8_t *, unsigned int);
int SHA1Result(SHA1Context *, uint8_t Message_Digest[SHA1HashSize]);

#endif