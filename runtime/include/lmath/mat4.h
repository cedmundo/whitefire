#ifndef IKE_MATH4_H
#define IKE_MATH4_H
#include <math.h>
#include <assert.h>
#include "vec3.h"
#include "vec4.h"

/**
 * \brief 4x4 Matrix representation.
 *
 * A flat array containing a 16-size element matrix (4 columns by 4 rows).
 * Data is accessed as column-major so should not be transposed to be used with OpenGL API.
 */
typedef union _mat4 {
    struct {
        float xx, xy, xz, xw;
        float yx, yy, yz, yw;
        float zx, zy, zz, zw;
        float wx, wy, wz, ww;
    };
    float raw[16];
} mat4;

static const mat4 mat4Zero = {
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
};

static const mat4 mat4Identity = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

/**
 * \brief Gets a matrix row by its index.
 *
 * \param mat4 m, matrix to lookup in.
 * \param int i, row index, between 0 and 3. Warning: do not use quantities outside range.
 * \return a vec4 containing the full row.
 */
static inline vec4 mat4Row(const mat4 m, const unsigned int i) {
    assert(i <= 4);

    vec4 r;
    r.x = m.raw[(i*4)]; // +0
    r.y = m.raw[(i*4)+1];
    r.z = m.raw[(i*4)+2];
    r.w = m.raw[(i*4)+3];
    return r;
}

/**
 * \brief Gets a matrix column by its index.
 *
 * \param mat4 m, matrix to lookup in.
 * \param int i, column index, between 0 and 3. Warning: do not use quantities outside range.
 * \return a vec4 containing the full column.
 */
static inline vec4 mat4Col(const mat4 m, const unsigned int i) {
    assert(i <= 3);

    vec4 r;
    r.x = m.raw[i]; // +0
    r.y = m.raw[i+4];
    r.z = m.raw[i+8];
    r.w = m.raw[i+12];
    return r;
}

/**
 * \brief Transposes a matrix.
 *
 * \param mat4 m matrix to be transposed (not modified).
 * \return transposed matrix.
 */
static inline mat4 mat4Transpose(const mat4 m) {
    mat4 r;

    r.xx = m.xx; r.xy = m.yx; r.xz = m.zx; r.xw = m.wx;
    r.yx = m.xy; r.yy = m.yy; r.yz = m.zy; r.yw = m.wy;
    r.zx = m.xz; r.zy = m.yz; r.zz = m.zz; r.zw = m.wz;
    r.wx = m.xw; r.wy = m.yw; r.wz = m.zw; r.ww = m.ww;

    return r;
}

/**
 * \brief Adds two matrices.
 * \param mat4 a left operand.
 * \param mat4 b right operand.
 * \return a new stack matrix with a+b components result.
 */
static inline mat4 mat4Add(const mat4 a, const mat4 b){
    mat4 r;
    r.xx = a.xx + b.xx; r.xy = a.xy + b.xy; r.xz = a.xz + b.xz; r.xw = a.xw + b.xw;
    r.yx = a.yx + b.yx; r.yy = a.yy + b.yy; r.yz = a.yz + b.yz; r.yw = a.yw + b.yw;
    r.zx = a.zx + b.zx; r.zy = a.zy + b.zy; r.zz = a.zz + b.zz; r.zw = a.zw + b.zw;
    r.wx = a.wx + b.wx; r.wy = a.wy + b.wy; r.wz = a.wz + b.wz; r.ww = a.ww + b.ww;
    return r;
}

/**
 * \brief Substracts two matrices.
 * \param mat4 a left operand.
 * \param mat4 b right operand.
 * \return a new stack matrix with a-b components result.
 */
static inline mat4 mat4Sub(const mat4 a, const mat4 b){
    mat4 r;
    r.xx = a.xx - b.xx; r.xy = a.xy - b.xy; r.xz = a.xz - b.xz; r.xw = a.xw - b.xw;
    r.yx = a.yx - b.yx; r.yy = a.yy - b.yy; r.yz = a.yz - b.yz; r.yw = a.yw - b.yw;
    r.zx = a.zx - b.zx; r.zy = a.zy - b.zy; r.zz = a.zz - b.zz; r.zw = a.zw - b.zw;
    r.wx = a.wx - b.wx; r.wy = a.wy - b.wy; r.wz = a.wz - b.wz; r.ww = a.ww - b.ww;
    return r;
}

/**
 * \brief Scales a matrix by a scalar factor.
 * \param mat4 a matrix to scale.
 * \param float s scalar factor.
 * \return new stack scaled matrix.
 */
