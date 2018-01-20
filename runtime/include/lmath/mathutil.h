#ifndef IKE_MATHUTIL_H
#define IKE_MATHUTIL_H
#include <math.h>

#define TODEG(m) ((m)*57.2958f)
#define TORAD(m) ((m)*0.0174533f)
#define ROUND4(m) (ceilf((m) * 10000.f)/10000.f)

// MAX/MIN: https://stackoverflow.com/questions/3437404/min-and-max-in-c
#define ENSURE_size_t(i)    _Generic((i), long unsigned int:   (i))
#define ENSURE_luint(i)     _Generic((i), long unsigned int:   (i))
#define ENSURE_uint(i)      _Generic((i), unsigned int:   (i))
#define ENSURE_int(i)       _Generic((i), int:   (i))
#define ENSURE_float(f)     _Generic((f), float: (f))

#define GENERIC_MAX(x, y) ((x) > (y) ? (x) : (y))

#define MAX(type, x, y) \
  (type)GENERIC_MAX(ENSURE_##type(x), ENSURE_##type(y))

#define GENERIC_MIN(x, y) ((x) < (y) ? (x) : (y))

#define MIN(type, x, y) \
  (type)GENERIC_MIN(ENSURE_##type(x), ENSURE_##type(y))

#endif /* IKE_MATHUTIL_H */
