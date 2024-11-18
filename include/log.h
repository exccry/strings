#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
  LOG_LVL_DEBUG,
  LOG_LVL_INFO,
  LOG_LVL_WARNING,
  LOG_LVL_ERROR,
  LOG_LVL_FATAL,
} LogLevel;

void _log(LogLevel level, const char *fmt, ...);

