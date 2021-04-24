
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "TExaS.h"
#include "uart.h"
#include "PLL.h"
#include "UniversalFunctions2.h"

#define a 6378137.0 //earth equatorial radius (wgs84 standard)
#define flattening 0.00335281 //flattening factor of ellipsoid (f = 1/298.257223563)
#define b 6356752.3142  //semi-minor axis (b = a * (1-f)
#define ecc 0.08181918275 //eccentricity (e^2 = 2f - f^2)
#define Pi 3.14159265358979323846 

  long double latitudedegrees2, altitudemeters2, longitudedegrees2, N2; //cartesianX2, cartesianY2, cartesianZ2,
    long double latitudeminutes2, longitudeminutes2, latituderadians2, longituderadians2;
    char latdegrees2[2], longdegrees2[3], latminutes2[10], longminutes2[10], altmeters2[6], rangeRMS2[3], stdMajor2[3], stdMinor2[3], orient2[3], stdLat2[3], stdLong2[3], stdAlt2[3];
    char timehours2[2], timemins2[2], timesecs2[5], messageIDGGA2[6], messageIDGNS2[6], messageIDGST2[6], checksum2[3];
    char latDirection2[1], navStatus2[1], posMode2char2[2], longDirection2[1], qualityofFix2[1], numofSatellites2[2], HDOP2[4], altUnits2[1], geoidSeperation2[4], geoidUnits2[1], diffAge2[4], diffStationID2[4];
    char  encodedGNS[100], encodedGST[100];
    char *scanner2, *end2;
  
void extractLatitude2(void){
    //printf("\n");
    int i=0;
    if (*scanner2 != ',' && scanner2 < end2){
      //printf("\nPointer extractLatitude %c", *scanner2);
        latdegrees2[0] = *scanner2; scanner2++;
      //printf("\nPointer extractLatitude %c", *scanner2);
        latdegrees2[1] = *scanner2; scanner2++;
    }

    //printf("\nlatdegrees = ");
    for (i=0; i<sizeof(latdegrees2); i++){
        //printf("%c", latdegrees2[i]);
    }

    i=0;

        latitudedegrees2 = strtod(latdegrees2, NULL);
        //printf("\nLatitude degrees in convertWholeNumber = %Lf", latitudedegrees2);
        while (*scanner2 != ',' && scanner2 < end2){
            latminutes2[i] = *scanner2; i++;
            //printf("\nPointer extractLatitude %c", *scanner2);
            scanner2++;
        }

        //printf("\nlatitudeminutes2array = ");
        for (i=0; i<sizeof(latminutes2); i++){
            //printf("%c", latminutes2[i]);
        }

        latitudeminutes2 = strtod(latminutes2, NULL);
        //printf("\nLatitude minutes in convertWholeNumber = %Lf", latitudeminutes2);
        latitudeminutes2 = latitudeminutes2 / 60;
        //printf("\nLatitude minutes / 60 = %Lf", latitudeminutes2);
        latitudedegrees2 = latitudedegrees2 + latitudeminutes2;
        //printf("\nLatitude degrees = %Lf", latitudedegrees2);

      //printf("\nPointer extractLatitude %c", *scanner2);
      scanner2++;
        i=0;
}

void extractlatDirection2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractlatDirection %c", *scanner2);
        latDirection2[i] = *scanner2; i++;
    if(*scanner2 == 'S'){
        latitudedegrees2 = latitudedegrees2 * -1;
      //printf("\nLatitude degrees final number with south = %Lf", latitudedegrees2);
  }
    scanner2++;
    }
    //printf("\nPointer extractlatDirection %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nLatDirection = %c", latDirection2[0]);
}

void extractLongitude2(void){
    //printf("\n");
    int i=0;
    if (*scanner2 != ',' && scanner2 < end2){
      //printf("\nPointer extractLongitude %c", *scanner2);
        longdegrees2[0] = *scanner2; scanner2++;
      //printf("\nPointer extractLongitude %c", *scanner2);
        longdegrees2[1] = *scanner2; scanner2++;
      //printf("\nPointer extractLongitude %c", *scanner2);
        longdegrees2[2] = *scanner2; scanner2++;
    }
        longitudedegrees2 = strtod(longdegrees2, NULL);

        while (*scanner2 != ',' && scanner2 < end2){
            longminutes2[i] = *scanner2; i++;
            //printf("\nPointer extractLongitude %c", *scanner2);
            scanner2++;
        }

        longitudeminutes2 = strtod(longminutes2, NULL);
        //printf("\nLongitude minutes after convertWholeNumber = %Lf", longitudeminutes2);
        longitudeminutes2 = longitudeminutes2 / 60.0;
        //printf("\nLongitude minutes after /60 = %Lf", longitudeminutes2);
      longitudedegrees2 = longitudedegrees2 + longitudeminutes2;
      //printf("\nLongitude degrees = %Lf", longitudedegrees2);

      //printf("\nPointer extractLongitude %c", *scanner2);
    scanner2++;
    i=0;
}

