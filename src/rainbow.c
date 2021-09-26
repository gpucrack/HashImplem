/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: rainbow.c
 * Created Date: 26/09/2021 11:33:43
 * Last Modified: 26/09/2021 16:46:11
 * -----
 * Copyright (c) 2021
 */

#include "rainbow.h";
#include <stdio.h>;
#include <stdlib.h>;
#include <string.h>;

char char_in_range(uint8_t n);
void reduce(uint8_t digest[], uint32_t digest_len, uint32_t iteration,
            char *plain_text);
Table gen_table(uint32_t counter_start, void *context, uint32_t output_len,
                int (*reset_ptr)(void *),
                int (*update_ptr)(void *, const uint8_t *, uint32_t),
                int (*result_ptr)(void *, uint8_t *output));

/*
        Returns a char in the [a-zA-Z0-9_-] range given a parameter in the
   [0-63] range. Look at an ASCII table to better understand this function
   (https://www.asciitable.com/).
*/
char char_in_range(uint8_t n) {
  if (!(n >= 0 && n <= 63)) {
    exit(1);
  }

  // 0..9
  if (n < 10) {
    return '0' + n;
  }

  // A..Z
  if (n < 36) {
    return 'A' + (n - 10);
  }

  // a..z
  if (n < 62) {
    return 'a' + (n - 36);
  }

  if (n == 62) {
    return '-';
  }

  return '_';
}

/*
        A reduce operation, which returns a plain text
        for a given cipher text and a given `iteration`.
        The nth `iteration` reduction function should give
        the nth+1 plain text reduction.
        We sum in an array of the size of the plain text and
        take the modulo to get an alphanumeric char.
*/
void reduce(uint8_t digest[], uint32_t digest_len, uint32_t iteration,
            char *plain_text) {
  uint32_t i;
  uint32_t sum[MAX_PASSWORD_LENGTH] = {0};
  uint32_t current_index = 0;

  for (i = 0; i < digest_len; i++) {
    sum[i % MAX_PASSWORD_LENGTH] += digest[i];
  }

  for (i = 0; i < MAX_PASSWORD_LENGTH; i++) {
    uint32_t mod = (sum[i] + iteration) % 65;

    // if mod == 64, skip a character (smaller password)
    if (mod != 64) {
      plain_text[current_index] = char_in_range(mod);
      current_index++;
    }
  }

  plain_text[current_index] = '\0';
}

/*
        Generates a rainbow table of size `TABLE_M*TABLE_T`, where
        `TABLE_M` is the number of rows
        `TABLE_T` is the number of plain texts in a chain.
        The `counter_start` parameter is used to discriminate
        rainbow tables so they're not all similar.
*/
Table gen_table(uint32_t counter_start, void *context, uint32_t output_len,
                int (*reset_ptr)(void *),
                int (*update_ptr)(void *, const uint8_t *, uint32_t),
                int (*result_ptr)(void *, uint8_t *output)) {

  uint32_t i, j;
  char **ending_points = malloc(sizeof(char *) * TABLE_M);

  // generate all rows
  for (i = 0; i < TABLE_M; i++) {
    // generate the chain
    ending_points[i] = malloc(sizeof(char) * MAX_PASSWORD_LENGTH + 1);

    char last_plain_text[MAX_PASSWORD_LENGTH + 1];
    sprintf(last_plain_text, "%u", counter_start + i);

    /*
            Apply a round of hash + reduce `TABLE_T - 1` times.
            The chain should look like this:
            n -> r0(h(n)) -> r1(h(r0(h(n))) -> ...
    */
    for (j = 0; j < TABLE_T - 1; j++) {
      uint8_t digest[output_len];

      (*reset_ptr)(context);
      (*update_ptr)(context, last_plain_text, strlen(last_plain_text));
      (*result_ptr)(context, digest);
      reduce(digest, output_len, j, last_plain_text);
    }

    strcpy(ending_points[i], last_plain_text);
  }

  return (Table){counter_start, ending_points};
}

/*
        Deletes a table.
*/
void delete_table(Table table) {
  uint32_t i;
  for (i = 0; i < TABLE_M; i++) {
    free(table.ending_points[i]);
  }

  free(table.ending_points);
}

// Pretty-prints a rainbow table.
void print_table(Table table) {
  uint32_t i;
  for (i = 0; i < TABLE_M; i++) {
    printf("%d -> ... -> %.*s\n", table.starting_point + i, MAX_PASSWORD_LENGTH,
           table.ending_points[i]);
  }
}

/*
        Offline phase of the attack.
        Generates all rainbow tables needed.
*/
void precompute(Table *rainbow_tables, void *context, uint32_t output_len,
                int (*reset_ptr)(void *),
                int (*update_ptr)(void *, const uint8_t *, uint32_t),
                int (*result_ptr)(void *, uint8_t *output)) {
  // make sure the counter of the starting point doesn't overflow
  // the plain text length
  char max_counter_length[128];
  sprintf(max_counter_length, "%u", TABLE_COUNT * TABLE_M);

  for (int i = 0; i < TABLE_COUNT; i++) {
    rainbow_tables[i] = gen_table(i * TABLE_M, context, output_len,
                                  (*reset_ptr), (*update_ptr), (*result_ptr));
  }
}