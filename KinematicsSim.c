#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif
typedef double vec3[3];
typedef double vec4[4];
typedef double mat3[3][3];
typedef double mat4[4][4];

void multiplyMatrix(mat3 rotation_matrix, vec3 p_one,vec3 p_zero);
void eulerZ(vec3 p_one, double alpha, vec3 p_zero);
void eulerY(vec3 p_one, double beta, vec3 p_zero);
void eulerX(vec3 p_one, double gamma, vec3 p_zero);
void homogeneousTransform(vec3 p_one,vec3 p_zero,vec3 origin,mat3 rotation);


int main()
{

    // double  alpha=M_PI/3;
    // double  beta=M_PI/15;
    // double  gamma=M_PI/5;
    // vec3 p_one = 
    //    {2,
    //     2,
    //     1};
    // vec3 p_zero=
    //    {0,
    //     0,
    //     0};
    

    // //  Function call
    // printf("----------------------------------\n");
    // printf("Z rotate\n");
    // printf("----------------------------------\n");
    // eulerZ(p_one,alpha,p_zero);

    // printf("----------------------------------\n");
    // printf("Y rotate\n");
    // printf("----------------------------------\n");
    // for (int i=0;i<3;i++)
    //     p_one[i]=p_zero[i];
    // eulerY(p_one,beta,p_zero);

    // printf("----------------------------------\n");
    // printf("Z rotate\n");
    // printf("----------------------------------\n");
    // for (int i=0;i<3;i++)
    //     p_one[i]=p_zero[i];
    // eulerZ(p_one,gamma,p_zero);
    
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
    mat4 t;

    homogeneousTransform(p_one,p_zero,origin,rotation);

    printf("point referenced from the origin:\n");
    for(int i=0;i<3;i++){
        printf(" %f \n",p_zero[i]);
    }
    return 0; 
}


void multiplyMatrix(mat3 rotation_matrix, vec3 p_one,vec3 p_zero)
{
    printf("p_one\n");
    for (int i=0;i<3;i++)
        printf(" %f \n",p_one[i]);

    printf("rotation_matrix \n");
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            printf(" %f ",rotation_matrix[i][j]);
        }
        printf("\n");
    }
    printf("p_zero\n");    
    for (int i=0;i<3;i++){
        p_zero[i]=0;
        for (int j=0;j<3;j++){
            p_zero[i]+=p_one[j]*rotation_matrix[i][j];
        }
        printf("%f\n",p_zero[i]);
    }

} 

void eulerZ(vec3 p_one, double alpha, vec3 p_zero)
{

    double c_alpha = cos(alpha);
    double s_alpha = sin(alpha);
    double rotation_z[3][3] = {{c_alpha,-s_alpha,0},{s_alpha,c_alpha,0},{0,0,1}};

    multiplyMatrix(rotation_z, p_one,p_zero);
}
void eulerY(vec3 p_one, double beta, vec3 p_zero)
{
    double c_beta = cos(beta);
    double s_beta = sin(beta);
    double rotation_y[3][3] = {{c_beta,0,s_beta},{0,1,0},{-s_beta,0,c_beta}};

    multiplyMatrix(rotation_y, p_one,p_zero);
}
void eulerX(vec3 p_one, double gamma, vec3 p_zero)
{
    double c_gamma = cos(gamma);
    double s_gamma = sin(gamma);
    double rotation_z[3][3] = {{c_gamma,-s_gamma,0},{s_gamma,c_gamma,0},{0,0,1}};

    multiplyMatrix(rotation_z, p_one,p_zero);
}

void homogeneousTransform(vec3 p_one,vec3 p_zero,vec3 origin,mat3 rotation){
    vec4 p1;
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

  