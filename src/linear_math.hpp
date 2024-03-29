#ifndef linear_math_hpp
#define linear_math_hpp

#include <math.h>

#define VEC(n)\
static inline vec##n operator*(float s, vec##n v) {\
  vec##n res;\
  for (int i=0; i<n; ++i) {\
    res.A[i] = s * v.A[i];\
  }\
  return res;\
}\
static inline float operator*(vec##n u, vec##n v) {\
  float res=0.f;\
  for (int i=0; i<n; ++i) {\
    res += u.A[i] * v.A[i];\
  }\
  return res;\
}\
static inline vec##n operator+(vec##n u, vec##n v) {\
  vec##n res;\
  for (int i=0; i<n; ++i) {\
    res.A[i] = u.A[i] + v.A[i];\
  }\
  return res;\
}\
static inline vec##n operator-(vec##n u, vec##n v) {\
  vec##n res;\
  for (int i=0; i<n; ++i) {\
    res.A[i] = u.A[i] - v.A[i];\
  }\
  return res;\
}\
static inline vec##n operator-(vec##n v) {\
  vec##n res;\
  for (int i=0; i<n; ++i) {\
    res.A[i] = -v.A[i];\
  }\
  return res;\
}\
static inline float mag(vec##n v) {\
  float sum = 0;\
  for (int i=0; i<n; ++i) {\
    sum += v.A[i]*v.A[i];\
  }\
  return sqrtf(sum);\
}\
static inline vec##n normalize(vec##n v) {\
  vec##n res;\
  const float magnitude = mag(v);\
  for (int i=0; i<n; ++i) {\
    res.A[i] = v.A[i]/magnitude;\
  }\
  return res;\
}

union vec2 {
  float A[2];
  struct {
    float x,y;
  };
};

union vec3 {
  float A[3];
  struct {
    float x,y,z;
  };
  struct {
    float r,g,b;
  };
};

union vec4 {
  float A[4];
  struct {
    float x,y,z,w;
  };
  struct {
    float r,g,b,a;
  };
};

static inline vec2 Vec2(float x, float y) {
  vec2 res;
  res.x = x;
  res.y = y;
  return res;
}

static inline vec3 Vec3(float x, float y, float z) {
  vec3 res;
  res.x = x;
  res.y = y;
  res.z = z;
  return res;
}

static inline vec4 Vec4(float x, float y, float z, float w) {
  vec4 res;
  res.x = x;
  res.y = y;
  res.z = z;
  res.w = w;
  return res;
}

VEC(2)
VEC(3)
VEC(4)

static inline vec3 cross(vec3 u, vec3 v) {
  return Vec3(u.y * v.z - u.z * v.y,
              u.z * v.x - u.x * v.z,
              u.x * v.y - u.y * v.x);
}


union quat {
  float A[4];
  struct {
    float r;
    float i;
    float j;
    float k;
  };
  struct {
    float w;
    vec3 v;
  };
};

static inline quat Quat(float r, float i, float j, float k) {
  quat q;
  q.r = r;
  q.i = i;
  q.j = j;
  q.k = k;
  return q;
}

static inline quat Quat(float angle, vec3 axis) {
  float s = sinf(angle/2);
  return Quat(cosf(angle/2), axis.x*s, axis.y*s, axis.z*s);
}

static inline quat operator+(quat q1, quat q2) {
  return Quat(q1.r+q1.r, q1.v+q2.v);
}

static inline quat operator*(quat q1, quat q2) {
  return Quat(q1.r*q2.r - q1.v*q2.v, cross(q1.v,q2.v) + q1.r*q2.v + q2.r*q1.v);
}

static inline quat operator*=(quat q1, quat q2) {
  return q1 * q2;
}

static inline quat normalize(quat q) {
  const float mag = sqrtf(q.r*q.r + q.i*q.i + q.j*q.j + q.k*q.k);
  return Quat(q.r/mag, q.i/mag, q.j/mag, q.k/mag);
}

union mat4 {
  float A[16];
  float CR[4][4];
};

static inline mat4 Mat4(float aa, float ba, float ca, float da,
                        float ab, float bb, float cb, float db,
                        float ac, float bc, float cc, float dc,
                        float ad, float bd, float cd, float dd) {
  mat4 res;
  res.CR[0][0]=aa, res.CR[1][0]=ba, res.CR[2][0]=ca, res.CR[3][0]=da;
  res.CR[0][1]=ab, res.CR[1][1]=bb, res.CR[2][1]=cb, res.CR[3][1]=db;
  res.CR[0][2]=ac, res.CR[1][2]=bc, res.CR[2][2]=cc, res.CR[3][2]=dc;
  res.CR[0][3]=ad, res.CR[1][3]=bd, res.CR[2][3]=cd, res.CR[3][3]=dd;
  return res;
}

