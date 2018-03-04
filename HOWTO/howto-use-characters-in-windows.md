## How to use characters in windows

``` c
// UNICODE support in standard C functions
#define _UNICODE
#include <tchar.h>
#include <wchar.h>
```

printf

wprintf


### Linux vs Win unicode strings

[what-is-linux-equivalent-of-lptstr-lpcstr](https://stackoverflow.com/questions/21574670/what-is-linux-equivalent-of-lptstr-lpcstr)

In Linux you don't usually use wchar_t for library API functions. Most libraries use UTF-8 encoded strings, so they take as strings plain arrays of NUL-terminated chars (IMO that is far better than duplicating all the functions with ANSI and Unicode versions).

So, with that in mind:

    LPCTSTR, LPCSTR, LPCWSTR -> const char *.
    LPTSTR, LPSTR, LPWSTR -> char *.

If you insist in using Unicode functions, MS style, you have to be aware that they actually use UTF-16 encoded strings, and wchar_t is not a portable type, as its size is not specified by the language. Instead you can use uint16_t:

    LPCWSTR -> const uint16_t *.
    LPWSTR -> uint16_t *.

And if you want to be extra MS compatible, you can use the UNICODE macro to conditionally typedef the LPTSTR and LPTCSTR into one of the others, but that's probably unneeded for your problem.


### [Windows Data Types for Strings](https://msdn.microsoft.com/en-us/library/windows/desktop/dd374131(v=vs.85).aspx)

// Generic types

#ifdef UNICODE
    typedef wchar_t TCHAR;
#else
    typedef unsigned char TCHAR;
#endif

typedef TCHAR *LPTSTR, *LPTCH;

// 8-bit character specific

typedef unsigned char CHAR;
typedef CHAR *LPSTR, *LPCH;

// Unicode specific (wide characters)

typedef unsigned wchar_t WCHAR;
typedef WCHAR *LPWSTR, *LPWCH;


### Links

Windows Data Types
- https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx

The Complete Guide to C++ Strings, Part I - Win32 Character Encodings
- https://www.codeproject.com/Articles/2995/The-Complete-Guide-to-C-Strings-Part-I-Win-Chara

wmain is not supported in mingw, fix:
- https://github.com/coderforlife/mingw-unicode-main/blob/master/mingw-unicode.c

Unicode output on windows console
- https://stackoverflow.com/questions/28216113/unicode-output-on-windows-console

Output unicode strings in Windows console app
- https://stackoverflow.com/questions/2492077/output-unicode-strings-in-windows-console-app

Unicode part 1: Windows console i/o approaches
- https://alfps.wordpress.com/2011/11/22/unicode-part-1-windows-console-io-approaches/
