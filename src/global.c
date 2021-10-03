/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: global.c
 * Created Date: 03/10/2021 15:29:41
 * Last Modified: 03/10/2021 16:07:06
 * -----
 * Copyright (c) 2021
 */

#include "global.h";

void hash(void *context, char *input, uint32_t input_len, uint8_t *output,
          uint32_t output_len, int (*reset_ptr)(void *),
          int (*update_ptr)(void *, const uint8_t *, uint32_t),
          int (*result_ptr)(void *, uint8_t *output)) {

  (*reset_ptr)(context);
  (*update_ptr)(context, input, input_len);
  (*result_ptr)(context, output);
}

void print_hash(uint8_t *digest, uint32_t len) {
  uint32_t i;
  for (i = 0; i < len; i++) {
    printf("%02X ", digest[i]);
  }
  printf("\n");
}