static inline mat4 operator*(mat4 a, mat4 b) {
  mat4 res = Mat4(
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f);
  for (int y=0; y<4; ++y) {
    for (int x=0; x<4; ++x) {
      for (int n=0; n<4; ++n) {
        res.CR[x][y] += a.CR[x][n] * b.CR[n][y];
      }
    }
  }
  return res;
}

static inline mat4 indentity() {
  return Mat4(
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f);
}

static inline mat4 translation(float x, float y, float z) {
  return Mat4(
    1.f, 0.f, 0.f, x,
    0.f, 1.f, 0.f, y,
    0.f, 0.f, 1.f, z,
    0.f, 0.f, 0.f, 1.f);
}

static inline mat4 translation(vec3 v) {
  return translation(v.x, v.y, v.z);
}

static inline mat4 translate(mat4 m, float x, float y, float z) {
  return translation(x,y,z) * m;
}

static inline mat4 translate(mat4 m, vec3 v) {
  return translation(v.x,v.y,v.z) * m;
}

static inline mat4 rotation(float x, float y, float z, float angle) {
  const float c = cosf(angle);
  const float s = sinf(angle);
  return Mat4(
    x*x+(1-x*x)*c, x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0,
    x*y*(1-c)+z*s, y*y+(1-y*y)*c, y*z*(1-c)-x*s, 0,
    x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z+(1-z*z)*c, 0,
    0,             0,             0,             1);
}

static inline mat4 rotation(quat q) {
  const float n = q.r*q.r + q.i*q.i + q.j*q.j + q.k*q.k;
  const float s = (n == 0) ? 0 : 2.f/n;
  const float ri = s*q.r*q.i, rj = s*q.r*q.j, rk = s*q.r*q.k,
              ii = s*q.i*q.i, ij = s*q.i*q.j, ik = s*q.i*q.k,
              jj = s*q.j*q.j, jk = s*q.j*q.k, kk = s*q.k*q.k;
  return Mat4(
    1-jj-kk, ij-rk,   ik+rj,   0,
    ij+rk,   1-ii-kk, jk-ri,   0,
    ik-rj,   jk+ri,   1-ii-jj, 0,
    0,       0,       0,       1);
}

static inline mat4 rotate(mat4 m, float x, float y, float z, float angle) {
  return rotation(x,y,z, angle)*m;
}

static inline mat4 rotate(mat4 m, quat q) {
  return rotation(q)*m;
}

static inline mat4 scaling(float x, float y, float z) {
  return Mat4(
    x,   0.f, 0.f, 0.f,
    0.f, y,   0.f, 0.f,
    0.f, 0.f, z,   0.f,
    0.f, 0.f, 0.f, 1.f);
}

static inline mat4 scale(mat4 m, float x, float y, float z) {
  const mat4 s = scaling(x,y,z);
  return s*m;
}

static inline mat4 scale(mat4 m, vec3 s) {
  return scale(m, s.x, s.y, s.z);
}

static inline mat4 scale(mat4 m, float f) {
  return scale(m, f,f,f);
}

static inline mat4 transpose(mat4 m) {
  mat4 res;
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      res.CR[i][j] = m.CR[j][i];
    }
  }
  return res;
}

