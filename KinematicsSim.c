#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

#define R1 3 // number of rows in Matrix-1
#define R2 3 
#define C1 3 
#define C2 1 

void multiplyMatrix(double rotation_matrix[][C1], double vector[][C2])
{
    int result[R1][C2];
    printf("resultant matrix is:\n");
    for(int i=0;i<R1;i++){
        for(int j=0;j<C2;j++){
            result[i][j]=0;
            for(int k=0;k<R2;k++){
                result[i][j] += rotation_matrix[i][k]*vector[k][j];
            }
            printf("%d\t",result[i][j]);
        }
        printf("\n");
    }
}

void eulerZ(double vector[][C2], double alpha)
{
    double c_alpha = cos(alpha);
    double s_alpha = sin(alpha);
    double rotation_z[R1][C1] = {{c_alpha,-s_alpha,0},{s_alpha,c_alpha,0},{0,0,1}};

    multiplyMatrix(rotation_z, vector);

}

int main()
{
    double  angle=M_PI;
    double  s_alpha=sin(angle);
    double  c_alpha=cos(angle);
    double* alpha = &angle;

    double rotation_matrix[R1][C1] = {{c_alpha,-s_alpha,0},{s_alpha,c_alpha,0},{0,0,1}};
    double vector[R2][C2] = {{2},{2},{1}};

    // if coloumn of rotation_matrix not equal to rows of vector
    if (C1 != R2) {
        printf("The number of columns in Matrix-1 must be "
               "equal to the number of rows in "
               "Matrix-2\n");
        printf("Please update MACROs value according to "
               "your array dimension in "
               "#define section\n");

        exit(EXIT_FAILURE);
    }

    // Function call
    eulerZ(vector,*alpha);
    eulerY(vector,*alpha);
    eulerX(vector,*alpha);

    return 0; 
}