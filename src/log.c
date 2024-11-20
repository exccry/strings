#include "log.h"

static const char *level_strings[] = {
  "DEBUG",
  "INFO",
  "WARNING",
  "ERROR",
  "FATAL"
};

void _log(LogLevel level, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  if (level >= LOG_LVL_ERROR)
    fprintf(stderr, "%s", RED);

  fprintf(stderr, "[%s] ", level_strings[level]);
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "%s\n", CL_CLEAR);

  fprintf(stderr, "\033[?12l\033[?25h");
  fflush(stderr);

  va_end(args);
}

