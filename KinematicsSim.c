#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>>

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#define R1 3  //number of rows in Matrix-1
#define R2 3 
#define C1 3 
#define C2 1 

void multiplyMatrix(double rotation_matrix[][3], double p_zero[3],double p_transformed[3])
{
    printf("p_zero\n");
    for (int i=0;i<3;i++)
        printf(" %f \n",p_zero[i]);

    printf("rotation_matrix \n");
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            printf(" %f ",rotation_matrix[i][j]);
        }
        printf("\n");
    }
    printf("p_transformed\n");    
    for (int i=0;i<3;i++){
        p_transformed[i]=0;
        for (int j=0;j<3;j++){
            p_transformed[i]+=p_zero[j]*rotation_matrix[i][j];
        }
        printf("%f\n",p_transformed[i]);
    }

} 


void eulerZ(double p_zero[3], double alpha, double p_transformed[3])
{

    double c_alpha = cos(alpha);
    double s_alpha = sin(alpha);
    double rotation_z[R1][C1] = {{c_alpha,-s_alpha,0},{s_alpha,c_alpha,0},{0,0,1}};

    multiplyMatrix(rotation_z, p_zero,p_transformed);
}
void eulerY(double p_zero[3], double beta, double p_transformed[3])
{
    double c_alpha = cos(beta);
    double s_alpha = sin(beta);
    double rotation_y[R1][C1] = {{c_alpha,-s_alpha,0},{s_alpha,c_alpha,0},{0,0,1}};

    multiplyMatrix(rotation_y, p_zero,p_transformed);
}
void eulerX(double p_zero[3], double gamma, double p_transformed[3])
{
    double c_alpha = cos(gamma);
    double s_alpha = sin(gamma);
    double rotation_z[R1][C1] = {{c_alpha,-s_alpha,0},{s_alpha,c_alpha,0},{0,0,1}};

    multiplyMatrix(rotation_z, p_zero,p_transformed);
}

int main()
{

    double  alpha=M_PI/2;
    double  beta=M_PI/2;
    double  gamma=M_PI/2;
    double  s_alpha=sin(alpha);
    double  c_alpha=cos(alpha);

    double p_zero[3] = {2,2,1};

    double p_transformed[3]={0,0,0};

    //  Function call
    printf("----------------------------------\n");
    printf("Z rotate\n");
    printf("----------------------------------\n");
    eulerZ(p_zero,alpha,p_transformed);

    printf("----------------------------------\n");
    printf("Y rotate\n");
    printf("----------------------------------\n");
    memcpy(p_zero, p_transformed, sizeof(*p_transformed));
    eulerY(p_zero,beta,p_transformed);


    for (int i=0;i<3;i++)
        p_zero[i]=p_transformed[i];
    printf("----------------------------------\n");
    printf("Z rotate\n");
    printf("----------------------------------\n");
    eulerZ(p_zero,gamma,p_transformed);
    

    return 0; 
}