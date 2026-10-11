# Windows / MSVC specific build settings.
#
# Included from the top-level CMakeLists.txt. Safe to include on any platform:
# everything here is guarded so non-Windows toolchains are unaffected.

if(NOT MSVC)
    return()
endif()

# Source files are stored as UTF-8 (they contain Chinese comments), while MSVC
# assumes the active code page. Force the compiler to read and emit UTF-8 so
# string literals and comments survive round-tripping.
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/utf-8>")

# Qt headers pull in a large amount of third-party code; keep the build output
# readable without weakening our own diagnostics.
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/MP>")
