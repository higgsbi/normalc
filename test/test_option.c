#include <normalc/safety/option.h>
#include <normalc/string/string.h>
#include <stdio.h>

typedef struct {
    char foo[10];
    size_t bar;
} CustomType;

#define DEFAULT_TYPE { "Default: ", 0 }
OPTION_TYPE(CustomType, CustomType, custom_type, DEFAULT_TYPE)

int main() { 
    OptionInt opt_int = option_int(5);
    if (option_int_is_present(opt_int)) {
        printf("Present: %d\n", option_int_get(opt_int));
    }

    OptionInt none = option_int_empty();
    if (option_int_is_empty(none)) {
        printf("Empty int option\n");
    }

    OptionString opt_string = option_string(string_from("String example"));

    if (option_string_is_present(opt_string)) {
        printf("Present: %s\n", option_string_get(opt_string)->buffer);
    }
    string_free(opt_string.value);

    OptionCustomType opt_type_two = option_custom_type_empty();
    CustomType type_two = option_custom_type_get_or(opt_type_two, (CustomType) {"Number: ", 5});
    printf("%s %zu\n", type_two.foo, type_two.bar);

    printf("Getting empty option:\n");
    opt_int = option_int_empty();
    printf("Present? -> %d\n", option_int_get(opt_int));
    

    return 0;
}
