#include "regman.h"
#include "strings.h"
#include "log.h"
#include "sstring.h"

int main(void)
{
   strings_init();

    // Create a heap-allocated String
    String *my_string = string_h("Hello, World!");
    if (!my_string || !my_string->value)
    {
        _log(LOG_LVL_ERROR, "Failed to create String.");
        return EXIT_FAILURE;
    }

    String str = string("Whaaaats going on my maaaaan");

    printf("%s\nlength: %ld\n", my_string->value, my_string->length);
    printf("%s\nlength: %ld\n", str.value, str.length);
    
    // Destroy the string when done
    string_destroy(&str);
    string_destroy_h(&my_string);

    const char *name = "my name is steve jobs\n";
    
    char buf[5];
    strncpy(buf, "some naaame", 10);
    printf("length: %d\t %d \n", strlen(buf), buf[5] == '\0');
    sstring(name);
    sstring(buf);

    string_check_for_signal();
    // Now my_string is NULL, and it has been removed from the registry
    return EXIT_SUCCESS;
}
