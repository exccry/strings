#include "sstring.h"

#define MAX_CHECK_LIMIT 65000

static const bool has_null_terminator(const char *str)
{
  for (int i = 0; i < MAX_CHECK_LIMIT; ++i)
    if (str[i] == '\0')
      return true;
  
  _log(LOG_LVL_WARNING, "Check limit for string exceeded - no '\\0' found");
  return false;
}

void sstring(const char *str)
{
  if (!str || !strlen(str))
  {
    _log(LOG_LVL_FATAL, "Source string has no length or is undefined");
    exit(EXIT_FAILURE);
  }

  if (!has_null_terminator(str))
    exit(EXIT_FAILURE);
}

/*
SString *sstring(const char *str)
{
  if (!str || !strlen(str))
  {
    _log(LOG_LVL_FATAL, "Source string has no length or is undefined");
    exit(EXIT_FAILURE);
  }

  // sizeof SString is wrong because we also need to take
  // the size/length of the string in consideration !!!
  SString *sstr = secure_alloc(sizeof(SString));
  //sstr.value = secure_alloc(strlen(str));
  

  return sstr;
}
*/