void extractlongDirection2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractlongDirection %c", *scanner2);
        longDirection2[i] = *scanner2; i++;
    if(*scanner2 == 'W'){
        longitudedegrees2 = longitudedegrees2 * -1;
      //printf("\nLongitude degrees final number with south = %Lf", longitudedegrees2);
  }
    scanner2++;
    }
    //printf("\nPointer extractlongDirection %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nLongDirection = %c", longDirection2[0]);
}

void extractqualityofFix2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractqualityofFix %c", *scanner2);
        qualityofFix2[i] = *scanner2;  i++;
        scanner2++;
    }

    //printf("\nQuality of Fix = ");
    for(i=0; i<sizeof(qualityofFix2); i++){
        //printf("%c", qualityofFix2[i]);
    }

    if (qualityofFix2[0] == '1'){
            //printf("\nQuality of fix = %c = Standard GPS (2D/3D)", qualityofFix2[0]);
        }
    if (qualityofFix2[0] == '2'){
            //printf("\nQuality of fix = %c = Differential GPS", qualityofFix2[0]);
        }
    if (qualityofFix2[0] == '0'){
            //printf("\nQuality of fix = %c = No Fix", qualityofFix2[0]);
        }
    if (qualityofFix2[0] == '4'){
            //printf("\nQuality of fix = %c = RTK Fixed", qualityofFix2[0]);
        }
    if (qualityofFix2[0] == '5'){
            //printf("\nQuality of fix = %c = RTK Float", qualityofFix2[0]);
        }
    if (qualityofFix2[0] == '6'){
            //printf("\nQuality of fix = %c = Estimated (DR) Fix", qualityofFix2[0]);
        }
    //printf("\nPointer extractqualityofFix %c", *scanner2);
    scanner2++;
    i=0;
}

void extractnumofSatellites2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractnumofSatellites %c", *scanner2);
        numofSatellites2[i] = *scanner2; i++;
        scanner2++;
    }
    //printf("\nPointer extractnumofSatellites %c", *scanner2);
    scanner2++;
    i=0;
//printf("\nNum of satellites = ");
for(i=0; i<sizeof(numofSatellites2); i++){
        //printf("%c", numofSatellites2[i]);
}
}

void extractHDOP2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractHDOP %c", *scanner2);
        HDOP2[i] = *scanner2; i++;
        scanner2++;
    }
    //printf("\nPointer extractHDOP %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nHDOP = ");
    for (i=0; i<sizeof(HDOP2); i++){
        //printf("%c", HDOP2[i]);
    }
}

void extractAltitude2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractAltitude %c", *scanner2);
        altmeters2[i] = *scanner2; i++;
        scanner2++;
    }
    altitudemeters2 = strtod(altmeters2, NULL);
    //printf("\nAltitude meters = %.1Lf", altitudemeters2);
    //printf("\nPointer extractAltitude %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nAltmeters = ");
    for (i=0; i<sizeof(altmeters2); i++){
        //printf("%c", altmeters2[i]);
    }
}

void extractaltitudeUnit2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractaltitudeUnit %c", *scanner2);
        altUnits2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractaltitudeUnit %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nAltitude Units = ");
    for (i=0; i<sizeof(altUnits2); i++){
        //printf("%c", altUnits2[i]);
    }
}

void extractgeoidSeperation2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractgeoidSeperation %c", *scanner2);
        geoidSeperation2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractgeoidSeperation %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nGeoid Seperation = ");
    for (i=0; i<sizeof(geoidSeperation2); i++){
        //printf("%c", geoidSeperation2[i]);
    }
}

void extractgeoidsepUnit2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractgeoidsepUnit %c", *scanner2);
        geoidUnits2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractgeoidsepUnit %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nGeoid Seperation Unit = ");
    for (i=0; i<sizeof(geoidUnits2); i++){
        //printf("%c", geoidUnits2[i]);
    }
}

