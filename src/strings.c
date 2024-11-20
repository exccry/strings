#include "log.h"
#include "strings.h"
#include "regman.h"

/* --- STATICS --- */
static const size_t _next_pow2(size_t n)
{
  if (n == 0) return 1;
  
  if (n > (SIZE_MAX / 2))
    return 0; // multiplying by 2 would overflow

  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;

  #if SIZE_MAX > UINT32_MAX
  n |= n >> 32;
  #endif

  return n + 1;
}

static void _reset(String *str)
{
  str->size = 0;
  str->length = 0;
  str->value = NULL;
}

void strings_init(void)
{
  if (atexit(cleanup_strings) != 0)
  {
    _log(LOG_LVL_FATAL, "Failed to register string cleanup functions");
    exit(EXIT_FAILURE);
  }

  register_signal_handlers();
}
/* --- END STATICS --- */

/* --- PUBLIC API --- */
String string(const char *src)
{
  String str = {0};

  if (!src)
  {
    str.length = 0;
    str.size = STRING_INIT_SZ;
    str.value = calloc(str.size, sizeof(char));

    if (!str.value)
    {
      _log(LOG_LVL_FATAL, "Memory allocation failed for String");
      _reset(&str);
      return str;
    }
    
    return str;
  }

  str.length = strlen(src);
  size_t required_size = str.length + 1;

  if (required_size < STRING_INIT_SZ)
    str.size = STRING_INIT_SZ;
  else
  {
    str.size = _next_pow2(required_size);
    if (!str.size)
    {
      _log(LOG_LVL_FATAL, "Size overflow during allocation");
      _reset(&str);
      return str;
    }
  }

  str.value = calloc(str.size, sizeof(char));
  if (!str.value)
  {
    _log(LOG_LVL_FATAL, "Memory allocation failed for String.");
    _reset(&str);
    return str;
  }

  memcpy(str.value, src, str.length);
  str.value[str.length] = '\0';

  return str;
}

String *string_h(const char *src)
{
  String *str = malloc(sizeof(String));
  if (!str)
  {
    _log(LOG_LVL_FATAL, "Memory allocation for String failed");
    return NULL;
  }

  *str = string(src);
  if (!vstr(str))
  {
     free(str);
     return NULL;
  }

  register_string(str);

  return str;
}

void string_destroy(String *str)
{
  if (str && str->value)
  {
    memset(str->value, 0, str->size);
    free(str->value);
    _reset(str);
  }
}

void string_destroy_h(String **str)
{
  if (str && *str)
  {
    unregister_string(*str);
    string_destroy(*str);
    free(*str);
    *str = NULL;
  }
}

bool vstr(String *str)
{
  return str->value == NULL ? false : true;
}

/* --- END PUBLIC API ---  */
