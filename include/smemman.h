#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "log.h"

void *secure_alloc(const size_t size);

void secure_free(void **ptr, const size_t size);

void secure_chfree(char **str, const size_t size);

const int seccmp(const void *a, const void *b, const size_t size);
