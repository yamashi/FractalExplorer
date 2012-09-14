#pragma once
#define MPFR_LIB_NAME "mpfr"

#if defined(MPFR_LIB_DYNAMIC) || defined(__GMP_LIBGMP_DLL)
#  define MPFR_LIB_PREFIX
#elif defined(_WIN32)
#  define MPFR_LIB_PREFIX "lib"
#endif

#if defined(_WIN64)
#  define MPFR_LIB_PLATFORM "-x64"
#elif defined(_WIN32)
#  define MPFR_LIB_PLATFORM "-x86"
#endif

#ifdef _DLL
#  if defined(_DEBUG)
// MDd
#    define MPFR_LIB_RT_OPT "-gd"
#  else
// MD
#    define MPFR_LIB_RT_OPT
#  endif
#else
#  if defined(_DEBUG)
// MTd
#    define MPFR_LIB_RT_OPT "-sgd"
#  else
// MT
#    define MPFR_LIB_RT_OPT "-s"
#  endif
#endif

#  pragma comment(lib, MPFR_LIB_PREFIX MPFR_LIB_NAME  MPFR_LIB_RT_OPT MPFR_LIB_PLATFORM ".lib")
// static MDd x86 : libmpfr-gd-x86.lib
// static MD x86 :  libmpfr-x86.lib
// static MTd x86 : libmpfr-sgd-x86.lib
// static MT x86 :  libmpfr-s-x86.lib

// dynamic MDd x86 : mpfr-gd-x86.lib
// dynamic MD x86 :  mpfr-x86.lib
// dynamic MTd x86 : mpfr-sgd-x86.lib
// dynamic MT x86 :  mpfr-s-x86.lib
