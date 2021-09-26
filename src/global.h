/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: global.h
 * Created Date: 26/09/2021 11:53:59
 * Last Modified: 26/09/2021 12:05:40
 * -----
 * Copyright (c) 2021
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

/*
 * If you do not have the ISO standard stdint.h header file, then you
 * must typdef the following:
 *  name             meaning
 *  uint32_t         unsigned 32 bit integer
 *  uint8_t          unsigned 8 bit integer (i.e., unsigned char)
 *  int_least16_t    integer of >= 16 bits
 */

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

#endif