void extractdiffcorrectionAge2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractdiffcorrectionAge %c", *scanner2);
        diffAge2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractdiffcorrectionAge %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nDiff Correction Age = ");
    for (i=0; i<sizeof(diffAge2); i++){
        //printf("%c", diffAge2[i]);
    }
}

void extractdiffStationIDandChecksum2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != '*' && scanner2 < end2){
        //printf("\nPointer extractdiffcorrectionStationID %c", *scanner2);
        diffStationID2[i] = *scanner2; i++;
        scanner2++;
    }
    if (*scanner2 == '*'){
        //printf("\nPointer extractChecksum %c", *scanner2);
        checksum2[0] = *scanner2; scanner2++;
        //printf("\nPointer extractChecksum %c", *scanner2);
        checksum2[1] = *scanner2; scanner2++;
        //printf("\nPointer extractChecksum %c", *scanner2);
        checksum2[2] = *scanner2; scanner2++;
    }
    //printf("\nPointer extractdiffcorrectionStationID %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nDiff Station ID = ");
    for (i=0; i<sizeof(diffStationID2); i++){
        //printf("%c", diffStationID2[i]);
    }
    //printf("\nChecksum = ");
    for (i=0; i<sizeof(checksum2); i++){
        //printf("%c", checksum2[i]);
    }
}

void extractdiffStationID2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractdiffcorrectionStationID %c", *scanner2);
        diffStationID2[i] = *scanner2; i++;
        scanner2++;
    }
    //printf("\nPointer extractdiffcorrectionStationID %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nDiff Station ID = ");
    for (i=0; i<sizeof(diffStationID2); i++){
        //printf("%c", diffStationID2[i]);
    }
}

void skipNameGGA2(void){
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
      //printf("\nPointer skipName %c", *scanner2);
        messageIDGGA2[i] = *scanner2; i++;
        scanner2++;
    }
      //printf("\nPointer skipName %c", *scanner2);
  scanner2++;
    i=0;
    //printf("\nMessage ID = ");
    for (i=0; i<sizeof(messageIDGGA2); i++){
        //printf("%c", messageIDGGA2[i]);
    }
}

void skipNameGNS2(void){
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
      //printf("\nPointer skipName %c", *scanner2);
        messageIDGNS2[i] = *scanner2; i++;
        scanner2++;
    }
      //printf("\nPointer skipName %c", *scanner2);
  scanner2++;
    i=0;
    //printf("\nMessage ID = ");
    for (i=0; i<sizeof(messageIDGNS2); i++){
        //printf("%c", messageIDGNS2[i]);
    }
}

void skipNameGST2(void){
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
      //printf("\nPointer skipName %c", *scanner2);
        messageIDGST2[i] = *scanner2; i++;
        scanner2++;
    }
      //printf("\nPointer skipName %c", *scanner2);
  scanner2++;
    i=0;
    //printf("\nMessage ID = ");
    for (i=0; i<sizeof(messageIDGST2); i++){
        //printf("%c", messageIDGST2[i]);
    }
}

void Skip2(void){
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
      //printf("\nPointer skipName %c", *scanner2);
        scanner2++;
    }
      //printf("\nPointer skipName %c", *scanner2);
  scanner2++;
}

void extractTime2(void){
    //printf("\n");
    if (*scanner2 != ',' && scanner2 < end2){
      //printf("\nPointer extractTime %c", *scanner2);
        timehours2[0] = *scanner2; scanner2++;
      //printf("\nPointer extractTime %c", *scanner2);
        timehours2[1] = *scanner2; scanner2++;
      //printf("\nPointer extractTime %c", *scanner2);
        timemins2[0] = *scanner2; scanner2++;
      //printf("\nPointer extractTime %c", *scanner2);
        timemins2[1] = *scanner2; scanner2++;
      //printf("\nPointer extractTime %c", *scanner2);
        timesecs2[0] = *scanner2; scanner2++;
      //printf("\nPointer extractTime %c", *scanner2);
        timesecs2[1] = *scanner2; scanner2++;
      //printf("\nPointer extractTime %c", *scanner2);
        timesecs2[2] = *scanner2; scanner2++;
      //printf("\nPointer extractTime %c", *scanner2);
        timesecs2[3] = *scanner2; scanner2++;
      //printf("\nPointer extractTime %c", *scanner2);
      timesecs2[4] = *scanner2; scanner2++;
    }

    //printf("\nTime = %c%c:%c%c:%c%c%c%c%c", timehours2[0],timehours2[1],timemins2[0],timemins2[1],timesecs2[0], timesecs2[1],timesecs2[2],timesecs2[3],timesecs2[4] );
      //printf("\nPointer extractTime %c", *scanner2);
      scanner2++;
}
 
