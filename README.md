# Normalc

## Build

The normalc standard library is available as a static library for system and local installation.
The local installation installs headers and libs to the `out` directory. 
The system installation installs to `/usr/local` for unix systems and CMake's `CMAKE_INSTALL_PREFIX` for Windows.

Local: `chmod +x install.sh && ./install.sh --local`  
System: `chmod +x install.sh && ./install.sh`  

Note: Windows users can access this with [WSL](https://learn.microsoft.com/en-us/windows/wsl/install)

## Type Safety

Normalc aims to allow for flexibility in data structures by using void pointers. However,
this results in tiresome casting and the removal of reasonable type safety. To remedy, each
data type can opt in to type safety with a simple 1 line macro. Here's an example with strings
in a vector:

#### Flexible
```C
void test()  {
    // (+) vector creation without complicated macros
    Vector* vector = vector_new(
		  1, 
		  (Duplicator) string_clone, 
		  (Destructor) string_free
		);

    // automatically casted from void*
    // (-) simple, no extra code, no macros
    // (-) no type safety
		vector_add(vector, string_from_format("foo %s", "bar"));	
    string_println(vector_get(vector, 0));

    vector_free(vector);
}
```

#### Type Safe
```C
VECTOR_SAFE(String, string)

void test() {
    Vector* vector = vector_new(
		  1, 
		  (Duplicator) string_clone, 
		  (Destructor) string_free
		);

    // (+) just as simple, no extra code no macros
    // (+) type safety 
    vector_add_string(vector, string_from_format("foo %s", "bar"));
    string_println(vector_get_string(vector, 0));

    // Fails at compile time with invalid types
    // vector_add_string(vector, 10) 

    vector_free(vector);
}
```


## Usage

All standard headers are prefixed with `normalc/` so using a string would be
`normalc/string/string.h`. This allows for pseudo namespacing at the
preprocessor level. For examples, see the "test" directory.

## Tests

To run a test, run the `test.sh` in the test folder with the first argument
being the name of the test you want to run.

## Style

- Headers in the normalc src directory should use relative paths (e.g., `../string/string.h`)
- Structs, enums, and unions are capitalized camel case
- Variables and functions are lower snake case
- Structs use `typedef` to reduce code length, regardless of reduced transparency
- Any function that would break control flow (e.g., invalid null pointers) should use an assert statement like `ASSERT_NONULL` 
- Object functions should start with their object name (e.g., `string_new()`)

