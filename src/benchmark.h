/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: benchmark.h
 * Created Date: 03/10/2021 16:43:21
 * Last Modified: 11/11/2021 14:04:15
 * -----
 * Copyright (c) 2021
 */

#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

#include "global.h"
#include "md4.h"
#include "nthash.h"
#include "sha1.h"
#include <openssl/md4.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <time.h>

void benchmark(uint32_t iterations);

#endif
