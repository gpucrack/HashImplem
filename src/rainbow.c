/*
 * Author: Hugo Pointcheval
 * Email: git@pcl.ovh
 * -----
 * File: rainbow.c
 * Created Date: 26/09/2021 11:33:43
 * Last Modified: 03/10/2021 17:10:21
 * -----
 * Copyright (c) 2021
 */

// Adapted from https://github.com/gpucrack/NaiveRainbow

#include "rainbow.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  else if (n < 36) {
    return 'A' + (n - 10);
  }

  // a..z
  else if (n < 62) {
    return 'a' + (n - 36);
  }

  else if (n == 62) {
    return '-';
  }

  return '_'; // 63
}

/*
        A reduce operation, which returns a plain text
        for a given digest and a given `iteration`.
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
        Transforms a startpoint from a counter to a valid password.
        Note that the last character of the counter is the MSB.
        The implementation uses bit shifting as I didn't think of
        another way of doing this, as a result it only works if the
        range of characters is a multiple of 2.
*/
void retrieve_startpoint(uint32_t counter, uint8_t *plain_text) {
  uint32_t i;
  for (i = 0; i < MAX_PASSWORD_LENGTH; i++) {
    // get the character corresponding to the last 6 bits
    plain_text[i] = char_in_range(counter & 63);
    // right shift the counter by 6 bits (2^6 == 64 values)
    counter >>= 6;

    if (!counter) {
      break;
    }
  }

  plain_text[i + 1] = '\0';
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
  uint8_t **ending_points = malloc(sizeof(char *) * TABLE_M);

  // generate all rows
  for (i = 0; i < TABLE_M; i++) {
    // generate the chain
    ending_points[i] = malloc(sizeof(char) * MAX_PASSWORD_LENGTH + 1);

    char last_plain_text[MAX_PASSWORD_LENGTH + 1];
    retrieve_startpoint(counter_start + i, last_plain_text);
    sprintf(last_plain_text, "%u", counter_start + i);

    /*
            Apply a round of hash + reduce `TABLE_T - 1` times.
            The chain should look like this:
            n -> r0(h(n)) -> r1(h(r0(h(n))) -> ...
    */
    for (j = 0; j < TABLE_T - 1; j++) {
      uint8_t digest[output_len];
      hash(context, last_plain_text, strlen(last_plain_text), digest,
           output_len, (*reset_ptr), (*update_ptr), (*result_ptr));
      reduce(digest, output_len, j, last_plain_text);
    }

    // insert only if the row is unique.
    // else, we mark the endpoint as NULL.
    bool merge = 0;
    for (j = 0; j < i; j++) {
      if (ending_points[j] && !strcmp(last_plain_text, ending_points[j])) {
        merge = 1;
        break;
      }
    }

    if (merge) {
      free(ending_points[i]);
      ending_points[i] = NULL;
    } else {
      strcpy(ending_points[i], last_plain_text);
    }
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
    uint8_t starting_point[MAX_PASSWORD_LENGTH + 1];
    retrieve_startpoint(table.starting_point + i, starting_point);

    printf("%s -> ... -> %s\n", starting_point, table.ending_points[i]);
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

/*
        Online phase of the attack.
        Uses the pre-generated rainbow tables to guess
        the plain text of the given cipher text.
        Returns in `password` the match if any, or returns an empty string.
*/
void attack(Table *rainbow_tables, uint8_t *digest, uint8_t *password,
            uint32_t hash_size, void *context, int (*reset_ptr)(void *),
            int (*update_ptr)(void *, const uint8_t *, uint32_t),
            int (*result_ptr)(void *, uint8_t *output)) {
  for (uint32_t i = 0; i < TABLE_COUNT; i++) {
    // iterate column by column, starting from the last plaintext
    // https://stackoverflow.com/questions/3623263/reverse-iteration-with-an-unsigned-loop-variable
    for (uint32_t j = TABLE_T - 1; j-- > 0;) {
      uint8_t column_plain_text[MAX_PASSWORD_LENGTH + 1];
      uint8_t column_digest_text[hash_size];
      memcpy(column_digest_text, digest, hash_size);

      // get the reduction of the cipher text corresponding to the current
      // column
      for (uint32_t k = j; k < TABLE_T - 2; k++) {
        reduce(column_digest_text, strlen(column_plain_text), k,
               column_plain_text);
        hash(context, column_plain_text, strlen(column_plain_text),
             column_digest_text, hash_size, (*reset_ptr), (*update_ptr),
             (*result_ptr));
      }
      reduce(column_digest_text, strlen(column_plain_text), TABLE_T - 2,
             column_plain_text);

      // iterate through all rows to check if it'a an endpoint
      for (uint32_t k = 0; k < TABLE_M; k++) {
        // we found a matching endpoint
        if (rainbow_tables[i].ending_points[k] &&
            !strcmp(rainbow_tables[i].ending_points[k], column_plain_text)) {
          // re-construct the chain
          uint8_t chain_plain_text[MAX_PASSWORD_LENGTH + 1];
          uint8_t chain_digest_text[hash_size];
          retrieve_startpoint(rainbow_tables[i].starting_point + k,
                              chain_plain_text);

          for (uint32_t l = 0; l < j; l++) {
            hash(context, chain_plain_text, strlen(chain_plain_text),
                 chain_digest_text, hash_size, (*reset_ptr), (*update_ptr),
                 (*result_ptr));
            reduce(chain_digest_text, strlen(chain_plain_text), l,
                   chain_plain_text);
          }
          hash(context, chain_plain_text, strlen(chain_plain_text),
               chain_digest_text, hash_size, (*reset_ptr), (*update_ptr),
               (*result_ptr));

          /*
                  The cipher was indeed present in the chain, this was
                  not a false positive from a reduction.
                  We found a plain text that matches the cipher text!
          */
          if (!memcmp(chain_digest_text, digest, hash_size)) {
            strcpy(password, chain_plain_text);
            return;
          }
        }
      }
    }
  }

  // no match found
  password[0] = '\0';
}
