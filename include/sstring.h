#pragma once

#include "log.h"
#include "smemman.h"

// should use secure mem-management
// every modifier function on the SString should yield a new object -> const
// the old obj must be securely overwritten, freed and set to NULL
typedef struct
{
  const char *value;
  const size_t length;
  const size_t size;
} SString;
