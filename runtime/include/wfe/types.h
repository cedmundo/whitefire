#ifndef WFE_TYPES_H
#define WFE_TYPES_H

// Any type representation.
typedef void* wfeAny;

// Raw data representation.
typedef char wfeData;

// Size with aligment definitions.
#ifdef WFE_USE_STDDEF
    #include <stddef.h>
    #include <stdalign.h>
    typedef size_t wfeSize;
    #define wfeOffsetOf(st, m) (offsetof(st, m))
    #define wfeAlignOf(type) (alignof(type))
#elif defined(WFE_USE_MSVSCDEF)
    #include <stdlib.h>
    typedef size_t wfeSize;
    #define wfeOffsetOf(st, m) (offsetof(st, m))
    #define wfeAlignOf(type) (__alignof(type))
#else
    #include <stdlib.h>
    typedef size_t wfeSize;
    #define wfeOffsetOf(st, m) ((size_t)&(((st *)0)->m))
    #define wfeAlignOf(type) (wfeOffsetOf(struct{char c;type member;}, member))
#endif

// Integer definition.
#ifdef WFE_USE_STDINT
    #include <stdint.h>
    typedef int16_t wfeInt16;
    typedef int32_t wfeInt32;
    typedef uint32_t wfeUint32;
    typedef int64_t wfeInt64;
    typedef uint64_t wfeUint64;
    typedef int64_t wfeInt;
    typedef uint64_t wfeUint;
#elif defined(WFE_USE_MSVSCINT)
    typedef __int16 wfeInt16;
    typedef __int32 wfeInt32;
    typedef unsigned __int32 wfeUint32;
    typedef __int64 wfeInt64;
    typedef unsigned __int64 wfeUint64;
    typedef __int64 wfeInt;
    typedef unsigned __int64 wfeUint;
#else
#warning "Integer type size is not guaranteed"
    typedef short wfeInt16;
    typedef int wfeInt32;
    typedef unsigned int wfeUint32;
    typedef long wfeInt64;
    typedef unsigned long wfeUint64;
    typedef long wfeInt;
    typedef unsigned long wfeUint;
#endif

// Floating definition.
typedef float wfeFloat32;
typedef double wfeFloat64;
#ifdef WFE_USE_FLOATNUM
typedef float wfeNum;
#else
typedef double wfeNum;
#endif

// Boolean definition.
typedef int wfeBool;
#define WFE_TRUE (1)
#define WFE_FALSE (0)

// Error definition.
typedef wfeInt64 wfeError;
#define WFE_SUCCESS ((wfeError) 0x0L)
#define WFE_FAILURE ((wfeError) 0x1L)
#define WFE_MEMORY_ERROR ((wfeError) 0x3L)
#define WFE_FILE_ERROR ((wfeError) 0x5L)

#define WFE_MAKE_FAILURE(code) (WFE_FAILURE | (code<<8))
#define WFE_MAKE_MEMORY_ERROR(code) (WFE_MEMORY_ERROR | (code<<8))
#define WFE_MAKE_FILE_ERROR(code) (WFE_FILE_ERROR | (code<<8))

#define WFE_CHECK_FLAG(value, bitindex) ((value & (1 << bitindex)) != 0)
#define WFE_HAS_FAILED(value) WFE_CHECK_FLAG(value, 0)
#define WFE_HAS_MEMORY_ERROR(value) WFE_CHECK_FLAG(value, 1)
#define WFE_HAS_FILE_ERROR(value) WFE_CHECK_FLAG(value, 2)

#define WFE_USER_ERROR_CODE(value) (value>>8)
#endif
