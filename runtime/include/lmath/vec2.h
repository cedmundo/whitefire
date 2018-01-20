#ifndef IKE_VEC2_H
#define IKE_VEC2_H
#include <math.h>

/**
 * \brief Vector of two dimentions.
 *
 * A defined union to hold references of X and Y components of a 2d vector. Both can
 * be accessible using x,y notation or u,v notation, also alternatively its possible to
 * access them with .xy member wich its a flat array containing both components.
 */
typedef union {
    struct {
        float x;
        float y;
    };
    struct {
        float u;
        float v;
    };
    float xy[2];
} vec2;

static const vec2 vec2Zero = {0.0f, 0.0f};
static const vec2 vec2One = {1.0f, 1.0f};

/**
 * \brief Make a vec2 using its x and y components.
 *
 * Stack vector which components are equal to x and y params.
 * \param x new vector's X component.
 * \param y new vector's Y component.
 * \return newly created vector (stack).
 */
static inline vec2 vec2Make(float x, float y) {
    vec2 r;
    r.x = x;
    r.y = y;
    return r;
}

/**
 * \brief Add two vec2s.
 *
 * Adds two vec2 variables, returning the result as a new vector (on stack).
 * \param vec2 a is the left operand.
 * \param vec2 b is the right operand.
 * \return a vec2 with the sum of components of a and b.
 */
static inline vec2 vec2Add(const vec2 a, const vec2 b) {
    vec2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

/**
 * \brief Substract two vec2s.
 *
 * Substracts two vec2 variables, returning the result as a new vector (on stack).
 * \param vec2 a is the left operand.
 * \param vec2 b is the right operand.
 * \return a vec2 with the substract of components of a minus b.
 */
static inline vec2 vec2Sub(const vec2 a, const vec2 b) {
    vec2 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

/**
 * \brief Scales a vector.
 *
 * Multiplies all components by factor s, returning result as a new vector (on stack).
 * \param vec2 a is the vector to scale.
 * \param s is the scalar factor.
 * \return the scalation result.
 */
static inline vec2 vec2Scale(const vec2 a, const float s) {
    vec2 r;
    r.x = a.x * s;
    r.y = a.y * s;
    return r;
}

/**
 * \brief Adds the multiplication of each vector's components.
 *
 * Inner multiplication is not matematically defined at all, it's more like a handy function
 * to help calculate some other stuff.
 * \param vec2 a is the left operand.
 * \param vec2 a is the right operand.
 * \return the sum of the multiplication of each component from both vectors.
 */
static inline float vec2MulInner(const vec2 a, const vec2 b) {
    float r = 0.0f;
    r += a.x * b.x;
    r += a.y * b.y;
    return r;
}

/**
 * \brief Square of length of a vector.
 *
 * Calculates the squared length of a vector, use this function instead vec2Len when comparing or
 * doing operations that doesn't require the exact length.
 * \param vec2 a the vector.
 * \return it's squared length.
 */
static inline float vec2SqrLen(const vec2 a) {
    return vec2MulInner(a, a);
}


/**
 * \brief Calculate a vector's length.
 *
 * Standard vector length calculation, use only when you have to use the exact length otherwise prefer vec2SqrLen.
 * \param vec2 a the vector.
 * \return vector's length.
 */
static inline float vec2Len(const vec2 a) {
    return sqrtf(vec2SqrLen(a));
}

/**
 * \brief Normalizes a vector.
 *
 * Standard vector normalization (divide one by the vector's length).
 * \param vec2 a the vector normalize.
 * \return normalized vector.
 */
static inline vec2 vec2Norm(const vec2 a) {
    float k = 1.0f / vec2Len(a);
    return vec2Scale(a, k);
}

/**
 * \brief Gets the lesser values between two vectors.
 *
 * Creates a new vector (on stack) with the minimum quantities of a and b vectors.
 * \param vec2 a the left vector.
 * \param vec2 b the right vector.
 * \return a vector containing two minor values.
 */
static inline vec2 vec2Min(const vec2 a, const vec2 b) {
    vec2 r;
    r.x = a.x < b.x ? a.x : b.x;
    r.y = a.y < b.y ? a.y : b.y;
    return r;
}

/**
 * \brief Gets the greater values between two vectors.
 *
 * Creates a new vector (on stack) with the maximum quantities of a and b vectors.
 * \param vec2 a the left vector.
 * \param vec2 b the right vector.
 * \return a vector containing two major values.
 */
static inline vec2 vec2Max(const vec2 a, const vec2 b) {
    vec2 r;
    r.x = a.x > b.x ? a.x : b.x;
    r.y = a.y > b.y ? a.y : b.y;
    return r;
}

#endif /* IKE_VEC2_H */
