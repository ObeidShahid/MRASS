#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "TExaS.h"
#include "uart.h"
#include "PLL.h"
#include "UniversalFunctions3.h"

#define a 6378137.0 //earth equatorial radius (wgs84 standard)
#define flattening 0.00335281 //flattening factor of ellipsoid (f = 1/298.257223563)
#define b 6356752.3142  //semi-minor axis (b = a * (1-f)
#define ecc 0.08181918275 //eccentricity (e^2 = 2f - f^2)
#define Pi 3.14159265358979323846

    long double latitudedegrees3, altitudemeters3, longitudedegrees3, N3;
    long double latitudeminutes3, longitudeminutes3, latituderadians3, longituderadians3;
    char latdegrees3[2], longdegrees3[3], latminutes3[10], longminutes3[10], altmeters3[6], rangeRMS3[3], stdMajor3[3], stdMinor3[3], orient3[3], stdLat3[4], stdLong3[4], stdAlt3[4];
    char timehours3[2], timemins3[2], timesecs3[5], messageIDGGA3[6], messageIDGNS3[6], messageIDGST3[6], checksum3[3];
    char latDirection3[1], navStatus3[1], posMode2char3[2], longDirection3[1], qualityofFix3[1], numofSatellites3[2], HDOP3[4], altUnits3[1], geoidSeperation3[4], geoidUnits3[1], diffAge3[4], diffStationID3[4];
    char encodedGGA3[100], encodedGST[100], encodedRMC3[100], encodedGNS3[100];
    char *scanner3; *end3;

void extractLatitude3(void){
    //printf("\n");
    int i=0;
    if (*scanner3 != ',' && scanner3 < end3){
      ////printf("\nPointer extractLatitude %c", *scanner3);
        latdegrees3[0] = *scanner3; scanner3++;
      ////printf("\nPointer extractLatitude %c", *scanner3);
        latdegrees3[1] = *scanner3; scanner3++;
    }

    //printf("\nlatdegrees = ");

    for (i=0; i<sizeof(latdegrees3); i++){
        //printf("%c", latdegrees3[i]);
    }

    i=0;

        latitudedegrees3 = strtod(latdegrees3, NULL);
        //printf("\nLatitude degrees in convertWholeNumber = %Lf", latitudedegrees3);
        while (*scanner3 != ',' && scanner3 < end3){
            latminutes3[i] = *scanner3; i++;
            ////printf("\nPointer extractLatitude %c", *scanner3);
            scanner3++;
        }

        //printf("\nlatitudeminutes3array = ");
        for (i=0; i<sizeof(latminutes3); i++){
            //printf("%c", latminutes3[i]);
        }

        latitudeminutes3 = strtod(latminutes3, NULL);
        //printf("\nLatitude minutes in convertWholeNumber = %Lf", latitudeminutes3);
        latitudeminutes3 = latitudeminutes3 / 60;
        //printf("\nLatitude minutes / 60 = %Lf", latitudeminutes3);
        latitudedegrees3 = latitudedegrees3 + latitudeminutes3;
        //printf("\nLatitude degrees final number = %Lf", latitudedegrees3);

        ////printf("\nPointer extractLatitude %c", *scanner3);
        scanner3++;
        i=0;
}

void extractlatDirection3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractlatDirection %c", *scanner3);
        latDirection3[i] = *scanner3; i++;
    if(*scanner3 == 'S'){
        latitudedegrees3 = latitudedegrees3 * -1;
      //printf("\nLatitude degrees final number with south = %Lf", latitudedegrees3);
  }
    scanner3++;
    }
    ////printf("\nPointer extractlatDirection %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nLatDirection = %c", latDirection3[0]);
}

