#include "regman.h"

static size_t allocated_count = 0;
static size_t allocated_capacity = 0;
static String **allocated_strings = NULL;
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
}

