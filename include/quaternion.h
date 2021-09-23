struct Quaternion
{
  float x;
  float y;
  float z;
  float w;
};

struct Quaternion Quaternion_new(float x, float y, float z, float w);
struct Quaternion Quaternion_conj(struct Quaternion quaternion_1);
float Quaternion_mod(struct Quaternion quaternion_1);
void Quaternion_toRotationMatrix(struct Quaternion quaternion, float rotationmatrix[3][3]);
struct Quaternion Quaternion_addQuaternion(struct Quaternion quaternion_1, struct Quaternion quaternion_2);
struct Quaternion Quaternion_subQuaternion(struct Quaternion quaternion_1, struct Quaternion quaternion_2);
struct Quaternion Quaternion_mulScalar(struct Quaternion quaternion_1, float scalar);
struct Quaternion Quaternion_divScalar(struct Quaternion quaternion_1, float scalar);
struct Quaternion Quaternion_mulQuaternion(struct Quaternion quaternion_1, struct Quaternion quaternion_2);
struct Quaternion Quaternion_exp(struct Quaternion quaternion_1);