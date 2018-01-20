#ifndef IKE_VEC3_H
#define IKE_VEC3_H
#include <math.h>
#include "vec2.h"

/**
 * \brief Vector of three dimentions.
 *
 * A defined union to hold references of X, Y and Z components  of a vector with three components,
 * accessible using (xyz), (rgb) and (yaw,pitch,roll) notations.
 */
typedef union {
    struct {
        float x;
        float y;
        float z;
    };
    struct {
        float yaw;
        float pitch;
        float roll;
    };
    struct {
        float r;
        float g;
        float b;
    };
    float xy[2];
    float xyz[3];
    float rgb[3];
} vec3;

static const vec3 vec3Zero = {0.0f, 0.0f, 0.0f};
static const vec3 vec3Up = {0.0f, 1.0f, 0.0f};
static const vec3 vec3One = {1.0f, 1.0f, 1.0f};

/**
 * \brief Makes a vec3 using x, y and z components.
 *
 * Make a vector which components are equal to x, y and z params.
 * \param x new vector's X component.
 * \param y new vector's Y component.
 * \param z new vector's Z component.
 * \return newly created vector (stack).
 */
static inline vec3 vec3Make(float x, float y, float z) {
    vec3 r;
    r.x = x;
    r.y = y;
    r.z = z;
    return r;
}

/**
 * \brief Makes a vec3 using a vec3 and z component.
 *
 * Make a vector which components are equal to x, y (from vec2) and z params.
 * \param vec2 v origin of X and Y components.
 * \param z new vector's Z component.
 * \return newly created vector (stack).
 */
static inline vec3 vec3MakeVz(vec2 v, float z) {
    vec3 r;
    r.x = v.x;
    r.y = v.y;
    r.z = z;
    return r;
}

/**
 * \brief Add two vec3s.
 *
 * Adds two vec3 variables, returning the result as a new vector (on stack).
 * \param vec3 a is the left operand.
 * \param vec3 b is the right operand.
 * \return a vec3 with the sum of components of a and b.
 */
static inline vec3 vec3Add(const vec3 a, const vec3 b) {
    vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

/**
 * \brief Substract two vec3s.
 *
 * Substracts two vec3 variables, returning the result as a new vector (on stack).
 * \param vec3 a is the left operand.
 * \param vec3 b is the right operand.
 * \return a vec3 with the substract of components of a minus b.
 */
static inline vec3 vec3Sub(const vec3 a, const vec3 b) {
    vec3 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

/**
 * \brief Scales a vec3.
 *
 * Multiplies all components by factor s, returning result as a new vector (on stack).
 * \param vec3 a is the vector to scale.
 * \param s is the scalar factor.
 * \return the scalation result.
 */
static inline vec3 vec3Scale(const vec3 a, const float s) {
    vec3 r;
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
    return r;
}

/**
 * \brief Adds the multiplication of each vector's components.
 *
 * Inner multiplication is not matematically defined at all, it's more like a handy function
 * to help calculate some other stuff.
 * \param vec3 a is the left operand.
 * \param vec3 a is the right operand.
 * \return the sum of the multiplication of each component from both vectors.
 */
static inline float vec3MulInner(const vec3 a, const vec3 b) {
    float r = 0.0f;
    r += a.x * b.x;
    r += a.y * b.y;
    r += a.z * b.z;
    return r;
}

/**
 * \brief Square of length of a vector.
 *
 * Calculates the squared length of a vector, use this function instead vec3Len when comparing or
 * doing operations that doesn't require the exact length.
 * \param vec3 a the vector.
 * \return it's squared length.
 */
static inline float vec3SqrLen(const vec3 a) {
    return vec3MulInner(a, a);
}

/**
 * \brief Calculate a vector's length.
 *
 * Standard vector length calculation, use only when you have to use the exact length otherwise prefer vec3SqrLen.
 * \param vec3 a the vector.
 * \return vector's length.
 */
static inline float vec3Len(const vec3 a) {
    return sqrtf(vec3SqrLen(a));
}

/**
 * \brief Normalizes a vector.
 *
 * Standard vector normalization (divide one by the vector's length).
 * \param vec3 a the vector normalize.
 * \return normalized vector.
 */
static inline vec3 vec3Norm(const vec3 a) {
    float k = 1.0f / vec3Len(a);
    return vec3Scale(a, k);
}

/**
 * \brief Gets the lesser values between two vectors.
 *
 * Creates a new vector (on stack) with the minimum quantities of a and b vectors.
 * \param vec3 a the left vector.
 * \param vec3 b the right vector.
 * \return a vector containing two minor values.
 */
static inline vec3 vec3Min(const vec3 a, const vec3 b) {
    vec3 r;
    r.x = a.x < b.x ? a.x : b.x;
    r.y = a.y < b.y ? a.y : b.y;
    r.z = a.z < b.z ? a.z : b.z;
    return r;
}

/**
 * \brief Gets the greater values between two vectors.
 *
 * Creates a new vector (on stack) with the maximum quantities of a and b vectors.
 * \param vec3 a the left vector.
 * \param vec3 b the right vector.
 * \return a vector containing two major values.
 */
static inline vec3 vec3Max(const vec3 a, const vec3 b) {
    vec3 r;
    r.x = a.x > b.x ? a.x : b.x;
    r.y = a.y > b.y ? a.y : b.y;
    r.z = a.z > b.z ? a.z : b.z;
    return r;
}

/**
 * \brief Dot product between two vec3s.
 *
 * \param vec3 a the left vector.
 * \param vec3 b the right vector.
 * \return result of dot product.
 */
static inline float vec3Dot(const vec3 a, const vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * \brief Cross product between two vec3s.
 *
 * \param vec3 a the left vector.
 * \param vec3 b the right vector.
 * \return result of cross product.
 */
static inline vec3 vec3Cross(const vec3 a, const vec3 b) {
    vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

/**
 * \brief Calcualtes a reflection vector from a across b.
 *
 * \param vec3 a the left vector.
 * \param vec3 b the right vector.
 * \return reflection vector.
 */
static inline vec3 vec3Reflect(const vec3 d, const vec3 b) {
    vec3 n = vec3Norm(b);
    vec3 r = vec3Sub(d, vec3Scale(n, 2*vec3Dot(d, n)));
    return r;
}

#endif /* IKE_VEC3_H */
