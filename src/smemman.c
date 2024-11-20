#include "smemman.h"

const int seccmp(const void *a, const void *b, const size_t size)
{
  unsigned char res;
  const unsigned char *p1 = a;
  const unsigned char *p2 = b;

  for (int i = 0; i < size; ++i)
    res |= p1[i] ^ p2[i];

  return res == 0;
}


void *secure_alloc(const size_t size)
{
  if (!size)
  {
    _log(LOG_LVL_FATAL, "size is 0");
    return NULL;
  }

  void *ptr = malloc(size);
  if (ptr == NULL) return NULL;

  if (mlock(ptr, size) != 0)
  {
    _log(LOG_LVL_FATAL, "mlock failed");
    free(ptr);
    return NULL;
  }

  return ptr;
}


void secure_free(void **ptr, const size_t size)
{
  if (!ptr || !(*ptr)) return;

  memset(*ptr, 0, size);
  munlock(*ptr, size);
  free(*ptr);
  *ptr = NULL;
}

void secure_chfree(char **str, const size_t size)
{
  void **ptr = (void **)str;
  secure_free(ptr, size);
}
