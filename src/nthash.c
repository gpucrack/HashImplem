/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: nthash.c
 * Created Date: 26/09/2021 14:25:44
 * Last Modified: 09/11/2021 10:56:17
 * -----
 * Copyright (c) 2021
 */

#include "nthash.h"
#include "md4.h"

void to_utf16le(const uint8_t *message, uint32_t message_len, uint8_t *out,
                uint32_t out_len) {
  int i;
  for (i = 0; i < out_len; i++) {
    out[i] = (char)0;
  }

  for (i = 0; i < message_len; i++) {
    out[i * 2] = message[i];
  }
}

int NTReset(NTContext *context) { return MD4Reset((MD4Context *)context); }

int NTUpdate(NTContext *context, const uint8_t *message_array,
             uint32_t length) {

  int bufsize = length * 2;
  uint8_t buf[bufsize];
  to_utf16le(message_array, length, buf, bufsize);

  return MD4Update((MD4Context *)context, buf, bufsize);
}

int NTResult(NTContext *context, uint8_t Message_Digest[NTHashSize]) {
  return MD4Result((MD4Context *)context, Message_Digest);
}
