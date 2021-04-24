#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "TExaS.h"
#include "uart.h"
#include "PLL.h"

#include "calculations.h"
#include "UniversalFunctions.h"
#include "UniversalFunctions2.h"
#include "UniversalFunctions3.h"

#define a 6378137.0 //earth equatorial radius (wgs84 standard)
#define flattening 0.00335281 //flattening factor of ellipsoid (f = 1/298.257223563)
#define b 6356752.3142  //semi-minor axis (b = a * (1-f)
#define ecc 0.00669437866 //eccentricity (e^2 = 2f - f^2)
#define Pi 3.14159265358979323846

long double latitudedegreescombined, longitudedegreescombined, altitudemeterscombined;
long double latituderadianscombined, longituderadianscombined;
long double cartesianX, cartesianY, cartesianZ, N;
double p_ypr_1[3],p_ypr_2[3],p_ypr_3[3];

void getCartesian(double lat, double lng, double altitude) {
	lat *= Pi / 180;
	lng *= Pi / 180;
	double N = a/sqrt(1-ecc*pow(sin(lat),2));
	cartesianX = (N+altitude) * cos(lat) * cos(lng);
	cartesianY = (N+altitude) * cos(lat) * sin(lng);
	cartesianZ = ((1-ecc)*(N+altitude)) * sin(lat);

	printf("\n");
	//printf("\nCartesian X = %Lf", cartesianX);
	//printf("\nCartesian Y = %Lf", cartesianY);
	//printf("\nCartesian Z = %Lf", cartesianZ);
}

/*void getCartesian(double lat, double lng, double altitude, double xyz[3]) {
    lat *= Pi / 180;
    lng *= Pi / 180;
    double N = a/sqrt(1-ecc*pow(sin(lat),2));
    xyz[0] = (N+altitude) * cos(lat) * cos(lng);
    xyz[1] = (N+altitude) * cos(lat) * sin(lng);
    xyz[2] = ((1-ecc)*(N+altitude)) * sin(lat);
}
 */

void getPolar(double *lat, double *lng, double *altitude, double xyz[3]) {
	*lng = atan2(xyz[1], xyz[0]) * (180/Pi);
	
	double p = sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1]);
	double phi = atan(xyz[2] / ((1 - ecc) * p));
	double h;
	
	double N = a / sqrt(1-ecc*pow(sin(phi), 2));
	h = p / cos(phi) - N;
	phi = atan(xyz[2] / ((1 - ecc * (N / (N + h))) * p));
	
	*altitude = h;
	*lat = phi * (180 / Pi);
}

void storeCartesian(){

    /*printf("\n");
	printf("\nLatitude degrees in storeCartesian = %.8Lf", latitudedegreescombined);
	printf("\nLongitude degrees in storeCartesian = %.8Lf", longitudedegreescombined);
	printf("\nAltitude meters in storeCartesian = %.8Lf", altitudemeterscombined);*/

	latituderadianscombined = latitudedegreescombined * (Pi/180);
	longituderadianscombined = longitudedegreescombined * (Pi/180);

//	printf("\n\nLatitude radians in storeCartesian = %.8Lf", latituderadians);
//	printf("\n\nLongitude radians in storeCartesian = %.8Lf", longituderadians);

	N = a/sqrt(1-pow(ecc,2)*pow(sin(latituderadianscombined),2));
//	printf("\n\nN - Radius of curvature in prime vertical = %.8Lf", N);

	cartesianX = (N+altitudemeterscombined) * cos(latituderadianscombined) * cos(longituderadianscombined);
	cartesianY = (N+altitudemeterscombined) * cos(latituderadianscombined) * sin(longituderadianscombined);
	cartesianZ = ((1-pow(ecc,2))*(N+altitudemeterscombined)) * sin(latituderadianscombined);

/*	printf("\n\nCartesianX = %.8Lf", cartesianX);
	printf("\nCartesianY = %.8Lf", cartesianY);
	printf("\nCartesianZ = %.8Lf", cartesianZ);*/

}

