#include "log.h"
#include "strings.h"

static String **allocated_strings = NULL;
static size_t allocated_count = 0;
static size_t allocated_capacity = 0;
static volatile sig_atomic_t signal_received = 0;
static pthread_mutex_t registry_mutex = PTHREAD_MUTEX_INITIALIZER;

void unregister_string(String *str)
{
  if (!str)
    return;

  pthread_mutex_lock(&registry_mutex);

  if (allocated_strings == NULL || allocated_count == 0)
  {
    pthread_mutex_unlock(&registry_mutex);
    return;
  }

  for (size_t i = 0; i < allocated_count; ++i)
  {
    if (allocated_strings[i] == str)
    {
      allocated_strings[i] = allocated_strings[allocated_count - 1];
      allocated_strings[allocated_count - 1] = NULL;
      --allocated_count;

      break;
    }
  }

  pthread_mutex_unlock(&registry_mutex);
}

void register_signal_handlers(void)
{
  struct sigaction sa;
  sa.sa_handler = signal_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  int signals[] = { SIGINT, SIGTERM, SIGABRT, SIGSEGV, SIGFPE };
  for (int i = 0; i < ARRS(signals); ++i)
  {
    if (sigaction(signals[i], &sa, NULL) != 0)
    {
      _log(LOG_LVL_FATAL, "Failed to register signal handler for signal %d", signals[i]);
      exit(EXIT_FAILURE);
    }
  }
}

void signal_handler(int signum)
{
  signal_received = signum;
}

void string_check_for_signal(void)
{
  if (signal_received)
  {
    _log(LOG_LVL_INFO, "Received signal %d, cleaning up...", signal_received);
    cleanup_strings();
    exit(EXIT_FAILURE);
  }
}

void register_string(String *str)
{
  if (!str)
    return;

  pthread_mutex_lock(&registry_mutex);

  if (allocated_count == allocated_capacity)
  {
    size_t new_capacity = allocated_capacity == 0 ? 16 : allocated_capacity * 2;
    String **new_list = realloc(allocated_strings, new_capacity * sizeof(String *));
    if (!new_list)
    {
      _log(LOG_LVL_FATAL, "Failed to allocate memory for string registry");
      pthread_mutex_unlock(&registry_mutex);
      exit(EXIT_FAILURE);
    }

    allocated_strings = new_list;
    allocated_capacity = new_capacity;
  }
  
  allocated_strings[allocated_count++] = str;

  pthread_mutex_unlock(&registry_mutex);
}

void cleanup_strings(void)
{
  for (int i = 0; i < allocated_count; ++i)
    string_destroy_h(&allocated_strings[i]);

  free(allocated_strings);
  allocated_strings = NULL;
  allocated_count = 0;
  allocated_capacity = 0;
  //_log(LOG_LVL_INFO, "Cleanup strings executed successfully");
}

void init_string_lib(void)
{
  if (atexit(cleanup_strings) != 0)
  {
    _log(LOG_LVL_FATAL, "Failed to register string cleanup functions");
    exit(EXIT_FAILURE);
  }

  register_signal_handlers();
}

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