void extractnavStatusandChecksum2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != '*' && scanner2 < end2){
        //printf("\nPointer extractnavStatusandChecksum %c", *scanner2);
        navStatus2[i] = *scanner2;  i++;
        scanner2++;
    }
    if (*scanner2 == '*'){
        //printf("\nPointer extractChecksum %c", *scanner2);
        checksum2[0] = *scanner2; scanner2++;
        //printf("\nPointer extractChecksum %c", *scanner2);
        checksum2[1] = *scanner2; scanner2++;
        //printf("\nPointer extractChecksum %c", *scanner2);
        checksum2[2] = *scanner2; scanner2++;
    }
    //printf("\nPointer extractnavStatusandChecksum %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nextractnavStatus = ");
    for (i=0; i<sizeof(navStatus2); i++){
        //printf("%c", navStatus2[i]);
    }
    //printf("\nChecksum = ");
    for (i=0; i<sizeof(checksum2); i++){
        //printf("%c", checksum2[i]);
    }
}

 void extractnavStatus2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractnavStatus %c", *scanner2);
        navStatus2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractnavStatus %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nextractnavStatus = ");
    for (i=0; i<sizeof(navStatus2); i++){
        //printf("%c", navStatus2[i]);
    }
}
 
void extractPosMode2char2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractPosMode2char %c", *scanner2);
        posMode2char2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractPosMode2char %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nPosMode2char = ");
    for (i=0; i<sizeof(posMode2char2); i++){
        //printf("%c", posMode2char2[i]);
    }
}

void extractRangeRMS2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractRangeRMS %c", *scanner2);
        rangeRMS2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractRangeRMS %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nRange RMS = ");
    for (i=0; i<sizeof(rangeRMS2); i++){
        //printf("%c", rangeRMS2[i]);
    }
}

void extractstdMajor2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractstdMajor %c", *scanner2);
        stdMajor2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractstdMajor %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nstd Major = ");
    for (i=0; i<sizeof(stdMajor2); i++){
        //printf("%c", stdMajor2[i]);
    }
}

void extractstdMinor2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractstdMinor %c", *scanner2);
        stdMinor2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractstdMinor %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nstd Minor = ");
    for (i=0; i<sizeof(stdMinor2); i++){
        //printf("%c", stdMinor2[i]);
    }
}

void extractOrient2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractOrient %c", *scanner2);
        orient2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractOrient %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nOrient = ");
    for (i=0; i<sizeof(orient2); i++){
        //printf("%c", orient2[i]);
    }
}

void extractstdLat2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractstdLat %c", *scanner2);
        stdLat2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractstdLat %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nstd Lat = ");
    for (i=0; i<sizeof(stdLat2); i++){
        //printf("%c", stdLat2[i]);
    }
}

void extractstdLong2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractstdLong %c", *scanner2);
        stdLong2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractstdLong %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nstd Long = ");
    for (i=0; i<sizeof(stdLong2); i++){
        //printf("%c", stdLong2[i]);
    }
}

void extractstdAlt2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != ',' && scanner2 < end2){
        //printf("\nPointer extractstdAlt %c", *scanner2);
        stdAlt2[i] = *scanner2;  i++;
        scanner2++;
    }
    //printf("\nPointer extractstdAlt %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nstd Alt = ");
    for (i=0; i<sizeof(stdAlt2); i++){
        //printf("%c", stdAlt2[i]);
    }
}

void extractstdAltandChecksum2(void){
    //printf("\n");
    int i=0;
    while (*scanner2 != '*' && scanner2 < end2){
        //printf("\nPointer extractstdAlt %c", *scanner2);
        stdAlt2[i] = *scanner2;  i++;
        scanner2++;
    }
    if (*scanner2 == '*'){
        //printf("\nPointer extractChecksum %c", *scanner2);
        checksum2[0] = *scanner2; scanner2++;
        //printf("\nPointer extractChecksum %c", *scanner2);
        checksum2[1] = *scanner2; scanner2++;
        //printf("\nPointer extractChecksum %c", *scanner2);
        checksum2[2] = *scanner2; scanner2++;
    }
    //printf("\nPointer extractstdAlt %c", *scanner2);
    scanner2++;
    i=0;
    //printf("\nstd Alt = ");
    for (i=0; i<sizeof(stdAlt2); i++){
        //printf("%c", stdAlt2[i]);
    }
    //printf("\nChecksum = ");
    for (i=0; i<sizeof(checksum2); i++){
        //printf("%c", checksum2[i]);
    }
}

