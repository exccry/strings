String string(const char *src)
{
  const size_t len = strlen(src);
  const size_t final_len = (len + 1) % 2 == 0 ? len + 1 : len + 2;
  const size_t final_size = len ? 
    (final_len > STRING_INIT_SZ ? 
     final_len : STRING_INIT_SZ) : 
    STRING_INIT_SZ;

  String str = { NULL, len, final_size };

  str.value = malloc(str.size);
  if (!str.value)
  {
    fprintf(stderr, "Memory allocation for string failed");
    exit(EXIT_FAILURE);
  }

  memset(str.value, 0, str.size);
  
  if (str.length)
  {
    strncpy(str.value, src, str.length);
    str.value[str.length] = '\0';
  } 

  return str;
}
// init newly allocated mem space to 0
static bool string_realloc(String *str)
{
  const size_t old_size = str->size;
  const size_t new_size = old_size * STRING_REALLOC_MULTIPLIER;
  
  if ((new_size <= old_size) || (old_size > SIZE_MAX / STRING_REALLOC_MULTIPLIER))
  {
    fprintf(stderr, "Size overflow during reallocation. Old size: %zu\n", old_size);
    return false;
  }

  char *new_value = realloc(str->value, new_size);
  
  if (!new_value)
  {
    fprintf(
      stderr,
      "Memory reallocation for string failed. Old size: %zu, New size: %zu\n",
      old_size, new_size);
    
    return false;
  }

  str->size = new_size;
  str->value = new_value;

  // always zero out newly allocated memory
  memset(str->value + old_size, 0, str->size - old_size);

  return true;
}

static bool string_shrink_to_len(String *str)
{
  // i will implement it
}

bool string_destroy(String str)
{
  return true;
}
