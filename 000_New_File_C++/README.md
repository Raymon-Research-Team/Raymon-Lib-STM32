# C / C++ Source Rename Helpers

These Windows batch files rename the conventional STM32 application source file between C and C++ extensions. They do not convert source code or make C code compatible with C++.

## Files

- `c2cpp.bat`: renames `main.c` to `main.cpp`.
- `cpp2c.bat`: renames `main.cpp` to `main.c`.

## Usage

1. Back up or commit your project first.
2. Copy the desired batch file into the directory containing the source file.
3. Run it from Command Prompt or File Explorer.
4. Confirm the expected filename exists, then update the IDE project/build configuration if necessary.

Run from the directory containing the file; the scripts use relative filenames.

## Important limitations

- These scripts only rename one file. They do not rewrite syntax, headers, linkage, or compiler settings.
- The rename can fail if the source file is missing or the destination already exists.
- C++ compilation may expose incompatible C constructs or name-mangling issues. Use `extern "C"` for C APIs that must retain C linkage.
- The reverse script does not translate C++ code back into valid C.

No automated build or compiler test is included in this folder.