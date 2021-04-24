#ifndef Rotation_h
#define Rotation_h

struct euler_angles {
    double yaw, pitch, roll;
};

void set_init_ref_frame(double p1[3], double p2[3], double p3[3]);

double *get_mat();

struct euler_angles Rotation(double P1[3], double P2[3], double P3[3]);

#endif
