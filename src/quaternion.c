#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Quaternion.h"

struct Quaternion Quaternion_new(float x, float y, float z, float w)
{
  struct Quaternion quaternion = {x, y, z, w};
  return quaternion;
}

struct Quaternion Quaternion_conj(struct Quaternion quaternion_1)
{
  struct Quaternion quaternion = Quaternion_new(-quaternion_1.x, -quaternion_1.y, -quaternion_1.z, quaternion_1.w);
  return quaternion;
}

float Quaternion_mod(struct Quaternion quaternion_1)
{
  float x_pow = pow(quaternion_1.x, 2);
  float y_pow = pow(quaternion_1.y, 2);
  float z_pow = pow(quaternion_1.z, 2);
  float w_pow = pow(quaternion_1.w, 2);

  return sqrt(x_pow + y_pow + z_pow + w_pow);
}

void Quaternion_toRotationMatrix(struct Quaternion quaternion, float rotationmatrix[3][3])
{
  float x = quaternion.x;
  float y = quaternion.y;
  float z = quaternion.z;
  float w = quaternion.w;

  float x_pow = pow(x, 2);
  float y_pow = pow(y, 2);
  float z_pow = pow(z, 2);
  float w_pow = pow(w, 2);
  
  rotationmatrix[0][0] = 2*(w_pow + x_pow) - 1;
  rotationmatrix[0][1] = 2*(x*y - w*z);
  rotationmatrix[0][2] = 2*(x*z + w*y);
  rotationmatrix[1][0] = 2*(x*y + w*z);
  rotationmatrix[1][1] = 2*(w_pow + y_pow) - 1;
  rotationmatrix[1][2] = 2*(y*z - w*x);
  rotationmatrix[2][0] = 2*(x*z - w*y);
  rotationmatrix[2][1] = 2*(y*z + w*x);
  rotationmatrix[2][2] = 2*(w_pow + z_pow) - 1;
}

struct Quaternion Quaternion_addQuaternion(struct Quaternion quaternion_1, struct Quaternion quaternion_2)
{
  struct Quaternion quaternion = Quaternion_new(quaternion_1.x + quaternion_2.x, quaternion_1.y + quaternion_2.y, quaternion_1.z + quaternion_2.z, quaternion_1.w + quaternion_2.w);
  return quaternion;
}

struct Quaternion Quaternion_subQuaternion(struct Quaternion quaternion_1, struct Quaternion quaternion_2)
{
  struct Quaternion quaternion = Quaternion_new(quaternion_1.x - quaternion_2.x, quaternion_1.y - quaternion_2.y, quaternion_1.z - quaternion_2.z, quaternion_1.w - quaternion_2.w);
  return quaternion;
}

struct Quaternion Quaternion_mulScalar(struct Quaternion quaternion_1, float scalar)
{
  struct Quaternion quaternion = Quaternion_new(quaternion_1.x * scalar, quaternion_1.y * scalar, quaternion_1.z * scalar, quaternion_1.w * scalar);
  return quaternion;
}

struct Quaternion Quaternion_divScalar(struct Quaternion quaternion_1, float scalar)
{
  struct Quaternion quaternion = Quaternion_new(quaternion_1.x / scalar, quaternion_1.y / scalar, quaternion_1.z / scalar, quaternion_1.w / scalar);
  return quaternion;
}

struct Quaternion Quaternion_mulQuaternion(struct Quaternion quaternion_1, struct Quaternion quaternion_2)
{
  float x = quaternion_1.x;
  float y = quaternion_1.y;
  float z = quaternion_1.z;
  float w = quaternion_1.w;

  float a = quaternion_2.x;
  float b = quaternion_2.y;
  float c = quaternion_2.z;
  float d = quaternion_2.w;

  struct Quaternion quaternion = Quaternion_new(x*d + y*c - z*b + w*a, -x*c + y*d + z*a + w*b, x*b - y*a + z*d + w*c, -x*a - y*b - z*c + w*d);
  return quaternion;
}

struct Quaternion Quaternion_exp(struct Quaternion quaternion_1)
{
  struct Quaternion quaternion = Quaternion_mulQuaternion(quaternion_1, quaternion_1);
  return quaternion;
}

