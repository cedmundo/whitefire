#ifndef IKE_QUAT_H
#define IKE_QUAT_H
#include <math.h>
#include "vec3.h"
#include "mat4.h"

/**
 * \brief Quaternion representation.
 *
 * A defined union to hold references of A1, A2, A3 and A4 components of a quaterion A, also accessibe
 * as 4-dimetion array.
 */
typedef union {
    struct {
        float w;
        float x; // xi
        float y; // yj
        float z; // zk
    };
    float wxyz[4];
} quat;

static const quat quatIdentity = {1, 0, 0, 0};

/**
 * \brief Makes a quat using x, y, z and w components.
 *
 * Make a vector which components are equal to x, y, z and w params.
 * \param w new quaternion's W component.
 * \param x new quaternion's X component.
 * \param y new quaternion's Y component.
 * \param z new quaternion's Z component.
 * \return newly created quaternion (stack).
 */
static inline quat quatMake(const float w, const float x, const float y, const float z) {
    quat r;
    r.w = w;
    r.x = x;
    r.y = y;
    r.z = z;
    return r;
}

/**
 * \brief Makes a quat using x, y and z euler angles.
 *
 * Converts euler angles to a quaternion.
 * \param x new quaternion's X angle.
 * \param y new quaternion's Y angle.
 * \param z new quaternion's Z angle.
 * \return newly created quaternion (stack).
 */
static inline quat quatMakeEuler(const vec3 angles) {
    quat r;
    const float x = angles.x*0.5f;
    const float y = angles.y*0.5f;
    const float z = angles.z*0.5f;
    const float sx = sinf(x);
    const float sy = sinf(y);
    const float sz = sinf(z);
    const float cx = cosf(x);
    const float cy = cosf(y);
    const float cz = cosf(z);

    r.w = cx * cy * cz + sx * sy * sz;
    r.x = sx * cy * cz - cx * sy * sz;
    r.y = cx * sy * cz + sx * cy * sz;
    r.z = cx * cy * sz - sx * sy * cz;

    return r;
}

/**
 * \brief Add two quaternions.
 *
 * Adds two quat variables, returning the result as a new vector (on stack).
 * \param quat a is the left operand.
 * \param quat b is the right operand.
 * \return a quaternion with the sum of components of a and b.
 */
static inline quat quatAdd(const quat a, const quat b) {
    quat r;
    r.w = a.w + b.w;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

/**
 * \brief Scales a quat.
 *
 * Multiplies all components by factor s, returning result as a new vector (on stack).
 * \param quat a is the vector to scale.
 * \param s is the scalar factor.
 * \return the scalation result.
 */
static inline quat quatScale(const quat a, const float s) {
    quat r;
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
    r.w = a.w * s;
    return r;
}

/**
 * \brief Cross product of two quaternions.
 *
 * Multiplies two quat variables, returning the result as a new quaternion.
 * \param quat a is the left operand.
 * \param quat b is the right operand.
 * \return multiplication result.
 */
static inline quat quatMul(const quat a, const quat b) {
    quat r;
    r.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
    r.x = a.w*b.x + a.x*b.w - a.y*b.z + a.z*b.y;
    r.y = a.w*b.y + a.x*b.z + a.y*b.w - a.z*b.x;
    r.z = a.w*b.z - a.x*b.y + a.y*b.x + a.z*b.w;
    return r;
}

/**
 * \brief Normalizes a quaternion.
 *
 * Normalize all components of a quaternion.
 * \param quat to normalize.
 * \return a normalized quaternion from param a.
 */
static inline quat quatNorm(const quat a) {
    quat r;
    float m = sqrtf(a.w*a.w + a.x*a.x + a.y*a.y + a.z*a.z);
    r.w = a.w/m;
    r.x = a.x/m;
    r.y = a.y/m;
    r.z = a.z/m;
    return r;
}

/**
 * \brief Conjugate a quaternion.
 *
 * Conjugates a quaternion.
 * \param quat to conjugate.
 * \return a conjugated quaternion from param a.
 */
static inline quat quatConjugate(const quat a) {
    quat r;
    r.w = a.w;
    r.x = -a.x;
    r.y = -a.y;
    r.z = -a.z;
    return r;
}

/**
 * \brief Dot product of two quaternions.
 *
 * Calcualtes dot prodcut between two quaternions.
 * \param quat a left operand.
 * \param quat a right operand.
 * \return dot product of two quaternions.
 */
static inline float quatDot(const quat a, const quat b) {
    return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
}

/**
 * \brief Inverse of a quaternion.
 *
 * \param quat a to invert.
 * \return a inverse quaternion of param a.
 */
static inline quat quatInverse(const quat a) {
    return quatScale(quatConjugate(a), 1/quatDot(a, a));
}

/**
 * \brief Length of a quaternion.
 *
 * \param quat to get length.
 * \return Magnitude (or length) of a quaternion.
 */
static inline float quatLen(const quat a) {
    return sqrtf(quatDot(a, a));
}

/**
 * \brief Make a quat from axis and angle.
 *
 * \param vec3 axis to rotate.
 * \param float angle.
 * \return A quaternion built from axis angle.
 */
static inline quat quatAxisAngle(const vec3 axis, const float angle) {
    const vec3 n = vec3Norm(axis);
    const float ra = angle * 0.5;
    const float sa = sinf(ra);
    return quatMake(cosf(ra), n.x * sa, n.y * sa, n.z * sa);
}

/**
 * \brief Quaternion to matrix representation.
 *
 * \param quat a to convert.
 * \return A *normalized* rotation matrix.
 */
static inline mat4 quatToMat4(const quat q) {
    mat4 r = mat4Identity;

    const float sqw = q.w*q.w;
    const float sqx = q.x*q.x;
    const float sqy = q.y*q.y;
    const float sqz = q.z*q.z;
    const float invs = 1.0f / (sqx + sqy + sqz + sqw);

	r.raw[0] = ( sqx - sqy - sqz + sqw) * invs;
    r.raw[5] = (-sqx + sqy - sqz + sqw) * invs;
    r.raw[10] = (-sqx - sqy + sqz + sqw) * invs;

    float tmp1 = q.x*q.y;
    float tmp2 = q.z*q.w;
    r.raw[4] = 2.0 * (tmp1 + tmp2) * invs;
    r.raw[1] = 2.0 * (tmp1 - tmp2) * invs;

    tmp1 = q.x*q.z;
    tmp2 = q.y*q.w;
    r.raw[8] = 2.0 * (tmp1 - tmp2) * invs;
    r.raw[2] = 2.0 * (tmp1 + tmp2) * invs;

    tmp1 = q.y*q.z;
    tmp2 = q.x*q.w;
    r.raw[9] = 2.0 * (tmp1 + tmp2) * invs;
    r.raw[6] = 2.0 * (tmp1 - tmp2) * invs;

    return mat4Transpose(r);
}

#endif /* IKE_QUAT_H */
