#ifndef KINEMATICS_H
#define KINEMATICS_H

typedef double vec2[2];
typedef double vec3[3];
typedef double vec4[4];
typedef double vec5[5];
typedef double vec6[6];
typedef double mat2[2][2];
typedef double mat3[3][3];
typedef double mat4[4][4];

typedef struct {
    double m[4][4];
}mat4s;

typedef struct{
    mat4s T[6];
}FKresults;

void mat2_mul(mat2 A, mat2 B, mat2 out);
void mat3_mul(mat3 A, mat3 B, mat3 out);
void mat4_mul(mat4 A, mat4 B, mat4 out);
mat4s mat4_muls(mat4s A, mat4s B);

void mat2_vec_mul(mat2 A, vec2 B, vec2 out);
void mat3_vec_mul(mat3 A, vec3 B, vec3 out);
void mat4_vec_mul(mat4 A, vec4 B, vec4 out);

void eulerZYX(double alpha, double beta, double gamma, mat3 R);
void eulerZYZ(double alpha, double beta, double gamma, mat3 R);
void eulerZYZinverse(vec2 alpha, vec2 beta, vec2 gamma, mat3 R);
void homogeneousTransform(vec3 p_one,vec3 p_zero,mat4 T);
mat4s dhTransformationMatrix(double alpha, double theta, double a, double d);
FKresults forwardKinematics6new(vec6 A_i,vec6 alpha_i,vec6 D_i,vec6 theta_i);
// void forwardKinematics2(vec2 A_i,vec2 alpha_i,vec2 D_i,vec2 theta_i,vec3 end_pos,mat3 end_rotation);

#endif