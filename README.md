# MJSTR

MJSTR is an internal module designed for use in future projects. It is written in C++17.

## Setup

1. Download the appropriate package based on your system architecture:

    * For 64-bit systems, download `Bin-x64.zip`.
    * For 32-bit systems, download `Bin-x86.zip`.

2. Extract the downloaded package. You should see the following directories:

    * `bin` - Debug and release binaries.
    * `inc` - Include headers.

3. Include the `inc\mjstr` directory in your project as an additional include directory.
4. Link `bin\{Debug|Release}\mjstr.lib` library to your project.
5. Don't forget to include the `bin\{Debug|Relase}\mjstr.dll` in your project's
output directory if your application relies on it during runtime.

## Usage

To integrate MJSTR into your project, you can include the appropriate header files
based on your requirements:

* **<mjstr/api.hpp>**: This header contains export/import macro, don't include it directly.
* **<mjstr/char_traits.hpp>**: This header contains `char_traits<CharT>` structure.
* **<mjstr/conversion.hpp>**: This header contains functions for conversion between `byte_string`, `utf8_string` and `unicode_string`.
* **<mjstr/string.hpp>**: This header contains string class.
* **<mjstr/string_view.hpp>**: This header contains lightweight non-owning string class.

## Compatibility

MJSTR is compatible with all versions of Windows that support the `MultiByteToWideChar()`
and `WideCharToMultiByte()`, as these are the only WinAPI functions utilized in the project.
Additionally, the project requires support for C++17.

## Questions and support

If you have any questions, encouter issues, or need assistance with the MJSTR,
feel free to reach out. You can reach me through the `Issues` section or email
([mjandura03@gmail.com](mailto:mjandura03@gmail.com)).

## License

Copyright © Mateusz Jandura.

SPDX-License-Identifier: Apache-2.0