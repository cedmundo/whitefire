#ifndef WFE_TYPES_H
#define WFE_TYPES_H

// Any type representation.
typedef void* wfeAny;

// Raw data representation.
typedef char wfeData;

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
#warning "Either stdint nor msvsc configs are available for int definition, type size is not guaranteed"
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
typedef double wfeNumeric;

// Boolean definition.
typedef int wfeBool;
#define WFE_TRUE (1)
#define WFE_FALSE (0)

// Character definition.
#ifdef WFE_USE_UCHAR
#include <uchar.h>
#else
typedef wfeInt16 char16_t;
typedef wfeInt32 char32_t;
#endif
typedef char16_t wfeChar16;
typedef char32_t wfeChar32;

// Error definition.
typedef wfeInt64 wfeError;

#include <stddef.h>
typedef size_t wfeSize;
#ifdef WFE_USE_C11_ALIGNOF
    #define wfeAlignOf(type) (alignof(type))
#else
    #define wfeAlignOf(type) (offsetof(struct{char c;type member;}, member))
#endif

#endif
