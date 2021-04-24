#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "TExaS.h"
#include "uart.h"
#include "PLL.h"
#include "UniversalFunctions.h"

#define a 6378137.0 //earth equatorial radius (wgs84 standard)
#define flattening 0.00335281 //flattening factor of ellipsoid (f = 1/298.257223563)
#define b 6356752.3142  //semi-minor axis (b = a * (1-f)
#define ecc 0.08181918275 //eccentricity (e^2 = 2f - f^2)
#define Pi 3.14159265358979323846 

    long double latitudedegrees, altitudemeters, longitudedegrees, N;
    long double latitudeminutes, longitudeminutes, latituderadians, longituderadians;
    char latdegrees[2], longdegrees[3], latminutes[10], longminutes[10], altmeters[6], rangeRMS[10], stdMajor[3], stdMinor[3], orient[3], stdLat[3], stdLong[3], stdAlt[3];
    char timehours[2], timemins[2], timesecs[5], messageIDGGA[6], messageIDGNS[6], messageIDGST[6], checksum[3];
    char latDirection[1], navStatus[1], posMode2char[2], longDirection[1], qualityofFix[1], numofSatellites[2], HDOP[4], altUnits[1], geoidSeperation[4], geoidUnits[1], diffAge[4], diffStationID[4];
    char encodedYPR[100], YPRtalker[6] = {'$','G','N','Y','P','R'};
    char *scanner; *end;
  
void extractLatitude(void){
    //printf("\n");
    int i=0;
    if (*scanner != ',' && scanner < end){
      //printf("\nPointer extractLatitude %c", *scanner);
        latdegrees[0] = *scanner; scanner++;
      //printf("\nPointer extractLatitude %c", *scanner);
        latdegrees[1] = *scanner; scanner++;
    }

    //printf("\nlatdegrees = ");
    for (i=0; i<sizeof(latdegrees); i++){
        //printf("%c", latdegrees[i]);
    }

    i=0;

        latitudedegrees = strtod(latdegrees, NULL);
        //printf("\nLatitude degrees in strtod = %Lf", latitudedegrees);
        while (*scanner != ',' && scanner < end){
            latminutes[i] = *scanner; i++;
            //printf("\nPointer extractLatitude %c", *scanner);
            scanner++;
        }

        //printf("\nlatitudeminutes1array = ");
        for (i=0; i<sizeof(latminutes); i++){
            //printf("%c", latminutes[i]);
        }

        latitudeminutes = strtod(latminutes, NULL);
        //printf("\nLatitude minutes in strtod = %Lf", latitudeminutes);
        latitudeminutes = latitudeminutes / 60;
        //printf("\nLatitude minutes / 60 = %Lf", latitudeminutes);
        latitudedegrees = latitudedegrees + latitudeminutes;
        //printf("\nLatitude degrees = %Lf", latitudedegrees);

      //printf("\nPointer extractLatitude %c", *scanner);
    scanner++;
        i=0;
}

void extractlatDirection(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractlatDirection %c", *scanner);
        latDirection[i] = *scanner; i++;
    if(*scanner == 'S'){
        latitudedegrees = latitudedegrees * -1;
      //printf("\nLatitude degrees final number with south = %Lf", latitudedegrees);
  }
    scanner++;
    }
    //printf("\nPointer extractlatDirection %c", *scanner);
    scanner++;
    i=0;
    //printf("\nLatDirection = %c", latDirection[0]);
}