void parseGGA2(char GGA[]){
    scanner2 = GGA;
    skipNameGGA2();
    extractTime2();
    extractLatitude2();
    extractlatDirection2();
    extractLongitude2();
    extractlongDirection2();
    extractqualityofFix2();
    extractnumofSatellites2();
    extractHDOP2();
    extractAltitude2();
    extractaltitudeUnit2();
    extractgeoidSeperation2();
    extractgeoidsepUnit2();
    extractdiffcorrectionAge2();
    extractdiffStationIDandChecksum2();
}

void parseGNS2(char GNS[]){
    scanner2 = GNS;
    skipNameGNS2();
    extractTime2();
    extractLatitude2();
    extractlatDirection2();
    extractLongitude2();
    extractlongDirection2();
    extractPosMode2char2();
    extractnumofSatellites2();
    extractHDOP2();
    extractAltitude2();
    extractgeoidSeperation2();
    extractdiffcorrectionAge2();
    extractdiffStationID2();
    extractnavStatusandChecksum2();
}
 
void parseGST2(char GST[]){
    scanner2 = GST;
    skipNameGST2();
    extractTime2();
    extractRangeRMS2();
    extractstdMajor2();
    extractstdMinor2();
    extractOrient2();
    extractstdLat2();
    extractstdLong2();
    extractstdAltandChecksum2();
}

