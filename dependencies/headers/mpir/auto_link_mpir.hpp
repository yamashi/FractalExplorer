#pragma once
#define MPIR_LIB_NAME "mpir_"

#if defined(MPIR_LIB_DYNAMIC) || defined(__GMP_LIBGMP_DLL)
#  define MPIR_LIB_PREFIX
#elif defined(_WIN32)
#  define MPIR_LIB_PREFIX "lib"
#endif

#if defined(_M_X64)
#  define MPIR_LIB_PLATFORM "-x64"
#elif defined(_WIN32)
#  define MPIR_LIB_PLATFORM "-x86"
#endif

#ifdef _DLL
#  if defined(_DEBUG)
// MDd
#    define MPIR_LIB_RT_OPT "-gd"
#  else
// MD
#    define MPIR_LIB_RT_OPT
#  endif
#else
#  if defined(_DEBUG)
// MTd
#    define MPIR_LIB_RT_OPT "-sgd"
#  else
// MT
#    define MPIR_LIB_RT_OPT "-s"
#  endif
#endif

#if !defined(MPIR_LIB_DYNAMIC) && !defined(__GMP_LIBGMP_DLL)
#  pragma comment(lib, MPIR_LIB_PREFIX MPIR_LIB_NAME "cxx"  MPIR_LIB_RT_OPT MPIR_LIB_PLATFORM ".lib")
#endif

#if defined(_WIN64)

#  if defined(AMD_K8)
#    pragma comment(lib, MPIR_LIB_PREFIX MPIR_LIB_NAME "k8"  MPIR_LIB_RT_OPT MPIR_LIB_PLATFORM ".lib")
#  elif defined(AMD_K10)
#    pragma comment(lib, MPIR_LIB_PREFIX MPIR_LIB_NAME "k10"  MPIR_LIB_RT_OPT MPIR_LIB_PLATFORM ".lib")
#  elif defined(INTEL_CORE2)
#    pragma comment(lib, MPIR_LIB_PREFIX MPIR_LIB_NAME "core2"  MPIR_LIB_RT_OPT MPIR_LIB_PLATFORM ".lib")
#  elif defined(NEHALEM)
#    pragma comment(lib, MPIR_LIB_PREFIX MPIR_LIB_NAME "nehalem"  MPIR_LIB_RT_OPT MPIR_LIB_PLATFORM ".lib")
#  else
#    pragma comment(lib, MPIR_LIB_PREFIX MPIR_LIB_NAME "gc"  MPIR_LIB_RT_OPT MPIR_LIB_PLATFORM ".lib")
#  endif

#elif defined(_WIN32)

#  if defined(PENTIUM)
#    pragma comment(lib, MPIR_LIB_PREFIX MPIR_LIB_NAME "p0"  MPIR_LIB_RT_OPT MPIR_LIB_PLATFORM ".lib")
#  elif defined(PENTIUM_III)
#    pragma comment(lib, MPIR_LIB_PREFIX MPIR_LIB_NAME "p3"  MPIR_LIB_RT_OPT MPIR_LIB_PLATFORM ".lib")
#  elif defined(PENTIUM_IV)
#    pragma comment(lib, MPIR_LIB_PREFIX MPIR_LIB_NAME "p4"  MPIR_LIB_RT_OPT MPIR_LIB_PLATFORM ".lib")
#  else
#    pragma comment(lib, MPIR_LIB_PREFIX MPIR_LIB_NAME "gc"  MPIR_LIB_RT_OPT MPIR_LIB_PLATFORM ".lib")
// static MDd x86 : libmpir_gc-gd-x86.lib
// static MD x86 :  libmpir_gc-x86.lib
// static MTd x86 : libmpir_gc-sgd-x86.lib
// static MT x86 :  libmpir_gc-s-x86.lib

// dynamic MDd x86 : mpir_gc-gd-x86.lib
// dynamic MD x86 :  mpir_gc-x86.lib
// dynamic MTd x86 : mpir_gc-sgd-x86.lib
// dynamic MT x86 :  mpir_gc-s-x86.lib
#endif

#endif
