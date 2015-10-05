/*
 *
 * Copyright (c) 2005-2015 Imperas Software Ltd., www.imperas.com
 *
 * The contents of this file are provided under the Software License Agreement
 * that you accepted before downloading this file.
 *
 * This header forms part of the Software but may be included and used unaltered
 * in derivative works.
 *
 * For more information, please visit www.OVPworld.org or www.imperas.com
 */

/* Please note that
 * __INTEL_COMPILER, __BORLANDC__ and _MSC_VER
 * conditionals are included for the users's convenience, but are not supported by Imperas.
 */

#include "hostapi/hostMacros.h"

#ifndef IMPTYPES_H
#define IMPTYPES_H

#if defined(__CYGWIN__)
    #error "Please use -mno-cygwin flag to gcc/cygwin compiler"
#endif

#if (IMPERAS_WINDOWS==1)
    #define IMPERAS_UNAME  "Windows"
    #define IMPERAS_SHRSUF "dll"
#else
    #define IMPERAS_UNAME  "Linux"
    #define IMPERAS_SHRSUF "so"
#endif

#if (IMPERAS_WINDOWS==1)

    #if defined(__INTEL_COMPILER)

        #define FUNC_NAME __FUNCTION__

        #define __uint8_t   unsigned __int8
        #define __int8_t    __int8
        #define __uint16_t  unsigned __int16
        #define __int16_t   __int16
        #define __uint32_t  unsigned __int32
        #define __int32_t   __int32
        #define __uint64_t  unsigned __int64
        #define __int64_t   __int64

        // get rid of the GCCism __attribute__
        #define __attribute__(X)
    #elif defined(__BORLANDC__)

        #define FUNC_NAME __FUNCTION__

        #define __uint8_t   unsigned char
        #define __int8_t    signed   char
        #define __uint16_t  unsigned short
        #define __int16_t   signed   short
        #define __uint32_t  unsigned int
        #define __int32_t   signed   int
        #define __uint64_t  unsigned __int64
        #define __int64_t   signed   __int64

        // get rid of the GCCism __attribute__
        #define __attribute__(X)
    #elif defined(_MSC_VER)
        #define FUNC_NAME __FUNCTION__

        #define __uint8_t   unsigned __int8
        #define __int8_t    __int8
        #define __uint16_t  unsigned __int16
        #define __int16_t   __int16
        #define __uint32_t  unsigned __int32
        #define __int32_t   __int32
        #define __uint64_t  unsigned __int64
        #define __int64_t   __int64
        #define __int128_t   __int64_t
        #define __uint128_t  __uint64_t
        #define inline __inline

        // get rid of the GCCism __attribute__
        #define __attribute__(X)
    #else   // Gcc x compile
        #define FUNC_NAME __FUNCTION__

        #define __uint8_t   unsigned char
        #define __int8_t    signed   char
        #define __uint16_t  unsigned short
        #define __int16_t   signed   short
        #define __uint32_t  unsigned int
        #define __int32_t   signed   int
        #define __uint64_t  unsigned long long
        #define __int64_t   signed   long long

        #ifndef NORETURN
            #define NORETURN __attribute__((noreturn))
        #endif
    #endif

#else

    #define FUNC_NAME __func__

    #ifndef NORETURN
        #define NORETURN __attribute__((noreturn))
    #endif

#endif

#include <sys/types.h>

typedef signed char   SChar;
typedef unsigned char UChar;
typedef __int8_t      Int8;
typedef __uint8_t     Uns8;
typedef __int16_t     Int16;
typedef __uint16_t    Uns16;
typedef __int32_t     Int32;
typedef __uint32_t    Uns32;
typedef __int64_t     Int64;
typedef __uint64_t    Uns64;
#if(IMPERAS_64_BIT_HOST)
    typedef __int128_t    Int128;
    typedef __uint128_t   Uns128;
#endif
typedef float         Flt32;
typedef double        Flt64;
typedef long double   Flt80;

//
// Boolean data types
//
#if defined(__cplusplus)
    typedef bool  Bool;
    #define False false
    #define True  true
#else
    #include <sys/types.h>
#ifdef _MSC_VER
    typedef int _Bool;
#endif
    typedef _Bool Bool;
    #define False ((Bool)0)
    #define True  ((Bool)1)
#endif

//
// Host-dependent data types
//
#if(IMPERAS_64_BIT_HOST==1)
    #define IMPERAS_POINTER_BYTES 8
    #define IMPERAS_POINTER_BITS  (IMPERAS_POINTER_BYTES*8)
    typedef Uns64                 UnsPS;
    typedef Int64                 IntPS;
    typedef Uns128                UnsPSx2;
    typedef Int128                IntPSx2;
