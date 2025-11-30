#include "matrix.h"

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