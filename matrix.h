
#define MATRIX_H

typedef double vec2[2];
typedef double vec3[3];
typedef double vec4[4];
typedef double vec5[5];
typedef double vec6[6];
typedef double mat2[2][2];
typedef double mat3[3][3];
typedef double mat4[4][4];

void mat2_mul(mat2 A, mat2 B, mat2 out);
void mat3_mul(mat3 A, mat3 B, mat3 out);
void mat4_mul(mat4 A, mat4 B, mat4 out);

void mat2_vec_mul(mat2 A, vec2 B, vec2 out);
void mat3_vec_mul(mat3 A, vec3 B, vec3 out);
void mat4_vec_mul(mat4 A, vec4 B, vec4 out);
