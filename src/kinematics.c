#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

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

void eulerZYX(double alpha, double beta, double gamma, mat3 R){
    // Calculates a rotation matrix from given angles of rotation through Z -> Y -> X

    double Ca = cos(alpha);
    double Cb = cos(beta);
    double Cg = cos(gamma);
    double Sa = sin(alpha);
    double Sb = sin(beta);
    double Sg = sin(gamma);

    R[0][0] = Ca*Cb;
    R[0][1] = Ca*Sb*Sg - Sa*Cg;
    R[0][2] = Ca*Sb*Cg + Sa*Sg;

    R[1][0] = Sa*Cb;
    R[1][1] = Sa*Sb*Sg + Ca*Cg;
    R[1][2] = Sa*Sb*Cg - Ca*Sg;

    R[2][0] = -Sb;
    R[2][1] = Cb*Sg;
    R[2][2] = Cb*Cg;
}

void eulerZYZ(double alpha, double beta, double gamma, mat3 R){
    // Calculates a rotation matrix from given angles of rotation through Z -> Y -> Z
    // Useful for wrist joints

    double Ca = cos(alpha);
    double Cb = cos(beta);
    double Cg = cos(gamma);
    double Sa = sin(alpha);
    double Sb = sin(beta);
    double Sg = sin(gamma);

    R[0][0] = Ca*Cb*Cg-Sa*Sg;
    R[0][1] = -Ca*Cb*Sg-Sa*Cg;
    R[0][2] = Ca*Sb;

    R[1][0] = Sa*Cb*Cg+Ca*Sg;
    R[1][1] = -Sa*Cb*Sg+Ca*Cg;
    R[1][2] = Sa*Sb;

    R[2][0] = -Sb*Cg;
    R[2][1] = Sb*Sg;
    R[2][2] = Cb;
}

void eulerZYZinverse(vec2 alpha, vec2 beta, vec2 gamma, mat3 R){
    // Calculate the angles requires to achieve a transform R
    // Useful for wrist joints
    // 

    beta[0] = atan2(sqrt(1-R[2][2]*R[2][2]),R[2][2]);
    double Sb = sin(beta[0]);
    if (fabs(Sb)<0.01){
        printf("singularity in inverse ZYZ, alpha and gamma chosen arbitrarily");
        alpha[0]=atan2(R[1][0],R[0][0]);
        gamma[0]=0;
        alpha[1]=atan2(-R[1][0],R[0][0]);
        gamma[1]=0;
    } else {
        alpha[0] = atan2(R[1][2]/Sb,R[0][2]/Sb);
        gamma[0] = atan2(R[2][1]/Sb,-R[2][0]/Sb);
        alpha[1] = atan2(-R[1][2]/Sb,-R[0][2]/Sb);
        gamma[1] = atan2(-R[2][1]/Sb,R[2][0]/Sb);
    }
}

mat4s dhTransformationMatrix(double alpha, double theta, double a, double d){
    // must follow DH convention:
    // axis X1 perpendiculat to Z0
    // axis X1 intercepts Z0

    mat4s T;

    double Ca = cos(alpha);
    double Ct = cos(theta);
    double Sa = sin(alpha);
    double St = sin(theta);

    T.m[0][0] = Ct;
    T.m[0][1] = -St*Ca;
    T.m[0][2] = St*Sa;
    T.m[0][3] = a*Ct;

    T.m[1][0] = St;
    T.m[1][1] = Ct*Ca;
    T.m[1][2] = -Ct*Sa;
    T.m[1][3] = a*St;

    T.m[2][0] = 0;
    T.m[2][1] = Sa;
    T.m[2][2] = Ca;
    T.m[2][3] = d;

    T.m[3][0] = 0;
    T.m[3][1] = 0;
    T.m[3][2] = 0;
    T.m[3][3] = 1;

    return T;

}

