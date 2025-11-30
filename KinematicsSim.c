#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "matrix.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif
// typedef double vec3[3];
// typedef double vec4[4];
// typedef double mat3[3][3];
// typedef double mat4[4][4];

void eulerZYX(double alpha, double beta, double gamma, mat3 R);
void homogeneousTransform(vec3 p_one,vec3 p_zero,vec3 origin,mat3 rotation);
void dhTransformationMatrix(double alpha, double theta, double a, double d, mat4 T);
void forwardKinematics6(vec6 A_i,vec6 alpha_i,vec6 D_i,vec6 theta_i,int n,vec3 end_pos,mat3 end_rotation);

int main()
{
    // Example of Euler Transform
    double  alpha=M_PI/2;
    double  beta=0;
    double  gamma=0;
    mat3 R = {0};

    eulerZYX(alpha,beta,gamma,R);

    printf("Rotational matrix from EulerZYX\n");
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            printf(" %f ",R[i][j]);
        }
        printf("\n");
    }

    // Example of homogeneous Transform
    vec3 origin = 
       {0,
        3,
        1};
    vec3 p_one = 
       {0,
        1,
        1};
    vec3 p_zero=
       {0,
        0,
        0};
    mat3 rotation = {{1,0,0},{0,0,-1},{0,1,0}};
    homogeneousTransform(p_one,p_zero,origin,rotation);

    printf("Homogoenous tranform of p1 to p0:\n");
    for(int i=0;i<3;i++){
        printf(" %f \n",p_zero[i]);
    }

    // Example of 6DOF robot End effector position calculation using DH convention
    vec6 A_i ={0,0.2,0,0,0,0};
    vec6 alpha_i ={M_PI/2,0,M_PI/2,-M_PI/2,M_PI/2,0};
    vec6 D_i ={0.3,0,0,0.2,0,0.1};
    vec6 theta_i ={0,0,0,0,0,0};
    int n = 6;
    mat3 end_rotation = {0};
    vec3 end_pos = {0};
    forwardKinematics6(A_i,alpha_i,D_i,theta_i,n,end_pos,end_rotation);

    printf("print end position relative to origin: \n");
    for(int i=0;i<3;i++){
        printf(" %f \n",end_pos[i]);
    }
    printf("print end rotation relative to origin: \n");
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            printf(" %f ",end_rotation[i][j]);
        }
        printf("\n");
    }

    return 0; 
}

void eulerZYX(double alpha, double beta, double gamma, mat3 R){
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

void dhTransformationMatrix(double alpha, double theta, double a, double d, mat4 T){
    // must follow DH convention:
    // axis X1 perpendiculat to Z0
    // axis X1 intercepts Z0

    double Ca = cos(alpha);
    double Ct = cos(theta);
    double Sa = sin(alpha);
    double St = sin(theta);

    T[0][0] = Ct;
    T[0][1] = -St*Ca;
    T[0][2] = St*Sa;
    T[0][3] = a*Ct;

    T[1][0] = St;
    T[1][1] = Ct*Ca;
    T[1][2] = -Ct*Sa;
    T[1][3] = a*St;

    T[2][0] = 0;
    T[2][1] = Sa;
    T[2][2] = Ca;
    T[2][3] = d;

    T[3][0] = 0;
    T[3][1] = 0;
    T[3][2] = 0;
    T[3][3] = 1;

}

void homogeneousTransform(vec3 p_one,vec3 p_zero,vec3 origin,mat3 rotation){
    mat4 T={0};
    //build homognenousTransform
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
               T[i][j]=rotation[i][j];
        }
        T[i][3]=origin[i];
    }
    T[3][0] = 0;
    T[3][1] = 0;
    T[3][2] = 0;
    T[3][3] = 1;

    //apply transform
    for (int i=0; i<3; i++){
        p_zero[i] = p_one[0]*T[i][0] + p_one[1]*T[i][1] + p_one[2]*T[i][2] + T[i][3];
    }
}

void forwardKinematics6(vec6 A_i,vec6 alpha_i,vec6 D_i,vec6 theta_i,int n,vec3 end_pos,mat3 end_rotation){

    mat4 T_old;
    mat4 T;
    mat4 T_temp;

    // First link
    dhTransformationMatrix(alpha_i[0],theta_i[0],A_i[0],D_i[0],T_old);

    for(int i=1;i<n;i++){
        dhTransformationMatrix(alpha_i[i],theta_i[i],A_i[i],D_i[i],T);
        mat4_mul(T_old,T,T_temp);
        memcpy(T_old,T_temp,sizeof(mat4));
        }
    
    for(int k=0;k<3;k++){
        for(int j=0;j<4;j++){
            if(j<3){
                end_rotation[k][j] = T_old[k][j];
            }
            if(j==3){
                end_pos[k]=T_old[k][j];
            }
        }
    }
}

  