#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <signal.h>
#include <stdatomic.h>
#include <pthread.h>

#define STRING_INIT_SZ            32
#define STRING_REALLOC_MULTIPLIER 4

#define ARRS(arr) (sizeof((arr)) / sizeof((arr[0])))

typedef struct strlib_string
{
  char *value;
  size_t length;
  size_t size;
} String;

void init_string_lib(void);

// for stack instantiation
String string(const char *str);

// for heap instantiations/allocations
String *string_h(const char *str);

// safe free stack
void string_destroy(String *str);

// secure use after free
void string_destroy_h(String **str);

bool vstr(String *str);