void encodeGNS(double lat, double lon, double alt) {
    int is_south = (lat < 0.0);
    char latentry[11];
    lat = fabs(lat);
    int degree = (int)floor(lat);
    lat -= degree;
    lat *= 60;

    sprintf(latentry, "%02d%8.5f", degree, lat);

    int is_west = (lon < 0.0);
    char lonentry[12];
    lon = fabs(lon);
    degree = (int)floor(lon);
    lon -= degree;
    lon *= 60;

    sprintf(lonentry, "%03d%8.5f", degree, lon);

    int i = 0;
    int cnt = 0;
    while (encodedGNS[i] != '\0' && cnt != 2) {
        //printf("%c", encodedGNS[i]);
        if (encodedGNS[i] == ',') {
            cnt++;
        }
        i++;
    }

    //printf("%s,%c,", latentry, (is_south)? 'S': 'N');
    //printf("%s,%c,", lonentry, (is_west)? 'W': 'E');

    while (encodedGNS[i] != '\0' && cnt != 6) {
        if (encodedGNS[i] == ',') {
                    cnt++;
        }
        i++;
    }

    while (encodedGNS[i] != '\0' && cnt != 9) {
            //printf("%c", encodedGNS[i]);
            if (encodedGNS[i] == ',') {
                cnt++;
            }
            i++;
    }

    //printf("%.01f,", alt);

    while (encodedGNS[i] != '\0' && cnt != 10) {
            if (encodedGNS[i] == ',') {
                        cnt++;
            }
            i++;
    }

   // printf("%s", encodedGNS + i);

/*
    int x=0, i=0;

    char lattoString[9],lontoString[10],alttoString[6];
    char latDegreesNew[3], longdegreesnew[4], lonDegreesNew[4], latMinutesNew[8], latMinutesFinal[8], lonMinutesNew[7];
    double latitudedegrees, latitudeminutes;

        sprintf(lattoString, "%.5lf", lat);
        //printf("\nlattoString = %s", lattoString);
        sprintf(lontoString, "%.5lf", lon);
        //printf("\nlontoString = %s", lontoString);
        sprintf(alttoString, "%.5lf", alt);
        //printf("\nalttoString = %s", alttoString);

        memcpy(latDegreesNew, lattoString, 2);
        //printf("\nlatDegreesNew = %s", latDegreesNew);
        memcpy(latMinutesNew, lattoString+2, 7);
        //printf("\nlatMinutesNew = %s", latMinutesNew);

        memcpy(longdegreesnew, lontoString, 1);
        //printf("\nlongdegreesnew = %s", longdegreesnew);
        memcpy(lonMinutesNew, lontoString+1, 7);
        //printf("\nlonMinutesNew = %s", lonMinutesNew);

        latitudeminutes = strtod(latMinutesNew,NULL);
        //printf("\nlatitudeminutes = %lf", latitudeminutes);

        latitudeminutes = latitudeminutes * 60;
        //printf("\nlatitudeminutes = %lf", latitudeminutes);

        sprintf(latMinutesFinal, "%.5lf", latitudeminutes);
        //printf("\nlattoString = %s", latMinutesFinal);

    //printf("\nLatitude = %s%s", latDegreesNew,latMinutesFinal);
    //printf("\nLongitude = %s%s", lonDegreesNew,lonMinutesNew);
    //printf("\nAltitude = %s", alttoString);

    i=0; x=0;

    for (i=0; i<sizeof(messageIDGNS2); i++){
        encodedGNS[x] = messageIDGNS2[i]; x++;
    }
    encodedGNS[x] = ','; x++;
  for ( i=0; i<sizeof(timehours2); i++){
      encodedGNS[x] = timehours2[i]; x++;
  }
  for (i=0; i<sizeof(timemins2); i++){
      encodedGNS[x] = timemins2[i]; x++;
  }
  for (i=0; i<sizeof(timesecs2); i++){
      encodedGNS[x] = timesecs2[i]; x++;
  }
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(latdegrees2); i++){
    encodedGNS[x] = latdegrees2[i]; x++;
}
for (i=0; i<sizeof(latminutes2); i++){
    encodedGNS[x] = latminutes2[i]; x++;
}
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(latDirection2); i++){
    encodedGNS[x] = latDirection2[i]; x++;
}
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(longdegrees2); i++){
    encodedGNS[x] = longdegrees2[i]; x++;
}
for (i=0; i<sizeof(longminutes2); i++){
    encodedGNS[x] = longminutes2[i]; x++;
}
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(longDirection2); i++){
    encodedGNS[x] = longDirection2[i]; x++;
}
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(posMode2char2); i++){
    encodedGNS[x] = posMode2char2[i]; x++;
}
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(numofSatellites2); i++){
    encodedGNS[x] = numofSatellites2[i]; x++;
}
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(HDOP2); i++){
    encodedGNS[x] = HDOP2[i]; x++;
}
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(altmeters2); i++){
    encodedGNS[x] = altmeters2[i]; x++;
}
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(geoidSeperation2); i++){
    encodedGNS[x] = geoidSeperation2[i]; x++;
}
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(diffAge2); i++){
    encodedGNS[x] = diffAge2[i]; x++;
}
  encodedGNS[x] = ','; x++;
for (i=0; i<sizeof(diffStationID2); i++){
    encodedGNS[x] = diffStationID2[i]; x++;
}
for (i=0; i<sizeof(navStatus2)-1; i++){
    encodedGNS[x] = navStatus2[i]; x++;
}
//for (i=0; i<sizeof(checksum2); i++){
//    encodedGNS[x] = checksum2[i]; x++;
//}


for (i=0; i<sizeof(encodedGNS); i++){
    printf("%c", encodedGNS[i]);
}
//printf("\n");
*/
}
 
void findStream2(char stream[]){
    int i=0;
    end2 = stream + 101;
    if(stream[3] == 'G' && stream[4] == 'G' && stream[5] == 'A'){
        //printf("\nSentence found - GGA\n");
        //printf("\n\nNmea sentence to decode = ");
        for(i=0; i<84; i++){
            //printf("%c", stream[i]);
        }
      parseGGA2(stream);
    }

    else if(stream[3] == 'G' && stream[4] == 'N' && stream[5] == 'S'){
        //printf("\nSentence found - GNS\n");
        //printf("\nNmea sentence to decode = ");
        for(i=0; i<84; i++){
            //printf("%c", stream[i]);
        }
        //printf("\n");
      memset(encodedGNS, 0, sizeof(encodedGNS));
      strcpy(encodedGNS, stream);
      parseGNS2(stream);
    }

    else if(stream[3] == 'G' && stream[4] == 'S' && stream[5] == 'T'){
        //printf("\nSentence found - GST\n");
        //printf("\nNmea sentence to decode = ");
        for(i=0; i<84; i++){
            //printf("%c", stream[i]);
        }
        //printf("\n");
      parseGST2(stream);
    }

    else{
        //printf("\n\nNo Stream Found");
    }
}