void extractLongitude(void){
    //printf("\n");
    int i=0;
    if (*scanner != ',' && scanner < end){
      //printf("\nPointer extractLongitude %c", *scanner);
        longdegrees[0] = *scanner; scanner++;
      //printf("\nPointer extractLongitude %c", *scanner);
        longdegrees[1] = *scanner; scanner++;
      //printf("\nPointer extractLongitude %c", *scanner);
        longdegrees[2] = *scanner; scanner++;
    }
        longitudedegrees = strtod(longdegrees, NULL);

        while (*scanner != ',' && scanner < end){
            longminutes[i] = *scanner; i++;
            //printf("\nPointer extractLongitude %c", *scanner);
            scanner++;
        }

        longitudeminutes = strtod(longminutes, NULL);
        //printf("\nLongitude minutes after strtod = %Lf", longitudeminutes);
        longitudeminutes = longitudeminutes / 60.0;
        //printf("\nLongitude minutes after /60 = %Lf", longitudeminutes);
        longitudedegrees = longitudedegrees + longitudeminutes;
        //printf("\nLongitude degrees = %Lf", longitudedegrees);

      //printf("\nPointer extractLongitude %c", *scanner);
    scanner++;
    i=0;
}

void extractlongDirection(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractlongDirection %c", *scanner);
        longDirection[i] = *scanner; i++;
    if(longDirection[0] == 'W'){
        longitudedegrees = longitudedegrees * -1;
      //printf("\nLongitude degrees final number with south = %Lf", longitudedegrees);
  }
    scanner++;
    }
    //printf("\nPointer extractlongDirection %c", *scanner);
    scanner++;
    i=0;
    //printf("\nLongDirection = %c", longDirection[0]);
}

void extractqualityofFix(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractqualityofFix %c", *scanner);
        qualityofFix[i] = *scanner;  i++;
        scanner++;
    }

    //printf("\nQuality of Fix = ");
    //for(i=0; i<sizeof(qualityofFix); i++){
        //printf("%c", qualityofFix[i]);
    //}

    if (qualityofFix[0] == '1'){
            //printf("\nQuality of fix = %c = Standard GPS (2D/3D)", qualityofFix[0]);
        }
    if (qualityofFix[0] == '2'){
            //printf("\nQuality of fix = %c = Differential GPS", qualityofFix[0]);
        }
    if (qualityofFix[0] == '0'){
            //printf("\nQuality of fix = %c = No Fix", qualityofFix[0]);
        }
    if (qualityofFix[0] == '4'){
            //printf("\nQuality of fix = %c = RTK Fixed", qualityofFix[0]);
        }
    if (qualityofFix[0] == '5'){
            //printf("\nQuality of fix = %c = RTK Float", qualityofFix[0]);
        }
    if (qualityofFix[0] == '6'){
            //printf("\nQuality of fix = %c = Estimated (DR) Fix", qualityofFix[0]);
        }
    //printf("\nPointer extractqualityofFix %c", *scanner);
    scanner++;
    i=0;
}

void extractnumofSatellites(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractnumofSatellites %c", *scanner);
        numofSatellites[i] = *scanner; i++;
        scanner++;
    }
    //printf("\nPointer extractnumofSatellites %c", *scanner);
    scanner++;
    i=0;
//printf("\nNum of satellites = ");
for(i=0; i<sizeof(numofSatellites); i++){
        //printf("%c", numofSatellites[i]);
}
}

void extractHDOP(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractHDOP %c", *scanner);
        HDOP[i] = *scanner; i++;
        scanner++;
    }
    //printf("\nPointer extractHDOP %c", *scanner);
    scanner++;
    i=0;
    //printf("\nHDOP = ");
    for (i=0; i<sizeof(HDOP); i++){
        //printf("%c", HDOP[i]);
    }
}

void extractAltitude(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractAltitude %c", *scanner);
        altmeters[i] = *scanner; i++;
        scanner++;
    }
    altitudemeters = strtod(altmeters, NULL);
    //printf("\nAltitude meters = %.1Lf", altitudemeters);
    //printf("\nPointer extractAltitude %c", *scanner);
    scanner++;
    i=0;
    //printf("\nAltitude = ");
    for (i=0; i<sizeof(altmeters); i++){
        //printf("%c", altmeters[i]);
    }
}