#else
    #define IMPERAS_POINTER_BYTES 4
    #define IMPERAS_POINTER_BITS  (IMPERAS_POINTER_BYTES*8)
    typedef Uns32                 UnsPS;
    typedef Int32                 IntPS;
    typedef Uns64                 UnsPSx2;
    typedef Int64                 IntPSx2;
#endif

//
// Function attributes
//
#ifndef FASTCALL
    #define FASTCALL __attribute__((fastcall))
#endif

//
// Simulated address types
//
typedef Uns64 Addr;
typedef Int64 Offset;

//
// Non-portable format strings
//
#if (IMPERAS_WINDOWS==1)
    #if (IMPERAS_POINTER_BITS == 64)
        #define FMT_SIZETd "%I64d"
        #define FMT_PTRd   "%I64d"
        #define FMT_PTRu   "%I64u"
        #define FMT_PTRx   "%I64x"
        #define FMT_64d    "%I64d"
        #define FMT_64u    "%I64u"
        #define FMT_64x    "%I64x"
        #define FMT_64X    "%I64X"
        #define FMT_6402x  "%02I64x"
        #define FMT_6404x  "%04I64x"
        #define FMT_6408x  "%08I64x"
        #define FMT_640Nx  "%016I64x"
        #define FMT_648d   "%8I64d"
        #define FMT_648u   "%8I64u"
        #define FMT_6412d  "%12I64d"
        #define FMT_64Nd   "%16I64d"
        #define FMT_64Nu   "%16I64u"
        #define FMT_640Nd  "%016I64d"
        #define FMT_640No  "%024I64o"
        #define FMT_6406u  "%06I64u"
        #define FMT_NATu   "%08I64u"
        #define FMT_NAT4u  "%04I64u"
        #define FMT_NAT8u  "%08I64u"
        #define FMT_NATx   "%08I64x"
    #elif (IMPERAS_POINTER_BITS == 32)
        #define FMT_SIZETd "%d"
        #define FMT_PTRd   "%d"
        #define FMT_PTRu   "%u"
        #define FMT_PTRx   "%x"
        #define FMT_64d    "%I64d"
        #define FMT_64u    "%I64u"
        #define FMT_64x    "%I64x"
        #define FMT_64X    "%I64X"
        #define FMT_6402x  "%02I64x"
        #define FMT_6404x  "%04I64x"
        #define FMT_6408x  "%08I64x"
        #define FMT_640Nx  "%016I64x"
        #define FMT_648d   "%8I64d"
        #define FMT_648u   "%8I64u"
        #define FMT_6412d  "%12I64d"
        #define FMT_64Nd   "%16I64d"
        #define FMT_64Nu   "%16I64u"
        #define FMT_640Nd  "%016I64d"
        #define FMT_640No  "%024I64o"
        #define FMT_6406u  "%06I64u"
        #define FMT_NATu   "%u"
        #define FMT_NAT4u  "%4u"
        #define FMT_NAT8u  "%8u"
        #define FMT_NATx   "%x"
    #endif
#else
    #if (IMPERAS_POINTER_BITS == 64)
        #define FMT_SIZETd "%ld"
        #define FMT_PTRd   "%ld"
        #define FMT_PTRu   "%lu"
        #define FMT_PTRx   "%lx"
        #define FMT_64d    "%ld"
        #define FMT_64u    "%lu"
        #define FMT_64x    "%lx"
        #define FMT_64X    "%lX"
        #define FMT_6402x  "%02lx"
        #define FMT_6404x  "%04lx"
        #define FMT_6408x  "%08lx"
        #define FMT_64012x "%012lx"
        #define FMT_640Nx  "%016lx"
        #define FMT_648d   "%8ld"
        #define FMT_648u   "%8lu"
        #define FMT_6412d  "%12ld"
        #define FMT_64Nd   "%16ld"
        #define FMT_64Nu   "%16lu"
        #define FMT_640Nd  "%016ld"
        #define FMT_640No  "%024lo"
        #define FMT_6406u  "%06lu"
        #define FMT_NATu   "%lu"
        #define FMT_NAT4u  "%4lu"
        #define FMT_NAT8u  "%8lu"
        #define FMT_NATx   "%lx"
    #elif (IMPERAS_POINTER_BITS == 32)
        #define FMT_SIZETd "%d"
        #define FMT_PTRd   "%d"
        #define FMT_PTRu   "%u"
        #define FMT_PTRx   "%x"
        #define FMT_64d    "%lld"
        #define FMT_64u    "%llu"
        #define FMT_64x    "%llx"
        #define FMT_64X    "%llX"
        #define FMT_6402x  "%02llx"
        #define FMT_6404x  "%04llx"
        #define FMT_6408x  "%08llx"
        #define FMT_64012x "%012llx"
        #define FMT_640Nx  "%016llx"
        #define FMT_648d   "%8lld"
        #define FMT_648u   "%8llu"
        #define FMT_6412d  "%12lld"
        #define FMT_64Nd   "%16lld"
        #define FMT_64Nu   "%16llu"
        #define FMT_640Nd  "%016lld"
        #define FMT_640No  "%024llo"
        #define FMT_6406u  "%06llu"
        #define FMT_NATu   "%u"
        #define FMT_NAT4u  "%4u"
        #define FMT_NAT8u  "%8u"
        #define FMT_NATx   "%x"
    #endif