void extractLongitude3(void){
    //printf("\n");
    int i=0;
    if (*scanner3 != ',' && scanner3 < end3){
      ////printf("\nPointer extractLongitude %c", *scanner3);
        longdegrees3[0] = *scanner3; scanner3++;
      ////printf("\nPointer extractLongitude %c", *scanner3);
        longdegrees3[1] = *scanner3; scanner3++;
      ////printf("\nPointer extractLongitude %c", *scanner3);
        longdegrees3[2] = *scanner3; scanner3++;
    }
        longitudedegrees3 = strtod(longdegrees3, NULL);

        while (*scanner3 != ',' && scanner3 < end3){
            longminutes3[i] = *scanner3; i++;
            ////printf("\nPointer extractLongitude %c", *scanner3);
            scanner3++;
        }

        longitudeminutes3 = strtod(longminutes3, NULL);
        //printf("\nLongitude minutes after convertWholeNumber = %Lf", longitudeminutes3);
        longitudeminutes3 = longitudeminutes3 / 60.0;
        //printf("\nLongitude minutes after /60 = %Lf", longitudeminutes3);
      longitudedegrees3 = longitudedegrees3 + longitudeminutes3;
      //printf("\nLongitude degrees final number = %Lf", longitudedegrees3);

      ////printf("\nPointer extractLongitude %c", *scanner3);
    scanner3++;
    i=0;
}

void extractlongDirection3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractlongDirection %c", *scanner3);
        longDirection3[i] = *scanner3; i++;
    if(*scanner3 == 'W'){
        longitudedegrees3 = longitudedegrees3 * -1;
      //printf("\nLongitude degrees final number with south = %Lf", longitudedegrees3);
  }
    scanner3++;
    }
    ////printf("\nPointer extractlongDirection %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nLongDirection = %c", longDirection3[0]);
}

void extractqualityofFix3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractqualityofFix %c", *scanner3);
        qualityofFix3[i] = *scanner3;  i++;
        scanner3++;
    }

    //printf("\nQuality of Fix = ");
    for (i=0; i<sizeof(qualityofFix3); i++){
        //printf("%c", qualityofFix3[i]);
    }

    if (qualityofFix3[0] == '1'){
            //printf("\nQuality of fix = %c = Standard GPS (2D/3D)", *scanner3);
        }
    if (qualityofFix3[0] == '2'){
            //printf("\nQuality of fix = %c = Differential GPS", *scanner3);
        }
    if (qualityofFix3[0] == '0'){
            //printf("\nQuality of fix = %c = No Fix", *scanner3);
        }
    if (qualityofFix3[0] == '4'){
            //printf("\nQuality of fix = %c = RTK Fixed", *scanner3);
        }
    if (qualityofFix3[0] == '5'){
            //printf("\nQuality of fix = %c = RTK Float", *scanner3);
        }
    if (qualityofFix3[0] == '6'){
            //printf("\nQuality of fix = %c = Estimated (DR) Fix", *scanner3);
        }
    ////printf("\nPointer extractqualityofFix %c", *scanner3);
    scanner3++;
    i=0;
}

void extractnumofSatellites3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractnumofSatellites %c", *scanner3);
        numofSatellites3[i] = *scanner3; i++;
        scanner3++;
    }
    ////printf("\nPointer extractnumofSatellites %c", *scanner3);
    scanner3++;
    i=0;
//printf("\nNum of satellites = ");
for (i=0; i<sizeof(numofSatellites3); i++){
        //printf("%c", numofSatellites3[i]);
}
}

void extractHDOP3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractHDOP %c", *scanner3);
        HDOP3[i] = *scanner3; i++;
        scanner3++;
    }
    ////printf("\nPointer extractHDOP %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nHDOP = ");
    for (i=0; i<sizeof(HDOP3); i++){
        //printf("%c", HDOP3[i]);
    }
}

void extractAltitude3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractAltitude %c", *scanner3);
        altmeters3[i] = *scanner3; i++;
        scanner3++;
    }
    altitudemeters3 = strtod(altmeters3, NULL);
    //printf("\nAltitude meters = %.1Lf", altitudemeters3);
    ////printf("\nPointer extractAltitude %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nAltmeters = ");
    for (i=0; i<sizeof(altmeters3); i++){
        //printf("%c", altmeters3[i]);
    }
}

void extractaltitudeUnit3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractaltitudeUnit %c", *scanner3);
        altUnits3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractaltitudeUnit %c", *scanner3);
    scanner3++;
    i=0;
}

