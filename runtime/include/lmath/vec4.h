#ifndef IKE_VEC4_H
#define IKE_VEC4_H
#include <math.h>
#include "vec3.h"
#include "vec2.h"

/**
 * \brief Vector of four dimentions.
 *
 * A defined union to hold references of X, Y, Z and W components  of a vector with three components,
 * accessible using (xyzw) and (rgba) notations.
 */
typedef union {
    struct {
        float x;
        float y;
        float z;
        float w;
    };
    struct {
        float r;
        float g;
        float b;
        float a;
    };
    float xy[2];
    float xyz[3];
    float xyzw[4];
    float rgb[3];
    float rgba[4];
} vec4;

static const vec4 vec4Zero = {0.0f, 0.0f, 0.0f, 0.0f};
static const vec4 vec4One = {1.0f, 1.0f, 1.0f, 1.0f};

/**
 * \brief Makes a vec4 using x, y and z components.
 *
 * Make a vector which components are equal to x, y, z and w params.
 * \param x new vector's X component.
 * \param y new vector's Y component.
 * \param z new vector's Z component.
 * \param w new vector's W component.
 * \return newly created vector (stack).
 */
static inline vec4 vec4Make(float x, float y, float z, float w) {
    vec4 r;
    r.x = x;
    r.y = y;
    r.z = z;
    r.w = w;
    return r;
}

/**
 * \brief Makes a vec4 using a vec3 and w component.
 *
 * Make a vector which components are equal to x, y, z (from vec3) and w params.
 * \param vec3 v origin of X, Y and Z components.
 * \param w new vector's W component.
 * \return newly created vector (stack).
 */
static inline vec4 vec4MakeVw(vec3 v, float w) {
    vec4 r;
    r.x = v.x;
    r.y = v.y;
    r.z = v.z;
    r.w = w;
    return r;
}

/**
 * \brief Makes a vec4 using a vec2, z and w components.
 *
 * Make a vector which components are equal to x, y (from vec2), z and w params.
 * \param vec2 v origin of X and Y components.
 * \param z new vector's Z component.
 * \param w new vector's W component.
 * \return newly created vector (stack).
 */
static inline vec4 vec4MakeVzw(vec2 v, float z, float w) {
    vec4 r;
    r.x = v.x;
    r.y = v.y;
    r.z = z;
    r.w = w;
    return r;
}

/**
 * \brief Add two vec4s.
 *
 * Adds two vec4 variables, returning the result as a new vector (on stack).
 * \param vec4 a is the left operand.
 * \param vec4 b is the right operand.
 * \return a vec4 with the sum of components of a and b.
 */
static inline vec4 vec4Add(const vec4 a, const vec4 b) {
    vec4 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    r.w = a.w + b.w;
    return r;
}

/**
 * \brief Substract two vec4s.
 *
 * Substracts two vec4 variables, returning the result as a new vector (on stack).
 * \param vec4 a is the left operand.
 * \param vec4 b is the right operand.
 * \return a vec4 with the substract of components of a minus b.
 */
static inline vec4 vec4Sub(const vec4 a, const vec4 b) {
    vec4 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    r.w = a.w - b.w;
    return r;
}

/**
 * \brief Scales a vec4.
 *
 * Multiplies all components by factor s, returning result as a new vector (on stack).
 * \param vec4 a is the vector to scale.
 * \param s is the scalar factor.
 * \return the scalation result.
 */
static inline vec4 vec4Scale(const vec4 a, const float s) {
    vec4 r;
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
    r.w = a.w * s;
    return r;
}

/**
 * \brief Adds the multiplication of each vector's components.
 *
 * Inner multiplication is not matematically defined at all, it's more like a handy function
 * to help calculate some other stuff.
 * \param vec4 a is the left operand.
 * \param vec4 a is the right operand.
 * \return the sum of the multiplication of each component from both vectors.
 */
static inline float vec4MulInner(const vec4 a, const vec4 b) {
    float r = 0.0f;
    r += a.x * b.x;
    r += a.y * b.y;
    r += a.z * b.z;
    r += a.w * b.w;
    return r;
}

/**
 * \brief Square of length of a vector.
 *
 * Calculates the squared length of a vector, use this function instead vec4Len when comparing or
 * doing operations that doesn't require the exact length.
 * \param vec4 a the vector.
 * \return it's squared length.
 */
static inline float vec4SqrLen(const vec4 a) {
    return vec4MulInner(a, a);
}

/**
 * \brief Calculate a vector's length.
 *
 * Standard vector length calculation, use only when you have to use the exact length otherwise prefer vec4SqrLen.
 * \param vec4 a the vector.
 * \return vector's length.
 */
static inline float vec4Len(const vec4 a) {
    return sqrtf(vec4SqrLen(a));
}

/**
 * \brief Normalizes a vector.
 *
 * Standard vector normalization (divide one by the vector's length).
 * \param vec4 a the vector normalize.
 * \return normalized vector.
 */
static inline vec4 vec4Norm(const vec4 a) {
    float k = 1.0f / vec4Len(a);
    return vec4Scale(a, k);
}

/**
 * \brief Gets the lesser values between two vectors.
 *
 * Creates a new vector (on stack) with the minimum quantities of a and b vectors.
 * \param vec2 a the left vector.
 * \param vec2 b the right vector.
 * \return a vector containing two minor values.
 */
static inline vec4 vec4Min(const vec4 a, const vec4 b) {
    vec4 r;
    r.x = a.x < b.x ? a.x : b.x;
    r.y = a.y < b.y ? a.y : b.y;
    r.z = a.z < b.z ? a.z : b.z;
    r.w = a.w < b.w ? a.w : b.w;
    return r;
}

/**
 * \brief Gets the greater values between two vectors.
 *
 * Creates a new vector (on stack) with the maximum quantities of a and b vectors.
 * \param vec4 a the left vector.
 * \param vec4 b the right vector.
 * \return a vector containing two major values.
 */
static inline vec4 vec4Max(const vec4 a, const vec4 b) {
    vec4 r;
    r.x = a.x > b.x ? a.x : b.x;
    r.y = a.y > b.y ? a.y : b.y;
    r.z = a.z > b.z ? a.z : b.z;
    r.w = a.w > b.w ? a.w : b.w;
    return r;
}

#endif /* IKE_VEC4_H */
