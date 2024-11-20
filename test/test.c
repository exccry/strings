#include "regman.h"
#include "strings.h"
#include "log.h"

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

    while(1)
      string_check_for_signal();

    // Now my_string is NULL, and it has been removed from the registry
    return EXIT_SUCCESS;
}
