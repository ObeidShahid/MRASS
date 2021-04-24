#include <math.h>
#include "Rotation.h"
#include <stdio.h>
typedef struct {
    double m[3][3];
} matrix;


static matrix init_ref_frame;

matrix get_ref_frame(double P1[3], double P2[3], double P3[3]) {
    double Origin[3],Mid[3];
    double V1[3],a[3],b[3],c[3];
    double V2[3];
    double V3[3], V4[3];
    int i;
    
    // Calculating the inital mid point of the reference frame //
    Origin[0] = (P1[0] + P2[0] +P3[0])/3; //X Position for the Midpoint
    Origin[1] = (P1[1] + P2[1] +P3[1])/3; //Y Position for the Midpoint
    Origin[2] = (P1[2] + P2[2] +P3[2])/3; //Z Position for the Midpoint
    
    Mid[0] = (P2[0] + P3[0])/2;
    Mid[1] = (P2[1] + P3[1])/2;
    Mid[2] = (P2[2] + P3[2])/2;

    // Determining the Vectors of the Inital  Reference Frame //
    
    V1[0] = Origin[0] - P1[0];
    V1[1] = Origin[1] - P1[1];
    V1[2] = Origin[2] - P1[2];

    V2[0] = Origin[0] - P2[0];
    V2[1] = Origin[1] - P2[1];
    V2[2] = Origin[2] - P2[2];

    V3[0] = Origin[0] - P3[0];
    V3[1] = Origin[1] - P3[1];
    V3[2] = Origin[2] - P3[2];

    V4[0] = Origin[0] - Mid[0];
    V4[1] = Origin[1] - Mid[1];
    V4[2] = Origin[2] - Mid[2];

    a[0] = V1[0];
    a[1] = V1[1];
    a[2] = V1[2];

    b[0] = V2[0] - V4[0];
    b[1] = V2[1] - V4[1];
    b[2] = V2[2] - V4[2];

    c[0] = (a[1]*b[2])-(a[2]*b[1]);
    c[1] = (a[2]*b[0])-(a[0]*b[2]);
    c[2] = (a[0]*b[1])-(a[1]*b[0]);

    matrix ref_frame;

    ref_frame.m[0][0] = a[0];
    ref_frame.m[1][0] = a[1];
    ref_frame.m[2][0] = a[2];
    ref_frame.m[0][1] = b[0];
    ref_frame.m[1][1] = b[1];
    ref_frame.m[2][1] = b[2];
    ref_frame.m[0][2] = c[0];
    ref_frame.m[1][2] = c[1];
    ref_frame.m[2][2] = c[2];
    
    return ref_frame;
}

double *get_mat(){
    return (double*)init_ref_frame.m;
}

matrix invert_matrix(matrix a) {
    double det = a.m[0][0]*(a.m[1][1]*a.m[2][2] - a.m[1][2]*a.m[2][1])
    - a.m[0][1]*(a.m[1][0]*a.m[2][2] - a.m[1][2]*a.m[2][0])
    + a.m[0][2]*(a.m[1][0]*a.m[2][1] - a.m[1][1]*a.m[2][0]);
    
    matrix a_inv;
    
    a_inv.m[0][0] = (a.m[1][1]*a.m[2][2] - a.m[1][2]*a.m[2][1])/det;
    a_inv.m[0][1] = -(a.m[0][1]*a.m[2][2] - a.m[0][2]*a.m[2][1])/det;
    a_inv.m[0][2] = (a.m[0][1]*a.m[1][2] - a.m[0][2]*a.m[1][1])/det;
    a_inv.m[1][0] = -(a.m[1][0]*a.m[2][2] - a.m[1][2]*a.m[2][0])/det;
    a_inv.m[1][1] = (a.m[0][0]*a.m[2][2] - a.m[0][2]*a.m[2][0])/det;
    a_inv.m[1][2] = -(a.m[0][0]*a.m[1][2] - a.m[0][2]*a.m[1][0])/det;
    a_inv.m[2][0] = (a.m[1][0]*a.m[2][1] - a.m[1][1]*a.m[2][0])/det;
    a_inv.m[2][1] = -(a.m[0][0]*a.m[2][1] - a.m[0][1]*a.m[2][0])/det;
    a_inv.m[2][2] = (a.m[0][0]*a.m[1][1] - a.m[0][1]*a.m[1][0])/det;

    return a_inv;
}

void set_init_ref_frame(double p1[3], double p2[3], double p3[3]) {
    init_ref_frame = invert_matrix(get_ref_frame(p1, p2, p3));
}

struct euler_angles Rotation(double P1[3], double P2[3], double P3[3]){
    matrix Matrix_B = get_ref_frame(P1, P2, P3);
    matrix Rotation_Matrix;

    double sum = 0;
    int R1_mat;
    int C1_mat;
    int j;
    for (R1_mat = 0; R1_mat < 3;  R1_mat++) {
      for (C1_mat = 0; C1_mat < 3; C1_mat++) {
        for (j = 0; j < 3; j++) {
           sum = sum + Matrix_B.m[R1_mat][j]*init_ref_frame.m[j][C1_mat];
        }
        Rotation_Matrix.m[R1_mat][C1_mat] = sum;
        sum = 0;
      }
    }

    struct euler_angles ret;


    ret.yaw = atan2(-(Rotation_Matrix.m[0][1]), Rotation_Matrix.m[0][0]) * 180/M_PI; // z axis

    ret.pitch = asin(Rotation_Matrix.m[0][2]) * 180/M_PI; // y axis

    if (fabs(Rotation_Matrix.m[0][2]) > 1) {
        ret.pitch = 0;
    }

    ret.roll = atan2(-(Rotation_Matrix.m[1][2]), Rotation_Matrix.m[2][2]) * 180/M_PI; // x axis

    //printf("\n\n YAW = %lf",   ret.yaw);
    //printf("\n PITCH = %lf",   ret.pitch);
    //printf("\n ROLL = %lf",   ret.roll);

    return ret;
}