#endif

//
// Format strings for Addr type
//
#define FMT_Ad   FMT_64d
#define FMT_Au   FMT_64u
#define FMT_Ax   FMT_64x
#define FMT_AX   FMT_64X
#define FMT_A08x FMT_6408x
#define FMT_A0Nx FMT_640Nx

//
// Format strings for double type
//
#define FMT_DBL   "%e"

//
// Format string for 64b hex constant
//
#define FMT_64_CONST "0x" FMT_64x "LL"

//
// Address size
//
#ifndef ADDRESS_SIZE
    #define ADDRESS_SIZE (Uns32)(sizeof(Addr)*8)
#endif

//
// 2 byte endian swap
//
#define SWAP_2_BYTE(_V) \
    ({                                                              \
        Uns16 _R16;                                                 \
        Uns16 _V16 = _V;                                            \
        __asm__ ("rorw $8, %w0" : "=r" (_R16) : "0" (_V16) : "cc"); \
        _R16;                                                       \
    })

//
// 4 byte endian swap
//
#define SWAP_4_BYTE(_V) \
    ({                                                              \
        Uns32 _R32;                                                 \
        Uns32 _V32 = _V;                                            \
        __asm__ ("bswap %0" : "=r" (_R32) : "0" (_V32));            \
        _R32;                                                       \
    })

//
// 8 byte endian swap
//
#define SWAP_8_BYTE(_V) \
    ({                                                              \
        union {Uns64 u64; Uns32 u32[2];} _R64;                      \
        union {Uns64 u64; Uns32 u32[2];} _V64 = {_V};               \
        _R64.u32[0] = SWAP_4_BYTE(_V64.u32[1]);                     \
        _R64.u32[1] = SWAP_4_BYTE(_V64.u32[0]);                     \
        _R64.u64;                                                   \
    })

//
// 16 byte endian swap
//
#if(IMPERAS_64_BIT_HOST)
#define SWAP_16_BYTE(_V) \
    ({                                                              \
        union {Uns128 u128; Uns64 u64[2];} _R128;                   \
        union {Uns128 u128; Uns64 u64[2];} _V128 = {_V};            \
        _R128.u64[0] = SWAP_8_BYTE(_V128.u64[1]);                   \
        _R128.u64[1] = SWAP_8_BYTE(_V128.u64[0]);                   \
        _R128.u128;                                                 \
    })
#endif

//
// pointer-sized endian swap
//
#if(IMPERAS_64_BIT_HOST)
    #define SWAP_PS_BYTE(_V) SWAP_16_BYTE(_V)
#else
    #define SWAP_PS_BYTE(_V) SWAP_8_BYTE(_V)
#endif

//
// 2 byte conditional endian swap
//
#define SWAP_2_BYTE_COND(_V, _DO_SWAP) \
    ({                                                              \
        Uns16 _V16C = _V;                                           \
        _DO_SWAP ? SWAP_2_BYTE(_V16C) : _V16C;                      \
    })

//
// 4 byte conditional endian swap
//
#define SWAP_4_BYTE_COND(_V, _DO_SWAP) \
    ({                                                              \
        Uns32 _V32C = _V;                                           \
        _DO_SWAP ? SWAP_4_BYTE(_V32C) : _V32C;                      \
    })

//
// 8 byte conditional endian swap
//
#define SWAP_8_BYTE_COND(_V, _DO_SWAP) \
    ({                                                              \
        Uns64 _V64C = _V;                                           \
        _DO_SWAP ? SWAP_8_BYTE(_V64C) : _V64C;                      \
    })

//
// 8 byte conditional endian swap
//
#if(IMPERAS_64_BIT_HOST)
#define SWAP_16_BYTE_COND(_V, _DO_SWAP) \
    ({                                                              \
        Uns128 _V128C = _V;                                         \
        _DO_SWAP ? SWAP_16_BYTE(_V128C) : _V128C;                   \
    })
#endif

//
// pointer-sized conditional endian swap
//
#if(IMPERAS_64_BIT_HOST)
    #define SWAP_PS_BYTE_COND(_V, _DO_SWAP) SWAP_16_BYTE_COND(_V, _DO_SWAP)
#else
    #define SWAP_PS_BYTE_COND(_V, _DO_SWAP) SWAP_8_BYTE_COND(_V, _DO_SWAP)
#endif

#endif /*IMPTYPES_H*/