#ifndef NORMALC_OPTION_H
#define NORMALC_OPTION_H

#include "stdbool.h"

/**
 * Defines an option type for the given type with attached helper functions
 * 
 * Paramaters:
 * type: inner type for the option
 * type_name: upper case name of the type (e.g., Int, Double, String)
 * func_name: lower case name of the type (e.g., int, double, string)
 * default_value: default value for empty options
 * 
 * Defined Functions:
 * option_type(value) -> OptionType
 * option_type_empty() -> OptionType
 * option_type_present(option) -> bool
 * option_type_empty(option) -> bool
 * option_type_get_or(option, other) -> type
 * option_type_get_or_default(option) -> type
 * 
 */
#define OPTION_TYPE(type, type_name, func_name, default_value) \
    typedef struct { type value; bool present; } Option##type_name; \
    inline Option##type_name option_##func_name(type value) { \
        return Option##type_name { value, true }; \
    } \
    inline Option##type_name option_##func_name##_empty() { \
        return Option##type_name { default_value, false }; \
    } \
    inline bool option_##func_name##_present(Option##type_name value) { \
        return value.present; \
    } \
    inline bool option_##func_name##_empty(Option##type_name value) { \
        return !value.present; \
    } \
    inline type option_##func_name##_get_or(Option##type_name value, type other) { \
        return value.present ? value.value : other; \
    } \
    inline type option_##func_name##_get_or_default(Option##type_name option) { \
        return option.present ? option.value : default_value; \
    } \

OPTION_TYPE(int, Int, int, 0);
OPTION_TYPE(long, Long, long, 0L);
OPTION_TYPE(float, Float, float, 0.0F);
OPTION_TYPE(double, Double, double, 0.0);
OPTION_TYPE(char, Char, char, '\0');
OPTION_TYPE(bool, Bool, bool, false);

#endif
