/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: global.h
 * Created Date: 26/09/2021 11:53:59
 * Last Modified: 03/10/2021 15:15:23
 * -----
 * Copyright (c) 2021
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

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