static inline mat4 mat4Scale(const mat4 a, float s) {
    mat4 r;
    r.xx = a.xx * s; r.xy = a.xy * s; r.xz = a.xz * s; r.xw = a.xw * s;
    r.yx = a.yx * s; r.yy = a.yy * s; r.yz = a.yz * s; r.yw = a.yw * s;
    r.zx = a.zx * s; r.zy = a.zy * s; r.zz = a.zz * s; r.zw = a.zw * s;
    r.wx = a.wx * s; r.wy = a.wy * s; r.wz = a.wz * s; r.ww = a.ww * s;
    return r;
}

/**
 * \brief Multiplies two matrices.
 * \param mat4 a left operand.
 * \param mat4 b right operand.
 * \return a result of multiplication between axb.
 */
static inline mat4 mat4Mul(const mat4 a, const mat4 b) {
    mat4 r = mat4Zero;

    r.xx = a.xx * b.xx + a.xy * b.yx + a.xz * b.zx + a.xw * b.wx;
    r.xy = a.xx * b.xy + a.xy * b.yy + a.xz * b.zy + a.xw * b.wy;
    r.xz = a.xx * b.xz + a.xy * b.yz + a.xz * b.zz + a.xw * b.wz;
    r.xw = a.xx * b.xw + a.xy * b.yw + a.xz * b.zw + a.xw * b.ww;

    r.yx = a.yx * b.xx + a.yy * b.yx + a.yz * b.zx + a.yw * b.wx;
    r.yy = a.yx * b.xy + a.yy * b.yy + a.yz * b.zy + a.yw * b.wy;
    r.yz = a.yx * b.xz + a.yy * b.yz + a.yz * b.zz + a.yw * b.wz;
    r.yw = a.yx * b.xw + a.yy * b.yw + a.yz * b.zw + a.yw * b.ww;

    r.zx = a.zx * b.xx + a.zy * b.yx + a.zz * b.zx + a.zw * b.wx;
    r.zy = a.zx * b.xy + a.zy * b.yy + a.zz * b.zy + a.zw * b.wy;
    r.zz = a.zx * b.xz + a.zy * b.yz + a.zz * b.zz + a.zw * b.wz;
    r.zw = a.zx * b.xw + a.zy * b.yw + a.zz * b.zw + a.zw * b.ww;

    r.wx = a.wx * b.xx + a.wy * b.yx + a.wz * b.zx + a.ww * b.wx;
    r.wy = a.wx * b.xy + a.wy * b.yy + a.wz * b.zy + a.ww * b.wy;
    r.wz = a.wx * b.xz + a.wy * b.yz + a.wz * b.zz + a.ww * b.wz;
    r.ww = a.wx * b.xw + a.wy * b.yw + a.wz * b.zw + a.ww * b.ww;

    return r;
}

/**
 * \brief Multiplies a vector with a matrix.
 */
static inline vec4 mat4MulVec4(const mat4 a, const vec4 b) {
    vec4 r;

    r.x = a.xx * b.x + a.xy * b.y + a.xz * b.z + a.xw * b.w;
    r.y = a.yx * b.x + a.yy * b.y + a.yz * b.z + a.yw * b.w;
    r.z = a.zx * b.x + a.zy * b.y + a.zz * b.z + a.zw * b.w;
    r.w = a.wx * b.x + a.wy * b.y + a.wz * b.z + a.ww * b.w;

    return r;
}

/**
 * \brief Makes a new model scale matrix.
 *
 * \param vec3 u the scale for X, Y and Z coordinates.
 * \return scale matrix.
 */
static inline mat4 mat4MakeScale(vec3 u) {
    mat4 r = mat4Identity;
    r.xx = u.x;
    r.yy = u.y;
    r.zz = u.z;
    return r;
}

/**
 * \brief Makes a new model rotation matrix.
 *
 * \param vec3 u the rotation axis to apply angle.
 * \param float angle the angle to rotate.
 * \return rotation matrix.
 */
static inline mat4 mat4MakeRotation(vec3 u, float angle) {
    mat4 r = mat4Identity;
    u = vec3Norm(u);

    float a_cos = cosf(angle);
    float a_sin = sinf(angle);
    float a_icos = (1 - cosf(angle));

    r.xx = a_cos + u.x * u.x * a_icos;
    r.xy = u.x * u.y * a_icos + u.z * a_sin;
    r.xz = u.x * u.z * a_icos + u.y * a_sin;

    r.yx = u.y * u.x * a_icos - u.z * a_sin;
    r.yy = a_cos + u.y * u.y * a_icos;
    r.yz = u.y * u.z * a_icos + u.x * a_sin;

    r.zx = u.z * u.x * a_icos - u.y * a_sin;
    r.zy = u.z * u.y * a_icos - u.x * a_sin;
    r.zz = a_cos + u.z * u.z * a_icos;

    return r;
}

