/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: global.h
 * Created Date: 26/09/2021 11:53:59
 * Last Modified: 03/10/2021 16:17:13
 * -----
 * Copyright (c) 2021
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>

typedef unsigned char bool;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
typedef short int int_16t;

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

#ifndef _GLOBAL_enum_
#define _GLOBAL_enum_
enum {
  Success = 0,
  Null,         /* Null pointer parameter */
  InputTooLong, /* input data too long */
  StateError    /* called Input after Result */
};
#endif

/*
 * Call hash on every algorithm.
 */
void hash(void *context, char *input, uint32_t input_len, uint8_t *output,
          uint32_t output_len, int (*reset_ptr)(void *),
          int (*update_ptr)(void *, const uint8_t *, uint32_t),
          int (*result_ptr)(void *, uint8_t *output));

/*
 * Prints a message digest in hexadecimal.
 */
void print_hash(uint8_t *digest, uint32_t len);

#endif