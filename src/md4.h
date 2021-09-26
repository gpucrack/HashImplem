/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: md4.h
 * Created Date: 25/09/2021 22:34:45
 * Last Modified: 26/09/2021 12:32:34
 * -----
 * Copyright (c) 2021
 */

#ifndef _MD4_H_

#include "global.h"

#define _MD4_H_
#define MD4HashSize 16

/* MD4 context. */
typedef struct MD4Context {
  uint32_t state[4];  /* state (ABCD) */
  uint32_t count[2];  /* number of bits, modulo 2^64 (lsb first) */
  uint8_t buffer[64]; /* input buffer */
} MD4Context;

/*
 *  Function Prototypes
 */
int MD4Reset(MD4Context *);
int MD4Update(MD4Context *, const uint8_t *, uint32_t);
int MD4Result(MD4Context *, uint8_t Message_Digest[MD4HashSize]);

#endif