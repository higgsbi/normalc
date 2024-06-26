# Normalc

## Build

The normalc standard library is only available as a static library
For installation, CMake 3.0 is a prerequisite.
You can install manually as follows:

```bash
git clone https://github.com/higgsbi/normalc/
cd normalc
mkdir build
cd build
cmake ..
sudo make install
```

For local installations without root permissions, cmake commands 
should be alterted to pick a local prefix. You can edit the following script 
for ease of use:

```bash
git clone https://github.com/higgsbi/normalc/
cd normalc
mkdir build
cd build
cmake -D CMAKE_INSTALL_PREFIX:PATH=(YOUR__PATH_HERE) ..
make install
```

Or, you can use the simplified `install.sh` script in the root folder below:

```bash
/bin/bash -c "$(curl -fs https://raw.githubusercontent.com/higgsbi/normalc/main/install.sh)"
```

## Usage

All standard headers are prefixed with `normalc/` so using a string would be
`normalc/string/string.h`. This allows for pseudo namespacing at the
preprocessor level. For examples, see the "Tests" header.

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