void extractaltitudeUnit(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractaltitudeUnit %c", *scanner);
        altUnits[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractaltitudeUnit %c", *scanner);
    scanner++;
    i=0;
    //printf("\nAltitude Unit = ");
    for (i=0; i<sizeof(altUnits); i++){
        //printf("%c", altUnits[i]);
    }
}

void extractgeoidSeperation(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractgeoidSeperation %c", *scanner);
        geoidSeperation[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractgeoidSeperation %c", *scanner);
    scanner++;
    i=0;
    //printf("\nGeoid Seperation = ");
    for (i=0; i<sizeof(geoidSeperation); i++){
        //printf("%c", geoidSeperation[i]);
    }
}

void extractgeoidsepUnit(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractgeoidsepUnit %c", *scanner);
        geoidUnits[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractgeoidsepUnit %c", *scanner);
    scanner++;
    i=0;
    //printf("\nGeoid Seperation Unit = ");
    for (i=0; i<sizeof(geoidUnits); i++){
        //printf("%c", geoidUnits[i]);
    }
}

void extractdiffcorrectionAge(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractdiffcorrectionAge %c", *scanner);
        diffAge[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractdiffcorrectionAge %c", *scanner);
    scanner++;
    i=0;
    //printf("\nDiff Correction Age = ");
    for (i=0; i<sizeof(diffAge); i++){
        //printf("%c", diffAge[i]);
    }
}

void extractdiffStationIDandChecksum(void){
    //printf("\n");
    int i=0;
    while (*scanner != '*' && scanner < end){
        //printf("\nPointer extractdiffcorrectionStationID %c", *scanner);
        diffStationID[i] = *scanner; i++;
        scanner++;
    }
    if (*scanner == '*'){
        //printf("\nPointer extractChecksum %c", *scanner);
        checksum[0] = *scanner; scanner++;
        //printf("\nPointer extractChecksum %c", *scanner);
        checksum[1] = *scanner; scanner++;
        //printf("\nPointer extractChecksum %c", *scanner);
        checksum[2] = *scanner; scanner++;
    }
    //printf("\nPointer extractdiffcorrectionStationID %c", *scanner);
    scanner++;
    i=0;
    //printf("\nDiff Station ID = ");
    for (i=0; i<sizeof(diffStationID); i++){
    //  //printf("%c", diffStationID[i]);
    }
    //printf("\nChecksum = ");
    for (i=0; i<sizeof(checksum); i++){
        //printf("%c", checksum[i]);
    }
}

void extractdiffStationID(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractdiffcorrectionStationID %c", *scanner);
        diffStationID[i] = *scanner; i++;
        scanner++;
    }
    //printf("\nPointer extractdiffcorrectionStationID %c", *scanner);
    scanner++;
    i=0;
    //printf("\nDiff Station ID = ");
    for (i=0; i<sizeof(diffStationID); i++){
        //printf("%c", diffStationID[i]);
    }
}

void skipNameGST(void){
    int i=0;
    while (*scanner != ',' && scanner < end){
      //printf("\nPointer skipName %c", *scanner);
        messageIDGST[i] = *scanner; i++;
        scanner++;
    }
      //printf("\nPointer skipName %c", *scanner);
  scanner++;
    i=0;
    //printf("\nMessage ID = ");
    for (i=0; i<sizeof(messageIDGST); i++){
        //printf("%c", messageIDGST[i]);
    }
}

void skipNameGNS(void){
    int i=0;
    while (*scanner != ',' && scanner < end){
      //printf("\nPointer skipName %c", *scanner);
        messageIDGNS[i] = *scanner; i++;
        scanner++;
    }
      //printf("\nPointer skipName %c", *scanner);
  scanner++;
    i=0;
    //printf("\nMessage ID = ");
    for (i=0; i<sizeof(messageIDGNS); i++){
        //printf("%c", messageIDGNS[i]);
    }
}

void skipNameGGA(void){
    int i=0;
    while (*scanner != ',' && scanner < end){
      //printf("\nPointer skipName %c", *scanner);
        messageIDGGA[i] = *scanner; i++;
        scanner++;
    }
      //printf("\nPointer skipName %c", *scanner);
  scanner++;
    i=0;
    //printf("\nMessage ID = ");
    for (i=0; i<sizeof(messageIDGGA); i++){
        //printf("%c", messageIDGGA[i]);
    }
}

void Skip(void){
    int i=0;
    while (*scanner != ',' && scanner < end){
      //printf("\nPointer Skip %c", *scanner);
        scanner++;
    }
  //printf("\nPointer Skip %c", *scanner);
  scanner++;
}

void extractTime(void){
    //printf("\n");
    if (*scanner != ',' && scanner < end){
      //printf("\nPointer extractTime %c", *scanner);
        timehours[0] = *scanner; scanner++;
      //printf("\nPointer extractTime %c", *scanner);
        timehours[1] = *scanner; scanner++;
      //printf("\nPointer extractTime %c", *scanner);
        timemins[0] = *scanner; scanner++;
      //printf("\nPointer extractTime %c", *scanner);
        timemins[1] = *scanner; scanner++;
      //printf("\nPointer extractTime %c", *scanner);
        timesecs[0] = *scanner; scanner++;
      //printf("\nPointer extractTime %c", *scanner);
        timesecs[1] = *scanner; scanner++;
      //printf("\nPointer extractTime %c", *scanner);
        timesecs[2] = *scanner; scanner++;
      //printf("\nPointer extractTime %c", *scanner);
        timesecs[3] = *scanner; scanner++;
      //printf("\nPointer extractTime %c", *scanner);
      timesecs[4] = *scanner; scanner++;
    }

        //printf("\nTime = %c%c:%c%c:%c%c%c%c%c", timehours[0],timehours[1],timemins[0],timemins[1],timesecs[0], timesecs[1],timesecs[2],timesecs[3],timesecs[4] );
      //printf("\nPointer extractTime %c", *scanner);
      scanner++;
}
 
/*void storeCartesian(){
  //printf("\n");
    //printf("\nLatitude degrees in storeCartesian = %Lf", latitudedegrees);
    //printf("\nLongitude degrees in storeCartesian = %Lf", longitudedegrees);
    //printf("\nAltitude meters in storeCartesian = %Lf", altitudemeters);

    latituderadians = latitudedegrees * (Pi/180);
    longituderadians = longitudedegrees * (Pi/180);

//  //printf("\n\nLatitude radians in storeCartesian = %Lf", latituderadians);
//  //printf("\n\nLongitude radians in storeCartesian = %Lf", longituderadians);

    N = a/sqrt(1-pow(ecc,2)*pow(sin(latituderadians),2));
//  //printf("\n\nN - Radius of curvature in prime vertical = %Lf", N);

    cartesianX = (N+altitudemeters) * cos(latituderadians) * cos(longituderadians);
    cartesianY = (N+altitudemeters) * cos(latituderadians) * sin(longituderadians);
    cartesianZ = ((1-pow(ecc,2))*(N+altitudemeters)) * sin(latituderadians);

    //printf("\n\nCartesianX = %Lf", cartesianX);
    //printf("\nCartesianY = %Lf", cartesianY);
    //printf("\nCartesianZ = %Lf", cartesianZ);

}*/
 
void extractnavStatusandChecksum(void){
    //printf("\n");
    int i=0;
    while (*scanner != '*' && scanner < end){
        //printf("\nPointer extractnavStatusandChecksum %c", *scanner);
        navStatus[i] = *scanner;  i++;
        scanner++;
    }
    if (*scanner == '*'){
        //printf("\nPointer extractChecksum %c", *scanner);
        checksum[0] = *scanner; scanner++;
        //printf("\nPointer extractChecksum %c", *scanner);
        checksum[1] = *scanner; scanner++;
        //printf("\nPointer extractChecksum %c", *scanner);
        checksum[2] = *scanner; scanner++;
    }
    //printf("\nPointer extractnavStatusandChecksum %c", *scanner);
    scanner++;
    i=0;
    //printf("\nextractnavStatus = ");
    for (i=0; i<sizeof(navStatus); i++){
        //printf("%c", navStatus[i]);
    }
    //printf("\nChecksum = ");
    for (i=0; i<sizeof(checksum); i++){
        //printf("%c", checksum[i]);
    }
}

 void extractnavStatus(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractnavStatus %c", *scanner);
        navStatus[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractnavStatus %c", *scanner);
    scanner++;
    i=0;
    //printf("\nextractnavStatus = ");
    for (i=0; i<sizeof(navStatus); i++){
        //printf("%c", navStatus[i]);
    }
}
 
void extractPosMode2char(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractPosMode2char %c", *scanner);
        posMode2char[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractPosMode2char %c", *scanner);
    scanner++;
    i=0;
    //printf("\nPosMode2char = ");
    for (i=0; i<sizeof(posMode2char); i++){
        //printf("%c", posMode2char[i]);
    }
}

void extractRangeRMS(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractRangeRMS %c", *scanner);
        rangeRMS[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractRangeRMS %c", *scanner);
    scanner++;
    i=0;
    //printf("\nRange RMS = ");
    for (i=0; i<sizeof(rangeRMS); i++){
        //printf("%c", rangeRMS[i]);
    }
}

void extractRangeRMSnotWorking(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractRangeRMS %c", *scanner);
        rangeRMS[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractRangeRMS %c", *scanner);
    scanner++;
    i=0;
    //printf("\nRange RMS = ");
    for (i=0; i<sizeof(rangeRMS); i++){
        //printf("%c", rangeRMS[i]);
    }
}

void extractstdMajor(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractstdMajor %c", *scanner);
        stdMajor[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractstdMajor %c", *scanner);
    scanner++;
    i=0;
    //printf("\nstd Major = ");
    for (i=0; i<sizeof(stdMajor); i++){
        //printf("%c", stdMajor[i]);
    }
}

void extractstdMinor(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractstdMinor %c", *scanner);
        stdMinor[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractstdMinor %c", *scanner);
    scanner++;
    i=0;
    //printf("\nstd Minor = ");
    for (i=0; i<sizeof(stdMinor); i++){
        //printf("%c", stdMinor[i]);
    }
}

void extractOrient(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractOrient %c", *scanner);
        orient[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractOrient %c", *scanner);
    scanner++;
    i=0;
    //printf("\nOrient = ");
    for (i=0; i<sizeof(orient); i++){
        //printf("%c", orient[i]);
    }
}

void extractstdLat(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractstdLat %c", *scanner);
        stdLat[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractstdLat %c", *scanner);
    scanner++;
    i=0;
    //printf("\nstd Lat = ");
    for (i=0; i<sizeof(stdLat); i++){
        //printf("%c", stdLat[i]);
    }
}

void extractstdLong(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractstdLong %c", *scanner);
        stdLong[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractstdLong %c", *scanner);
    scanner++;
    i=0;
    //printf("\nstd Long = ");
    for (i=0; i<sizeof(stdLong); i++){
        //printf("%c", stdLong[i]);
    }
}

void extractstdAlt(void){
    //printf("\n");
    int i=0;
    while (*scanner != ',' && scanner < end){
        //printf("\nPointer extractstdAlt %c", *scanner);
        stdAlt[i] = *scanner;  i++;
        scanner++;
    }
    //printf("\nPointer extractstdAlt %c", *scanner);
    scanner++;
    i=0;
    //printf("\nstd Alt = ");
    for (i=0; i<sizeof(stdAlt); i++){
        //printf("%c", stdAlt[i]);
    }
}

void extractstdAltandChecksum(void){
    //printf("\n");
    int i=0;
    while (*scanner != '*' && scanner < end){
        //printf("\nPointer extractstdAlt %c", *scanner);
        stdAlt[i] = *scanner;  i++;
        scanner++;
    }
    if (*scanner == '*'){
        //printf("\nPointer extractChecksum %c", *scanner);
        checksum[0] = *scanner; scanner++;
        //printf("\nPointer extractChecksum %c", *scanner);
        checksum[1] = *scanner; scanner++;
        //printf("\nPointer extractChecksum %c", *scanner);
        checksum[2] = *scanner; scanner++;
    }
    //printf("\nPointer extractstdAlt %c", *scanner);
    scanner++;
    i=0;
    //printf("\nstd Alt = ");
    for (i=0; i<sizeof(stdAlt); i++){
        //printf("%c", stdAlt[i]);
    }
    //printf("\nChecksum = ");
    for (i=0; i<sizeof(checksum); i++){
        //printf("%c", checksum[i]);
    }
}

void parseGGA(char GGA[]){
    scanner = GGA;
    skipNameGGA();
    extractTime();
    extractLatitude();
    extractlatDirection();
    extractLongitude();
    extractlongDirection();
    extractqualityofFix();
    extractnumofSatellites();
    extractHDOP();
    extractAltitude();
    extractaltitudeUnit();
    extractgeoidSeperation();
    extractgeoidsepUnit();
    extractdiffcorrectionAge();
    extractdiffStationIDandChecksum();
    //storeCartesian();
}

void parseGNS(char GNS[]){
    scanner = GNS;
    skipNameGNS();
    extractTime();
    extractLatitude();
    extractlatDirection();
    extractLongitude();
    extractlongDirection();
    extractPosMode2char();
    extractnumofSatellites();
    extractHDOP();
    extractAltitude();
    extractgeoidSeperation();
    //extractdiffcorrectionAge(); // Due to output error
    extractdiffStationID();
    extractnavStatusandChecksum();
}
 
void parseGST(char GST[]){
    scanner = GST;
    skipNameGST();
    extractTime();
    extractRangeRMSnotWorking();
    extractstdMajor();
    extractstdMinor();
    extractOrient();
    extractstdLat();
    extractstdLong();
    extractstdAltandChecksum();
}

int nmea_checksum(char *stream)
{
    int checksum = 0;
    int i;

    // the first $ sign and the last two bytes of original CRC + the * sign
    for (i = 1; i < strlen(stream); i ++) {
        checksum ^= stream[i];
    }

    return checksum;
}

void encodeYPR(double yaw, double pitch, double roll, double heading){
    int checkSum;
    char hexChecksum[2];

    sprintf(encodedYPR, "%s,%.7lf,%.7lf,%.7lf,%.5lf", YPRtalker,yaw,pitch,roll, heading);
    //printf("\n\nEncoded NMEA sentence = %s ", encodedYPR);

    checkSum = nmea_checksum(encodedYPR);
    sprintf(hexChecksum, "%x", checkSum);
    //printf("\ncheckSum = %s", hexChecksum);

    sprintf(encodedYPR, "%s,%.7lf,%.7lf,%.7lf,%.5lf*%s", YPRtalker,yaw,pitch,roll,heading,hexChecksum);
    //printf("%s\n", encodedYPR);
}


void findStream(char stream[],int size){
    int i;
    end = stream + 101;
    if((stream[4] == 'G' && stream[5] == 'G' && stream[6] == 'A') || (stream[3] == 'G' && stream[4] == 'G' && stream[5] == 'A')){
        //printf("\n\nSentence found - GGA");
        //printf("\nNmea sentence to decode = ");
        for(i=0; i<size; i++){
        //printf("%c", stream[i]);
        }
        //printf("\n");
      parseGGA(stream);
    }

    else if((stream[3] == 'G' && stream[4] == 'N' && stream[5] == 'S') || (stream[4] == 'G' && stream[5] == 'N' && stream[6] == 'S')){
        //printf("\n\nSentence found - GNS");
        //printf("\nNmea sentence to decode = ");
        for(i=0; i<size; i++){
        //printf("%c", stream[i]);
        }
        //printf("\n");
        parseGNS(stream);
    }

    else if((stream[3] == 'G' && stream[4] == 'S' && stream[5] == 'T') || (stream[4] == 'G' && stream[5] == 'S' && stream[6] == 'T')){
        //printf("\n\nSentence found - GST");
        //printf("\nNmea sentence to decode = ");
        for(i=0; i<size; i++){
            //printf("%c", stream[i]);
        }
        //printf("\n");
      parseGST(stream);
    }

    else{
        //printf("\n\nNo Stream Found");
    }
}