void extractgeoidSeperation3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractgeoidSeperation %c", *scanner3);
        geoidSeperation3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractgeoidSeperation %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nGeoid Seperation = ");
    for (i=0; i<sizeof(geoidSeperation3); i++){
        //printf("%c", geoidSeperation3[i]);
    }
}

void extractgeoidsepUnit3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractgeoidsepUnit %c", *scanner3);
        geoidUnits3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractgeoidsepUnit %c", *scanner3);
    scanner3++;
    i=0;
}

void extractdiffcorrectionAge3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractdiffcorrectionAge %c", *scanner3);
        diffAge3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractdiffcorrectionAge %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nDiff Correction Age = ");
    for (i=0; i<sizeof(diffAge3); i++){
        //printf("%c", diffAge3[i]);
    }
}

void extractdiffStationIDandChecksum3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != '*' && scanner3 < end3){
        ////printf("\nPointer extractdiffcorrectionStationID %c", *scanner3);
        diffStationID3[i] = *scanner3; i++;
        scanner3++;
    }
    if (*scanner3 == '*'){
        ////printf("\nPointer extractChecksum %c", *scanner3);
        checksum3[0] = *scanner3; scanner3++;
        ////printf("\nPointer extractChecksum %c", *scanner3);
        checksum3[1] = *scanner3; scanner3++;
        ////printf("\nPointer extractChecksum %c", *scanner3);
        checksum3[2] = *scanner3; scanner3++;
    }
    ////printf("\nPointer extractdiffcorrectionStationID %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nDiff Station ID = ");
    for (i=0; i<sizeof(diffStationID3); i++){
        //printf("%c", diffStationID3[i]);
    }
    //printf("\nChecksum = ");
    for (i=0; i<sizeof(checksum3); i++){
        //printf("%c", checksum3[i]);
    }
}

void extractdiffStationID3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractdiffcorrectionStationID %c", *scanner3);
        diffStationID3[i] = *scanner3; i++;
        scanner3++;
    }
    ////printf("\nPointer extractdiffcorrectionStationID %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nDiff Station ID = ");
    for (i=0; i<sizeof(diffStationID3); i++){
        //printf("%c", diffStationID3[i]);
    }
}

void skipNameGGA3(void){
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
      ////printf("\nPointer skipName %c", *scanner3);
        messageIDGGA3[i] = *scanner3; i++;
        scanner3++;
    }
      ////printf("\nPointer skipName %c", *scanner3);
  scanner3++;
    i=0;
    //printf("\nMessage ID = ");
    for (i=0; i<sizeof(messageIDGGA3); i++){
        //printf("%c", messageIDGGA3[i]);
    }
}

void skipNameGNS3(void){
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
      ////printf("\nPointer skipName %c", *scanner3);
        messageIDGNS3[i] = *scanner3; i++;
        scanner3++;
    }
      ////printf("\nPointer skipName %c", *scanner3);
  scanner3++;
    i=0;
    //printf("\nMessage ID = ");
    for (i=0; i<sizeof(messageIDGNS3); i++){
        //printf("%c", messageIDGNS3[i]);
    }
}

void skipNameGST3(void){
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
      ////printf("\nPointer skipName %c", *scanner3);
        messageIDGST3[i] = *scanner3; i++;
        scanner3++;
    }
      ////printf("\nPointer skipName %c", *scanner3);
  scanner3++;
    i=0;
    //printf("\nMessage ID = ");
    for (i=0; i<sizeof(messageIDGST3); i++){
        //printf("%c", messageIDGST3[i]);
    }
}

void Skip3(void){
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
      ////printf("\nPointer skipName %c", *scanner3);
        scanner3++;
    }
      ////printf("\nPointer skipName %c", *scanner3);
  scanner3++;
}

