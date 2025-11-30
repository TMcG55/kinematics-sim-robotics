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

void homogeneousTransform(vec3 p_one,vec3 p_zero,vec3 origin,mat3 rotation){
    mat4 T={0};
    //build homognenousTransform
    for (int i=0;i<3;i++){
        for (int j=0;j<4;j++){
               T[i][j]=rotation[i][j];
       T[i][3]=origin[i];
        }
    }
    T[3][3]=1;

    //apply transform
    for (int i=0; i<3; i++){
        p_zero[i] = p_one[0]*T[i][0] + p_one[1]*T[i][1] + p_one[2]*T[i][2] + T[i][3];
    }
}

  