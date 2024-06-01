# Normalc

## Build

The normalc standard library is only available as a static library
To install, CMake 3.0 is a prerequisite.

```bash
mkdir build
cd build
cmake ..
sudo make install
```

## Usage

All standard headers are prefixed with `normalc/` so using a string would be
`normalc/string/string.h`. This allows for pseudo namespacing at the
preprocessor level.

## Tests

To run a test, run the `test.sh` in the test folder with the first argument
being the name of the test you want to run.

## Style

- Headers in the standard library should use relative paths (e.g., `../string/string.h`)
- Structs, enums, and unions are capitalized camel case
- Variables and functions are lower snake case
- Structs use `typedef` to reduce code length, regardless of reduced transparency
- Functions must take in a pointer destination as parameters, rather than as a return type
- Any function that can produce an error could be handled, must return an error
- Any function that would break control flow (e.g., invalid null pointers) should use an assert statement like `ASSERT_NONULL` 
