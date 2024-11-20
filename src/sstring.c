#include "sstring.h"

#define MAX_CHECK_LIMIT 1024

const int sstrlen(const char *str)
{
  for (int i = 0; i < MAX_CHECK_LIMIT; ++i)
  {
    if (!isascii(str[i]) && str[i] != '\0')
    {
      _log(LOG_LVL_FATAL, "String failed length check, string might be malicious!");
      return 0;
    }

    if (str[i] == '\0')
      return i;
  }

  _log(LOG_LVL_FATAL, "String exceeded length check, no '\\0' found");
  return 0;
}

void sstring(const char *str)
{
  if (!str)
  {
    _log(LOG_LVL_FATAL, "String is undefined");
    exit(EXIT_FAILURE);
  }

  const int len = sstrlen(str);
  if (!len)
  {
     _log(LOG_LVL_FATAL, "String has no length or is undefined");
    exit(EXIT_FAILURE);
  }  
  
  printf("%d\n", len);
}
