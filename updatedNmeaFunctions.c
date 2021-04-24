/*
#include "updatedNmeaFunctions.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
	
enum NmeaType getStreamType(char *stream) {
	if (strncmp(stream+3, "GGA", 3) == 0) {
	    return GGA;
	}
	
	return NOSTREAM;
}

double extractLatitude(char *stream){
	if (!stream) {
		return 0;
	}
	
	char latdeg[3];
	
	latdeg[0] = stream[0];
	latdeg[1] = stream[1];
	latdeg[2] = '\0';
	
	double latmins = strtod(stream + 2, NULL);
	
	return strtod(latdeg, NULL) + latmins / 60;
	
//	//printf("\n");
//	int i=0;
//	double latitudedegrees;
//	if (*stream != ','){
//	  //printf("\nPointer extractLatitude %c", *stream);
//		latdegrees[0] = *stream; stream++;
//	  //printf("\nPointer extractLatitude %c", *stream);
//		latdegrees[1] = *stream; stream++;
//		
//		latitudedegrees = strtod(latdegrees, NULL);

//		while (*stream != ','){
//			latminutes[i] = *stream; i++;
//			//printf("\nPointer extractLatitude %c", *stream);
//			stream++;
//		}
//		
//		latitudeminutes = strtod(latminutes, NULL);
//		//printf("\nLatitude minutes in convertWholeNumber = %.8Lf", latitudeminutes); 
//		latitudeminutes = latitudeminutes / 60;
//		//printf("\nLatitude minutes / 60 = %.8Lf", latitudeminutes);
//	  latitudedegrees = latitudedegrees + latitudeminutes;
//	  //printf("\nLatitude degrees final number = %.8Lf", latitudedegrees);
//		
//	}
//	  //printf("\nPointer extractLatitude %c", *stream);
//    stream++;
//	
//    return latitudedegrees;	
}

void extractlatDirection(char *stream, double *lat, char *latDir){
	if (stream && *stream == 'S') {
		*lat *= -1;
		memcpy(latDir, stream, 1);
	}
	if (stream && *stream == 'N') {
		memcpy(latDir, stream, 1);
	}
	
//	//printf("\n");
//	int i=0;
//	while (*stream != ','){
//		//printf("\nPointer extractlatDirection %c", *stream);
//		latDirection[i] = *stream; i++;
//	if(*stream == 'S'){
//		latitudedegrees = latitudedegrees * -1;
//	  //printf("\nLatitude degrees final number with south = %.8Lf", latitudedegrees);
//  }
//	stream++;
//	}
//	//printf("\nPointer extractlatDirection %c", *stream);
//	stream++;
//	
//	return latDirection[0];
}

double extractLongitude(char *stream){
		if (!stream) {
		return 0;
	}
	
	char lngdeg[4];
	
	lngdeg[0] = stream[0];
	lngdeg[1] = stream[1];
	lngdeg[2] = stream[2];
	lngdeg[3] = '\0';
	
	double lngmins = strtod(stream + 3, NULL);
	
	return strtod(lngdeg, NULL) + lngmins / 60;
	
//	//printf("\n");
//	int i=0;
//	if (*stream != ','){
//	  //printf("\nPointer extractLongitude %c", *stream);
//		longdegrees[0] = *stream; stream++;
//	  //printf("\nPointer extractLongitude %c", *stream);
//		longdegrees[1] = *stream; stream++;
//	  //printf("\nPointer extractLongitude %c", *stream);
//		longdegrees[2] = *stream; stream++;
//		
//		longitudedegrees = strtod(longdegrees, NULL);

//		while (*stream != ','){
//			longminutes[i] = *stream; i++;
//			//printf("\nPointer extractLongitude %c", *stream);
//			stream++;
//		}
//		
//		longitudeminutes = strtod(longminutes, NULL);
//		//printf("\nLongitude minutes after convertWholeNumber = %.8Lf", longitudeminutes);
//		longitudeminutes = longitudeminutes / 60.0;
//		//printf("\nLongitude minutes after /60 = %.8Lf", longitudeminutes);
//	  longitudedegrees = longitudedegrees + longitudeminutes;
//	  //printf("\nLongitude degrees final number = %.8Lf", longitudedegrees);
//		
//	}
//	  //printf("\nPointer extractLongitude %c", *stream);
//    stream++;	
}

void extractlongDirection(char *stream, double *lng, char *longDir){
	if (stream && *stream == 'W') {
		*lng *= -1;
		memcpy(longDir, stream, 1);
	}
	if (stream && *stream == 'E') {
		memcpy(longDir, stream, 1);
	}
//	//printf("\n");
//	int i=0;
//	while (*stream != ','){
//		//printf("\nPointer extractlongDirection %c", *stream);
//		longDirection[i] = *stream; i++;
//	if(*stream == 'W'){
//		longitudedegrees = longitudedegrees * -1;
//	  //printf("\nLongitude degrees final number with south = %.8Lf", longitudedegrees);
//  }
//	stream++;
//	}
//	//printf("\nPointer extractlongDirection %c", *stream);
//	stream++;
}

int extractqualityofFix(char *stream){
	if (stream) {
		return atoi(stream);
	}
	return 0;
//	//printf("\n");
//	int i=0;
//	while (*stream != ','){
//		//printf("\nPointer extractqualityofFix %c", *stream);
//		qualityofFix[i] = *stream;  i++;
//		if (*stream == '1'){
//			printf("\nQuality of fix = %c = Standard GPS (2D/3D)", *stream);
//		}
//		if (*stream == '2'){
//			printf("\nQuality of fix = %c = Differential GPS", *stream);
//		}
//		if (*stream == '0'){
//			printf("\nQuality of fix = %c = No Fix", *stream);
//		}
//		if (*stream == '4'){
//			printf("\nQuality of fix = %c = RTK Fixed", *stream);
//		}
//		if (*stream == '5'){
//			printf("\nQuality of fix = %c = RTK Float", *stream);
//		}
//		if (*stream == '6'){
//			printf("\nQuality of fix = %c = Estimated (DR) Fix", *stream);
//		}		
//		stream++;
//	}
//	//printf("\nPointer extractqualityofFix %c", *stream);
//	stream++;
}

int extractnumofSatellites(char *stream){
		if (stream) {
		return atoi(stream);
	}
	return 0;
//	//printf("\n");
//	int i=0;
//	while (*stream != ','){
//		//printf("\nPointer extractnumofSatellites %c", *stream);
//		numofSatellites[i] = *stream; i++;
//		stream++;
//	}
//	//printf("\nPointer extractnumofSatellites %c", *stream);
//	stream++;	
}

double extractHDOP(char *stream){
	if (stream) {
		return strtod(stream, NULL);
	}
	return 0;
	
//	//printf("\n");
//	int i=0;
//	while (*stream != ','){
//		//printf("\nPointer extractHDOP %c", *stream);
//		HDOP[i] = *stream; i++;
//		stream++;
//	}
//	//printf("\nPointer extractHDOP %c", *stream);
//	stream++;
	
}

double extractAltitude(char *stream){
	if (stream) {
		return strtod(stream, NULL);
	}
	return 0;
//	//printf("\n");
//	int i=0;
//	while (*stream != ','){
//		//printf("\nPointer extractAltitude %c", *stream);
//		altmeters[i] = *stream; i++;
//		stream++;
//	}
//	altitudemeters = strtod(altmeters, NULL);
//	//printf("\nPointer extractAltitude %c", *stream);
//	stream++;	
}

void extractaltitudeUnit(char *stream, char *altitudeUnit){
	if (stream) {
		memcpy(altitudeUnit, stream, 1);
	}
	 
//	//printf("\n");
//	int i=0;
//	while (*stream != ','){
//		//printf("\nPointer extractaltitudeUnit %c", *stream);
//		altUnits[i] = *stream;  i++;
//		stream++;
//	}
//	//printf("\nPointer extractaltitudeUnit %c", *stream);
//	stream++;	
}

double extractgeoidSeperation(char *stream){
	if (stream) {
		return strtod(stream, NULL);
	}
	return 0;
//	//printf("\n");
//	int i=0;
//	while (*stream != ','){
//		//printf("\nPointer extractgeoidSeperation %c", *stream);
//		geoidSeperation[i] = *stream;  i++;
//		stream++;
//	}
//	//printf("\nPointer extractgeoidSeperation %c", *stream);
//	stream++;	
}

void	extractgeoidsepUnit(char *stream, char *geoidsepUnit){
	if (stream) {
		memcpy(geoidsepUnit, stream, 1);
	}
	
	//printf("\n");
//	int i=0;
//	while (*stream != ','){
//		//printf("\nPointer extractgeoidsepUnit %c", *stream);
//		geoidUnits[i] = *stream;  i++;
//		stream++;
//	}
//	//printf("\nPointer extractgeoidsepUnit %c", *stream);
//	stream++;	
	
}

double extractdiffcorrectionAge(char *stream){
	if (stream) {
		return strtod(stream, NULL);
	}
	return 0;
	
//	//printf("\n");
//	int i=0;
//	while (*stream != ','){
//		//printf("\nPointer extractdiffcorrectionAge %c", *stream);
//		diffAge[i] = *stream;  i++;
//		stream++;
//	}
//	//printf("\nPointer extractdiffcorrectionAge %c", *stream);
//	stream++;
	
}

void extractdiffStationID(char *stream, char *diffStation){
	if (stream) {
		memcpy(diffStation, stream, 4);
	}
	
//	//printf("\n");
//	int i=0;
//	while (*stream != '*'){
//		//printf("\nPointer extractdiffcorrectionStationID %c", *stream);
//		diffStationID[i] = *stream; i++;
//		stream++;
//	}
////	if (*stream == '*'){
////		//printf("\nPointer extractChecksum %c", *stream);
////		checksum[0] = *stream; stream++;
////		//printf("\nPointer extractChecksum %c", *stream);
////		checksum[1] = *stream; stream++;
////		//printf("\nPointer extractChecksum %c", *stream);
////		checksum[2] = *stream; stream++;
////	}
//	//printf("\nPointer extractdiffcorrectionStationID %c", *stream);
//	stream++;
	
}

//void skipName(char *stream){
//	int i=0;
//	while (*stream != ','){
//	  //printf("\nPointer skipName %c", *stream);
//		messageID[i] = *stream; i++;
//		stream++;		
//	}
//	  //printf("\nPointer skipName %c", *stream);
//    stream++;	
//}

void extractHeader(char *stream, char *header){
	if (stream) {
		strncpy(header, stream, 6);
		header[6] = '\0';
	}
}

void extractTime(char *stream, char *time){
	if (stream){
		memcpy(time, stream, 9);
	}
	
//	//printf("\n");
//	if (*stream != ','){
//	  //printf("\nPointer extractTime %c", *stream);
//		timehours[0] = *stream; stream++;
//	  //printf("\nPointer extractTime %c", *stream);
//		timehours[1] = *stream; stream++;
//	  //printf("\nPointer extractTime %c", *stream);
//		timemins[0] = *stream; stream++;
//	  //printf("\nPointer extractTime %c", *stream);
//		timemins[1] = *stream; stream++;
//	  //printf("\nPointer extractTime %c", *stream);
//		timesecs[0] = *stream; stream++;
//	  //printf("\nPointer extractTime %c", *stream);
//		timesecs[1] = *stream; stream++;
//	  //printf("\nPointer extractTime %c", *stream);
//		timesecs[2] = *stream; stream++;
//	  //printf("\nPointer extractTime %c", *stream);
//		timesecs[3] = *stream; stream++;
//	  //printf("\nPointer extractTime %c", *stream);
//	  timesecs[4] = *stream; stream++;
//	}	
//	  //printf("\nPointer extractTime %c", *stream);
//	  stream++;

}

void parseGGA(char *stream, struct GGA_data *out) {
//	memcpy(out->header, stream, 5);
//	out->header[5] = '\0';
	char *scanner = stream;
	extractHeader(scanner, out->header);
	scanner = strchr(scanner++, ',') + 1;
	extractTime(scanner, out->time);
	scanner = strchr(scanner++, ',') + 1;
	out->latitude = extractLatitude(scanner);
	scanner = strchr(scanner++, ',') + 1;
	extractlatDirection(scanner, &out->latitude, out->latDir);
	scanner = strchr(scanner++, ',') + 1;
	out->longitude = extractLongitude(scanner);
	scanner = strchr(scanner++, ',') + 1;
	extractlongDirection(scanner, &out->longitude, out->longDir);
	scanner = strchr(scanner++, ',') + 1;
	out->qualityOfFix = extractqualityofFix(scanner);
	scanner = strchr(scanner++, ',') + 1;
	out->numOfSats = extractnumofSatellites(scanner);
	scanner = strchr(scanner++, ',') + 1;
	out->HDOP = extractHDOP(scanner);
	scanner = strchr(scanner++, ',') + 1;
	out->altitude = extractAltitude(scanner);
	scanner = strchr(scanner++, ',') + 1;
	extractaltitudeUnit(scanner, out->altitudeUnit);
	scanner = strchr(scanner++, ',') + 1;
	out->geoidSeperation = extractgeoidSeperation(scanner);
	scanner = strchr(scanner++, ',') + 1;
	extractgeoidsepUnit(scanner, out->geoidSeperationUnit);
	scanner = strchr(scanner++, ',') + 1;
	out->diffage = extractdiffcorrectionAge(scanner);
	scanner = strchr(scanner++, ',') + 1;
	extractdiffStationID(scanner, out->diffStationID);
	scanner = strchr(scanner++, '*');
	memcpy(out->checksum, scanner, 3);
	out->checksum[3] = '\0';
}
*/