static inline mat4 inverse(mat4 m) {
  const mat4 t = transpose(m);
  const float pairs[24] = {
    t.A[10]*t.A[15],
    t.A[11]*t.A[14],
    t.A[9]*t.A[15],
    t.A[11]*t.A[13],
    t.A[9]*t.A[14],
    t.A[10]*t.A[13],
    t.A[8]*t.A[15],
    t.A[11]*t.A[12],
    t.A[8]*t.A[14],
    t.A[10]*t.A[12],
    t.A[8]*t.A[13],
    t.A[9]*t.A[12],
    t.A[2]*t.A[7],
    t.A[3]*t.A[6],
    t.A[1]*t.A[7],
    t.A[3]*t.A[5],
    t.A[1]*t.A[6],
    t.A[2]*t.A[5],
    t.A[0]*t.A[7],
    t.A[3]*t.A[4],
    t.A[0]*t.A[6],
    t.A[2]*t.A[4],
    t.A[0]*t.A[5],
    t.A[1]*t.A[4]};
  const float cofactors[16] = {
    pairs[0]*t.A[5] + pairs[3]*t.A[6] + pairs[4]*t.A[7] -
    pairs[1]*t.A[5] - pairs[2]*t.A[6] - pairs[5]*t.A[7],
    pairs[1]*t.A[4] + pairs[6]*t.A[6] + pairs[9]*t.A[7] -
    pairs[0]*t.A[4] - pairs[7]*t.A[6] - pairs[8]*t.A[7],
    pairs[2]*t.A[4] + pairs[7]*t.A[5] + pairs[10]*t.A[7] -
    pairs[3]*t.A[4] - pairs[6]*t.A[5] - pairs[11]*t.A[7],
    pairs[5]*t.A[4] + pairs[8]*t.A[5] + pairs[11]*t.A[6] -
    pairs[4]*t.A[4] - pairs[9]*t.A[5] - pairs[10]*t.A[6],
    pairs[1]*t.A[1] + pairs[2]*t.A[2] + pairs[5]*t.A[3] -
    pairs[0]*t.A[1] - pairs[3]*t.A[2] - pairs[4]*t.A[3],
    pairs[0]*t.A[0] + pairs[7]*t.A[2] + pairs[8]*t.A[3] -
    pairs[1]*t.A[0] - pairs[6]*t.A[2] - pairs[9]*t.A[3],
    pairs[3]*t.A[0] + pairs[6]*t.A[1] + pairs[11]*t.A[3] -
    pairs[2]*t.A[0] - pairs[7]*t.A[1] - pairs[10]*t.A[3],
    pairs[4]*t.A[0] + pairs[9]*t.A[1] + pairs[10]*t.A[2] -
    pairs[5]*t.A[0] - pairs[8]*t.A[1] - pairs[11]*t.A[2],
    pairs[12]*t.A[13] + pairs[15]*t.A[14] + pairs[16]*t.A[15] -
    pairs[13]*t.A[13] - pairs[14]*t.A[14] - pairs[17]*t.A[15],
    pairs[13]*t.A[12] + pairs[18]*t.A[14] + pairs[21]*t.A[15] -
    pairs[12]*t.A[12] - pairs[19]*t.A[14] - pairs[20]*t.A[15],
    pairs[14]*t.A[12] + pairs[19]*t.A[13] + pairs[22]*t.A[15] -
    pairs[15]*t.A[12] - pairs[18]*t.A[13] - pairs[23]*t.A[15],
    pairs[17]*t.A[12] + pairs[20]*t.A[13] + pairs[23]*t.A[14] -
    pairs[16]*t.A[12] - pairs[21]*t.A[13] - pairs[22]*t.A[14],
    pairs[14]*t.A[10] + pairs[17]*t.A[11] + pairs[13]*t.A[9] -
    pairs[16]*t.A[11] - pairs[12]*t.A[9] - pairs[15]*t.A[10],
    pairs[20]*t.A[11] + pairs[12]*t.A[8] + pairs[19]*t.A[10] -
    pairs[18]*t.A[10] - pairs[21]*t.A[11] - pairs[13]*t.A[8],
    pairs[18]*t.A[9] + pairs[23]*t.A[11] + pairs[15]*t.A[8] -
    pairs[22]*t.A[11] - pairs[14]*t.A[8] - pairs[19]*t.A[9],
    pairs[22]*t.A[10] + pairs[16]*t.A[8] + pairs[21]*t.A[9] -
    pairs[20]*t.A[9] - pairs[23]*t.A[10] - pairs[17]*t.A[8]};
  const float det = 1.f/(t.A[0]*cofactors[0] + t.A[1]*cofactors[1] + t.A[2]*cofactors[2] + t.A[3]*cofactors[3]);

  mat4 res;
  for (int i=0; i<16; ++i) {
    res.A[i] = cofactors[i]*det;
  }
  return res;
}

static inline mat4 lookAt(vec3 eye, vec3 at, vec3 up) {
  const vec3 forward = normalize(at-eye);
  const vec3 right = normalize(cross(forward,up));
  const vec3 top = cross(right,forward);

  return Mat4(
    right.x,    right.y,    right.z,   0.f,
    top.x,      top.y,      top.z,     0.f,
   -forward.x, -forward.y, -forward.z, 0.f,
    0.f,        0.f,        0.f,       1.f);
}

static inline mat4 perspective(float fov, float aspect, float n, float f) {
  float const a = 1.f / tanf(fov/2.f);
  return Mat4(
    a/aspect, 0.f,  0.f,        0.f,
    0.f,      a,    0.f,        0.f,
    0.f,      0.f, (f+n)/(n-f), (2*f*n)/(n-f),
    0.f,      0.f, -1.f,        0.f);
}

#endif