void extractTime3(void){
    //printf("\n");
    if (*scanner3 != ',' && scanner3 < end3){
      ////printf("\nPointer extractTime %c", *scanner3);
        timehours3[0] = *scanner3; scanner3++;
      ////printf("\nPointer extractTime %c", *scanner3);
        timehours3[1] = *scanner3; scanner3++;
      ////printf("\nPointer extractTime %c", *scanner3);
        timemins3[0] = *scanner3; scanner3++;
      ////printf("\nPointer extractTime %c", *scanner3);
        timemins3[1] = *scanner3; scanner3++;
      ////printf("\nPointer extractTime %c", *scanner3);
        timesecs3[0] = *scanner3; scanner3++;
      ////printf("\nPointer extractTime %c", *scanner3);
        timesecs3[1] = *scanner3; scanner3++;
      ////printf("\nPointer extractTime %c", *scanner3);
        timesecs3[2] = *scanner3; scanner3++;
      ////printf("\nPointer extractTime %c", *scanner3);
        timesecs3[3] = *scanner3; scanner3++;
      ////printf("\nPointer extractTime %c", *scanner3);
      timesecs3[4] = *scanner3; scanner3++;
    }

        printf("\nTime = %c%c:%c%c:%c%c%c%c%c", timehours3[0],timehours3[1],timemins3[0],timemins3[1],timesecs3[0], timesecs3[1],timesecs3[2],timesecs3[3],timesecs3[4] );
      ////printf("\nPointer extractTime %c", *scanner3);
      scanner3++;
}

/*void storeCartesian3(){
  //printf("\n");
    //printf("\nLatitude degrees in storeCartesian = %Lf", latitudedegrees3);
    //printf("\nLongitude degrees in storeCartesian = %Lf", longitudedegrees3);
    //printf("\nAltitude meters in storeCartesian = %Lf", altitudemeters3);

    latituderadians3 = latitudedegrees3 * (Pi/180);
    longituderadians3 = longitudedegrees3 * (Pi/180);

    //printf("\n\nLatitude radians in storeCartesian = %Lf", latituderadians3);
    //printf("\n\nLongitude radians in storeCartesian = %Lf", longituderadians3);

    N3 = a/sqrt(1-pow(ecc,2)*pow(sin(latituderadians3),2));
    //printf("\n\nN - Radius of curvature in prime vertical = %Lf", N);

    cartesianX3 = (N3+altitudemeters3) * cos(latituderadians3) * cos(longituderadians3);
    cartesianY3 = (N3+altitudemeters3) * cos(latituderadians3) * sin(longituderadians3);
    cartesianZ3 = ((1-pow(ecc,2))*(N3+altitudemeters3)) * sin(latituderadians3);

    //printf("\n\nCartesianX = %Lf", cartesianX3);
    //printf("\nCartesianY = %Lf", cartesianY3);
    //printf("\nCartesianZ = %Lf", cartesianZ3);

}*/

void extractnavStatusandChecksum3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != '*' && scanner3 < end3){
        ////printf("\nPointer extractnavStatusandChecksum %c", *scanner3);
        navStatus3[i] = *scanner3;  i++;
        scanner3++;
    }
    if (*scanner3 == '*'){
        ////printf("\nPointer extractChecksum %c", *scanner3);
        checksum3[0] = *scanner3; scanner3++;
        ////printf("\nPointer extractChecksum %c", *scanner3);
        checksum3[1] = *scanner3; scanner3++;
        ////printf("\nPointer extractChecksum %c", *scanner3);
        checksum3[2] = *scanner3; scanner3++;
    }
    ////printf("\nPointer extractnavStatusandChecksum %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nextractnavStatus = ");
    for (i=0; i<sizeof(navStatus3); i++){
        //printf("%c", navStatus3[i]);
    }
    //printf("\nChecksum = ");
    for (i=0; i<sizeof(checksum3); i++){
        //printf("%c", checksum3[i]);
    }
}

 void extractnavStatus3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractnavStatus %c", *scanner3);
        navStatus3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractnavStatus %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nextractnavStatus = ");
    for (i=0; i<sizeof(navStatus3); i++){
        //printf("%c", navStatus3[i]);
    }
}

void extractPosMode2char3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractPosMode2char %c", *scanner3);
        posMode2char3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractPosMode2char %c", *scanner3);
    scanner3++;
    i=0;
    //printf("\nPosMode2char = ");
    for (i=0; i<sizeof(posMode2char3); i++){
        //printf("%c", posMode2char3[i]);
    }
}

