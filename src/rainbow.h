/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: rainbow.h
 * Created Date: 26/09/2021 16:16:53
 * Last Modified: 03/10/2021 16:32:35
 * -----
 * Copyright (c) 2021
 */

#ifndef _RAINBOW_H_

#include "global.h"

#define _RAINBOW_H_

// The max password length in the rainbow tables.
#define MAX_PASSWORD_LENGTH 2

// The number of rows in the table.
#define TABLE_M 30

// The size of a chain in the table.
#define TABLE_T 10000

// The number of tables.
#define TABLE_COUNT 3

/*
        A rainbow table.
        The endings points are an array of plain texts.
        The starting point is the plain text of the first raw,
        as an unsigned int.
*/
typedef struct Table {
  uint32_t starting_point;
  char **ending_points;
} Table;

void precompute(Table *rainbow_tables, void *context, uint32_t output_len,
                int (*reset_ptr)(void *),
                int (*update_ptr)(void *, const uint8_t *, uint32_t),
                int (*result_ptr)(void *, uint8_t *output));
void attack(Table *rainbow_tables, uint8_t *digest, uint8_t *password,
            uint32_t hash_size, void *context, int (*reset_ptr)(void *),
            int (*update_ptr)(void *, const uint8_t *, uint32_t),
            int (*result_ptr)(void *, uint8_t *output));

void delete_table(Table rainbow_table);
void print_table(Table table);

#endif