void homogeneousTransform(vec3 p_one,vec3 p_zero,mat4 T){
    // Converts vector P1 in reference frame 1 to p0 in frame 0 using translation and rotation
    // from 0 -> 1
    
    // mat4 Tnew;

    // //build homognenousTransform
    // for (int i=0;i<3;i++){
    //     for (int j=0;j<3;j++){
    //            Tnew[i][j]=rotation[i][j];
    //     }
    //     Tnew[i][3]=origin[i];
    // }
    // Tnew[3][0] = 0;
    // Tnew[3][1] = 0;
    // Tnew[3][2] = 0;
    // Tnew[3][3] = 1;

    //apply transform
    for (int i=0; i<3; i++){
        p_zero[i] = p_one[0]*T[i][0] + p_one[1]*T[i][1] + p_one[2]*T[i][2] + T[i][3];
    }
}

FKresults forwardKinematics6new(vec6 A_i,vec6 alpha_i,vec6 D_i,vec6 theta_i){
    // Calculates the forward kinematics of a 6DOF robot using DH convention to
    // give end effector position in cartesian coordinates and rotation relative to
    // the base frame
    FKresults out;
    FKresults individualTransforms;
    mat4s T;

    for(int i=0;i<6;i++){
        individualTransforms.T[i]=dhTransformationMatrix(alpha_i[i],theta_i[i],A_i[i],D_i[i]);
    }

    out.T[0]=individualTransforms.T[0];
    for(int i=1;i<6;i++){
        int j=i-1;
        out.T[i]=mat4_muls(out.T[i-1],individualTransforms.T[i]);
    }
    
    return out;
} 

// void forwardKinematics2(vec2 A_i,vec2 alpha_i,vec2 D_i,vec2 theta_i,vec3 end_pos,mat3 end_rotation){
//     // Calculates the forward kinematics of a 6DOF robot using DH convention to
//     // give end effector position in cartesian coordinates and rotation relative to
//     // the base frame

//     mat4 T_old;
//     mat4 T;
//     mat4 T_temp;

//     // First link
//     dhTransformationMatrix(alpha_i[0],theta_i[0],A_i[0],D_i[0],T_old);

//     for(int i=1;i<2;i++){
//         dhTransformationMatrix(alpha_i[i],theta_i[i],A_i[i],D_i[i],T);
//         mat4_mul(T_old,T,T_temp);
//         memcpy(T_old,T_temp,sizeof(mat4));
//         }
//     for(int k=0;k<3;k++){
//         for(int j=0;j<4;j++){
//             if(j<3){
//                 end_rotation[k][j] = T_old[k][j];
//             }
//             if(j==3){
//                 end_pos[k]=T_old[k][j];
//             }
//         }
//     }
// } 

void mat2_mul(mat2 A, mat2 B, mat2 out){
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            out[i][j]=0;
            for(int k=0;k<2;k++)
                out[i][j]+=A[i][k]*B[k][j];
        }
    }
}
void mat3_mul(mat3 A, mat3 B, mat3 out){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            out[i][j]=0;
            for(int k=0;k<3;k++)
                out[i][j]+=A[i][k]*B[k][j];
        }
    }
}
void mat4_mul(mat4 A, mat4 B, mat4 out){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            out[i][j]=0;
            for(int k=0;k<4;k++)
                out[i][j]+=A[i][k]*B[k][j];
        }
    }
}
mat4s mat4_muls(mat4s A, mat4s B){
    mat4s out;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            out.m[i][j]=0;
            for(int k=0;k<4;k++)
                out.m[i][j]+=A.m[i][k]*B.m[k][j];
        }
    }
    return out;
}

void mat2_vec_mul(mat2 A, vec2 B, vec2 out){
    for(int i=0;i<2;i++){
        out[i]=0;
        for(int j=0;j<2;j++){
            out[i]+=A[i][j]*B[j];
        }
    }
}
void mat3_vec_mul(mat3 A, vec3 B, vec3 out){
    for(int i=0;i<3;i++){
        out[i]=0;
        for(int j=0;j<3;j++){
            out[i]+=A[i][j]*B[j];
        }
    }
}
void mat4_vec_mul(mat4 A, vec4 B, vec4 out){
    for(int i=0;i<4;i++){
        out[i]=0;
        for(int j=0;j<4;j++){
            out[i]+=A[i][j]*B[j];
        }
    }
}

  