void extractRangeRMS3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractRangeRMS %c", *scanner3);
        rangeRMS3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractRangeRMS %c", *scanner3);
    scanner3++;
    i=0;
    printf("\nRange RMS = ");
    for (i=0; i<sizeof(rangeRMS3); i++){
        printf("%c", rangeRMS3[i]);
    }
}

void extractstdMajor3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractstdMajor %c", *scanner3);
        stdMajor3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractstdMajor %c", *scanner3);
    scanner3++;
    i=0;
    printf("\nstd Major = ");
    for (i=0; i<sizeof(stdMajor3); i++){
        printf("%c", stdMajor3[i]);
    }
}

void extractstdMinor3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractstdMinor %c", *scanner3);
        stdMinor3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractstdMinor %c", *scanner3);
    scanner3++;
    i=0;
    printf("\nstd Minor = ");
    for (i=0; i<sizeof(stdMinor3); i++){
        printf("%c", stdMinor3[i]);
    }
}

void extractOrient3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractOrient %c", *scanner3);
        orient3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractOrient %c", *scanner3);
    scanner3++;
    i=0;
    printf("\nOrient = ");
    for (i=0; i<sizeof(orient3); i++){
        printf("%c", orient3[i]);
    }
}

void extractstdLat3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractstdLat %c", *scanner3);
        stdLat3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractstdLat %c", *scanner3);
    scanner3++;
    i=0;
    printf("\nstd Lat = ");
    for (i=0; i<sizeof(stdLat3); i++){
        printf("%c", stdLat3[i]);
    }
}

void extractstdLong3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractstdLong %c", *scanner3);
        stdLong3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractstdLong %c", *scanner3);
    scanner3++;
    i=0;
    printf("\nstd Long = ");
    for (i=0; i<sizeof(stdLong3); i++){
        printf("%c", stdLong3[i]);
    }
}

void extractstdAlt3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != ',' && scanner3 < end3){
        ////printf("\nPointer extractstdAlt %c", *scanner3);
        stdAlt3[i] = *scanner3;  i++;
        scanner3++;
    }
    ////printf("\nPointer extractstdAlt %c", *scanner3);
    scanner3++;
    i=0;
    printf("\nstd Alt = ");
    for (i=0; i<sizeof(stdAlt3); i++){
        printf("%c", stdAlt3[i]);
    }
}

void extractstdAltandChecksum3(void){
    //printf("\n");
    int i=0;
    while (*scanner3 != '*' && scanner3 < end3){
        ////printf("\nPointer extractstdAlt %c", *scanner3);
        stdAlt3[i] = *scanner3;  i++;
        scanner3++;
    }
    if (*scanner3 == '*'){
        ////printf("\nPointer extractChecksum %c", *scanner3);
        checksum3[0] = *scanner3; scanner3++;
        ////printf("\nPointer extractChecksum %c", *scanner3);
        checksum3[1] = *scanner3; scanner3++;
        ////printf("\nPointer extractChecksum %c", *scanner3);
        checksum3[2] = *scanner3; scanner3++;
    }
    ////printf("\nPointer extractstdAlt %c", *scanner3);
    scanner3++;
    i=0;
    printf("\nstd Alt = ");
    for (i=0; i<sizeof(stdAlt3); i++){
        printf("%c", stdAlt3[i]);
    }
    printf("\nChecksum = ");
    for (i=0; i<sizeof(checksum3); i++){
        printf("%c", checksum3[i]);
    }
}

void parseGGA3(char GGA[]){
    scanner3 = GGA;
    skipNameGGA3();
    //extractTime3();
    extractLatitude3();
    extractlatDirection3();
    extractLongitude3();
    extractlongDirection3();
    extractqualityofFix3();
    extractnumofSatellites3();
    extractHDOP3();
    extractAltitude3();
    extractaltitudeUnit3();
    extractgeoidSeperation3();
    extractgeoidsepUnit3();
    extractdiffcorrectionAge3();
    extractdiffStationIDandChecksum3();
    //storeCartesian3();
}