void combinePositions(long double lat, long double lon, long double alt, long double lat2, long double lon2, long double alt2, long double lat3, long double lon3, long double alt3){
/*    printf("\n\nCOMBINE POSITIONS\n");

    printf("\nLatitude 1 = %Lf", lat);
    printf("\nLongitude 1 = %Lf", lon);
    printf("\nAltitude 1 = %Lf", alt);
    printf("\nLatitude 2 = %Lf", lat2);
    printf("\nLongitude 2 = %Lf", lon2);
    printf("\nAltitude 2 = %Lf", alt2);
    printf("\nLatitude 3 = %Lf", lat3);
    printf("\nLongitude 3 = %Lf", lon3);
    printf("\nAltitude 3 = %Lf", alt3);*/

    latitudedegreescombined = (lat+lat2+lat3)/3;
    longitudedegreescombined = (lon+lon2+lon3)/3;
    altitudemeterscombined = (alt+alt2+alt3)/3;

    printf("\n");
    //printf("\nLatitude combined = %.8Lf", latitudedegreescombined);
    //printf("\nLongitude combined = %.8Lf", longitudedegreescombined);
    //printf("\nAltitude combined = %.8Lf", altitudemeterscombined);

   // storeCartesian();
}

void cartesian1(long double lat, long double lon, long double alt){
    lat *= Pi / 180;
    lon *= Pi / 180;
    double N = a/sqrt(1-ecc*pow(sin(lat),2));
    p_ypr_1[0] = (N+alt) * cos(lat) * cos(lon);
    p_ypr_1[1] = (N+alt) * cos(lat) * sin(lon);
    p_ypr_1[2] = ((1-ecc)*(N+alt)) * sin(lat);

  //  printf("\n");

    //printf("\nCartesian 1 X = %Lf", p_ypr_1[0]);
    //printf("\nCartesian 1 Y = %Lf", p_ypr_1[1]);
    //printf("\nCartesian 1 Z = %Lf", p_ypr_1[2]);

    //printf("\nR1 = [%Lf;%Lf;%Lf]",p_ypr_1[0],p_ypr_1[1], p_ypr_1[2]);

}

void cartesian2(long double lat, long double lon, long double alt){
    lat *= Pi / 180;
    lon *= Pi / 180;
    double N = a/sqrt(1-ecc*pow(sin(lat),2));
    p_ypr_2[0] = (N+alt) * cos(lat) * cos(lon);
    p_ypr_2[1] = (N+alt) * cos(lat) * sin(lon);
    p_ypr_2[2] = ((1-ecc)*(N+alt)) * sin(lat);

   // printf("\n");
    //printf("\nCartesian 2 X = %Lf", p_ypr_2[0]);
    //printf("\nCartesian 2 Y = %Lf", p_ypr_2[1]);
    //printf("\nCartesian 2 Z = %Lf", p_ypr_2[2]);

  //  printf("\nR2 = [%Lf;%Lf;%Lf]",p_ypr_2[0],p_ypr_2[1], p_ypr_2[2]);
}

void cartesian3(long double lat, long double lon, long double alt){
    lat *= Pi / 180;
    lon *= Pi / 180;
    double N = a/sqrt(1-ecc*pow(sin(lat),2));
    p_ypr_3[0] = (N+alt) * cos(lat) * cos(lon);
    p_ypr_3[1] = (N+alt) * cos(lat) * sin(lon);
    p_ypr_3[2] = ((1-ecc)*(N+alt)) * sin(lat);

  //  printf("\n");

    //printf("\nCartesian 3 X = %Lf", p_ypr_3[0]);
    //printf("\nCartesian 3 Y = %Lf", p_ypr_3[1]);
    //printf("\nCartesian 3 Z = %Lf", p_ypr_3[2]);


   // printf("\nR3 = [%Lf;%Lf;%Lf]",p_ypr_3[0],p_ypr_3[1], p_ypr_3[2]);
}