/**
 * \brief Makes a new model rotation matrix only on X axis (euler).
 *
 * \param float angle in radians.
 * \return rotation matrix on X axis.
 */
static inline mat4 mat4MakeRotationX(float angle) {
    mat4 r = mat4Identity;
    float c = cosf(angle);
    float s = sinf(angle);

    r.yy = c;
    r.yz = s;

    r.zy = -s;
    r.zz = c;
    return r;
}

/**
 * \brief Makes a new model rotation matrix only on Y axis (euler).
 *
 * \param float angle in radians.
 * \return rotation matrix on Y axis.
 */
static inline mat4 mat4MakeRotationY(float angle) {
    mat4 r = mat4Identity;
    float c = cosf(angle);
    float s = sinf(angle);

    r.xx = c;
    r.xz = -s;

    r.zx = s;
    r.zz = c;
    return r;
}

/**
 * \brief Makes a new model rotation matrix only on Z axis (euler).
 *
 * \param float angle in radians.
 * \return rotation matrix on Z axis.
 */
static inline mat4 mat4MakeRotationZ(float angle) {
    mat4 r = mat4Identity;
    float c = cosf(angle);
    float s = sinf(angle);

    r.xx = c;
    r.xy = s;

    r.yx = -s;
    r.yy = c;
    return r;
}

/**
 * \brief Makes a new ortho matrix.
 *
 * Generates a new projection matrix usen left, right, bottom, top, near and far parameters.
 *
 * \param float l left
 * \param float r right
 * \param float b bottom
 * \param float t top
 * \param float n near
 * \param float f far
 * \return a othographic projection matrix.
 */
static inline mat4 mat4MakeOrtho(float l, float r, float b, float t, float n, float f){
    mat4 m = mat4Zero;

    m.xx = 2.0f/(r-l);
    m.yy = 2.0f/(t-b);
    m.zz = -2.0f/(f-n);

    m.wx = -(r+l)/(r-l);
    m.wy = -(t+b)/(t-b);
    m.wz = -(f+n)/(f-n);
    m.ww = 1.0f;

    return m;
}

/**
 * \brief Makes a new perhashmaptive matrix.
 *
 * Generates a new projection matrix usen fov, ahashmapt, near and far parameters.
 *
 * \param float yfov Field of view from Y axis
 * \param float ahashmapt ratio
 * \param float n near
 * \param float f far
 * \return a prerhashmaptive projection matrix.
 */
static inline mat4 mat4MakePerhashmaptive(float yfov, float ahashmapt, float n, float f) {
    mat4 m = mat4Zero;

    // Note: use radians intead degrees
    float const a = 1.0f / tan(yfov / 2.0f);

    m.xx = a / ahashmapt;
    m.xy = 0.0f;
    m.xz = 0.0f;
    m.xw = 0.0f;

    m.yx = 0.0f;
    m.yy = a;
    m.yz = 0.0f;
    m.yw = 0.0f;

    m.zx = 0.0f;
    m.zy = 0.0f;
    m.zz = -((f + n) / (f - n));
    m.zw = -1.0f;

    m.wx = 0.0f;
    m.wy = 0.0f;
    m.wz = -((2.0f * f * n) / (f - n));
    m.ww = 0.0f;
    return m;
}

/**
 * \brief Makes a new "looking at" matrix.
 *
 * \param float eye position to look at
 * \param float center of object
 * \param float up
 */
static inline mat4 mat4LookAt(vec3 eye, vec3 center, vec3 up) {
    mat4 m = mat4Identity;
    vec3 f = vec3Norm(vec3Sub(center, eye));
    vec3 u = vec3Norm(up);
    vec3 s = vec3Norm(vec3Cross(f, u));
    u = vec3Cross(s, f);

    m.xx = s.x;
    m.yx = s.y;
    m.zx = s.z;
    m.xy = u.x;
    m.yy = u.y;
    m.zy = u.z;
    m.xz = -f.x;
    m.yz = -f.y;
    m.zz = -f.z;
    m.wx = -vec3Dot(s, eye);
    m.wy = -vec3Dot(u, eye);
    m.wz =  vec3Dot(f, eye);
    return m;
}

#endif /* IKE_MATH4_H */
