#include <stdio.h>
#include <string.h>
#include <stdlib.h>

size_t s_strlen(const char *str)
{
  size_t len = 0;
  for (; *str; ++len, ++str)
    ;
  return len;
}

char *s_strcpy(char *dest, const char *src)
{
  char *p_dest = dest;
  while((*dest++ = *src++))
    ;
  return p_dest;
}

char *s_strncpy(char *dest, const char *src, const size_t size)
{
  char *p_dest = dest;
  size_t i = 0;
  for (; i < size && *src; ++i)
    *dest++ = *src++;

  if (i < size)
    for (; i < size; ++i)
      *dest++ = '\0';

  return p_dest;
}

char *s_strncat(char *dest, const char *src, const size_t n)
{
  char *p_dest = dest, 
    p_start = dest;
  
  while (*p_start)
    ++p_start;
  
  for (int i = 0; i < n && *src; ++i)
    *p_start++ = *src++;

  return p_dest;
}

char *s_strcmp(const char *str1, const char *str2);
char *s_strncmp(const char *str1, const char *str2, const size_t n);

void print_buffer(char *buffer, const size_t size)
{
  for (int i = 0; i < size; ++i)
    buffer[i] ? printf("%c ", buffer[i]) : printf("0 ");
  printf("\n");
}

char *s_str_copy(char *dest, const char *src, const size_t size)
{
  char *p_dest = dest;

  s_strncpy(dest, src, size - 1);
  dest[size - 1] = '\0';

  return p_dest;
}

int main(void)
{
  /*char *str = "something";
  printf("%ld\n", s_strlen(str));
  char str2[50];

  char *p_str = s_strcpy(str2, str);
  printf("%s %ld %d\n", str2, s_strlen(str2), strcmp(str2, str));
  printf("%s\n", p_str);*/

  /*char *p_str = s_strncpy(str2, str, 4);
  printf("%s %ld %d\n", str2, s_strlen(str2), strcmp(str2, str));
  printf("%s\n", p_str);
  */

  char buffer[10];
  s_strcpy(buffer, "Hello man");
  char x[20];
  printf("%s\n", x);
  //s_strncpy(buffer, "Hello mannnn", sizeof(buffer));
  print_buffer(buffer, sizeof(buffer));

  s_str_copy(buffer, "Hello_", sizeof(buffer));
  print_buffer(buffer, sizeof(buffer));

  s_strncpy(buffer, "1", sizeof(buffer));
  print_buffer(buffer, sizeof(buffer));

  s_strncpy(buffer, "12", sizeof(buffer));
  print_buffer(buffer, sizeof(buffer));
  s_strncpy(buffer, "123", sizeof(buffer));
  print_buffer(buffer, sizeof(buffer));
  s_strncpy(buffer, "1234", sizeof(buffer));
  print_buffer(buffer, sizeof(buffer));
  s_strncpy(buffer, "12345", sizeof(buffer));
  print_buffer(buffer, sizeof(buffer));
  s_strncpy(buffer, "123456", sizeof(buffer));
  print_buffer(buffer, sizeof(buffer));
}
