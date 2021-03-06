# IMPORTANT NOTE:
#
# I have decided DO-CV will work on recent C++ compilers because C++11 features
# are used. Code is much more elegant, more concise, easier to maintain.
#
# C++11 used features:
# - auto
# - lambda
#

do_step_message("Found ${CMAKE_CXX_COMPILER_ID} compiler:")

# Visual C++ compiler
if (MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
  message(STATUS "  - NON-SECURE warnings are disabled.")
  add_definitions(/EHsc)
  message(STATUS "  - Using /EHsc: catches C++ exceptions only and tells the compiler to assume that extern C functions never throw a C++ exception.")
  if (MSVC_VERSION EQUAL 1700)
    message(STATUS "  - Using version 2012: setting '_VARIADIC_MAX=10' to compile 'Google Test'")
    add_definitions(/D _VARIADIC_MAX=10)
  endif ()

# Clang compiler
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set (ENABLE_CXX11 "-std=c++11 -stdlib=libc++")

# GNU compiler
elseif (CMAKE_COMPILER_IS_GNUCXX)               
  exec_program(${CMAKE_C_COMPILER} ARGS "-dumpversion" OUTPUT_VARIABLE _gcc_version_info)
  string(REGEX REPLACE "^([0-9]+).*$"                   "\\1" GCC_MAJOR ${_gcc_version_info})
  string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*$"          "\\1" GCC_MINOR ${_gcc_version_info})
  string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" GCC_PATCH ${_gcc_version_info})
  if(GCC_PATCH MATCHES "\\.+")
    set(GCC_PATCH "")
  endif()
  if(GCC_MINOR MATCHES "\\.+")
    set(GCC_MINOR "")
  endif()
  if(GCC_MAJOR MATCHES "\\.+")
    set(GCC_MAJOR "")
  endif()
  set(GCC_VERSION "${GCC_MAJOR}.${GCC_MINOR}")
  
  do_substep_message("${CMAKE_CXX_COMPILER_ID} compiler version: ${GCC_VERSION}")
  if (GCC_VERSION  VERSION_LESS 4.5)
    message(FATAL_ERROR "GNU compiler version lower than 4.5 are not supported anymore: C++0x features (auto and lambda) are needed.")
  elseif (GCC_VERSION  VERSION_LESS 4.7)
    set(ENABLE_CXX11 "-std=c++0x")
  else ()
    set (ENABLE_CXX11 "-std=c++11")
  endif ()
else ()
  message("WARNING: Compiler '${CMAKE_CXX_COMPILER}' may not be supported by DO-CV. Make sure that C++0x features are needed (auto and lambda) and adjust the CMake variable 'ENABLE_CXX11'. Otherwise, report back to me: david.ok8@gmail.com and I'll try to do what I can.")
endif ()

if (UNIX)
  # Base compilation flags.
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wunused-variable -fPIE")
  # Additional flags for Release builds.
  set(CMAKE_CXX_RELEASE_FLAGS "-03 -ffast-math")
  # Additional flags for Debug builds, which include code coverage.
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -g -DDEBUG -D_DEBUG --coverage -fprofile-arcs -ftest-coverage -fno-inline")
endif ()


do_step_message("Activating C++11 features with: '${ENABLE_CXX11}'")