void parseGNS3(char GNS[]){
    scanner3 = GNS;
    skipNameGNS3();
   // extractTime3();
    extractLatitude3();
    extractlatDirection3();
    extractLongitude3();
    extractlongDirection3();
    extractPosMode2char3();
    extractnumofSatellites3();
    extractHDOP3();
    extractAltitude3();
    extractgeoidSeperation3();
    extractdiffcorrectionAge3();
    extractdiffStationID3();
    extractnavStatusandChecksum3();
}

void parseGST3(char GST[]){
    scanner3 = GST;
    skipNameGST3();
    extractTime3();
    extractRangeRMS3();
    extractstdMajor3();
    extractstdMinor3();
    extractOrient3();
    //Skip3();
    //Skip3();
    //Skip3();
    extractstdLat3();
    extractstdLong3();
    extractstdAltandChecksum3();
}

void encodeGST(void){

    int x=0;
    int i=0 ;
    for (i=0; i<sizeof(messageIDGST3); i++){
        encodedGST[x] = messageIDGST3[i]; x++;
    }
  encodedGST[x] = ','; x++;
  for ( i=0; i<sizeof(timehours3); i++){
      encodedGST[x] = timehours3[i]; x++;
  }
  for (i=0; i<sizeof(timemins3); i++){
      encodedGST[x] = timemins3[i]; x++;
  }
  for (i=0; i<sizeof(timesecs3); i++){
      encodedGST[x] = timesecs3[i]; x++;
  }
  encodedGST[x] = ','; x++;
for (i=0; i<sizeof(rangeRMS3); i++){
    encodedGST[x] = rangeRMS3[i]; x++;
}
  encodedGST[x] = ','; x++;
for (i=0; i<sizeof(stdMajor3); i++){
    encodedGST[x] = stdMajor3[i]; x++;
}
  encodedGST[x] = ','; x++;
for (i=0; i<sizeof(stdMinor3); i++){
    encodedGST[x] = stdMinor3[i]; x++;
}
  encodedGST[x] = ','; x++;
for (i=0; i<sizeof(orient3); i++){
    encodedGST[x] = orient3[i]; x++;
}
  encodedGST[x] = ','; x++;
for (i=0; i<sizeof(stdLat3); i++){
    encodedGST[x] = stdLat3[i]; x++;
}
  encodedGST[x] = ','; x++;
for (i=0; i<sizeof(stdLong3); i++){
    encodedGST[x] = stdLong3[i]; x++;
}
encodedGST[x] = ','; x++;
for (i=0; i<sizeof(stdAlt3); i++){
  encodedGST[x] = stdAlt3[i]; x++;
}
for (i=0; i<sizeof(checksum3); i++){
  encodedGST[x] = checksum3[i]; x++;
}

printf("\n\nEncoded NMEA sentence = ");
for (i=0; i<sizeof(encodedGST); i++){
       printf("%c", encodedGST[i]);
    }
    printf("\n");
}

void findStream3(char stream[]){
    int i=0;
    end3 = stream + 101;
    if(stream[3] == 'G' && stream[4] == 'G' && stream[5] == 'A'){
        //printf("\nSentence found - GGA\n");
        //printf("\n\nNmea sentence to decode = ");
        for (i=0; i<84; i++){
            //printf("%c", stream[i]);
        }
      parseGGA3(stream);
    }

    else if(stream[3] == 'G' && stream[4] == 'N' && stream[5] == 'S'){
        //printf("\nSentence found - GNS\n");
        //printf("\nNmea sentence to decode = ");
        for (i=0; i<84; i++){
            //printf("%c", stream[i]);
        }
        //printf("\n");
      parseGNS3(stream);
    }

    else if(stream[3] == 'G' && stream[4] == 'S' && stream[5] == 'T'){
        printf("\nSentence found - GST\n");
        printf("\nNmea sentence to decode = ");
        for (i=0; i<40; i++){
            printf("%c", stream[i]);
        }
        //printf("\n");
      parseGST3(stream);
    }

    else{
        //printf("\n\nNo Stream Found");
    }
}
