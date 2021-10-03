/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: nthash.h
 * Created Date: 26/09/2021 14:25:37
 * Last Modified: 26/09/2021 14:27:21
 * -----
 * Copyright (c) 2021
 */

#ifndef _NT_H_

#include "global.h"

#define _NT_H_
#define NTHashSize 16

/* NTHash context. */
typedef struct NTContext {
  uint32_t state[4];  /* state (ABCD) */
  uint32_t count[2];  /* number of bits, modulo 2^64 (lsb first) */
  uint8_t buffer[64]; /* input buffer */
} NTContext;

/*
 *  Function Prototypes
 */
int NTReset(NTContext *);
int NTUpdate(NTContext *, const uint8_t *, uint32_t);
int NTResult(NTContext *, uint8_t Message_Digest[NTHashSize]);

#endif