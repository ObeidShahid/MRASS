/*
#ifndef updatedNmeaFunctions_h
#define updatedNmeaFunctions_h

enum NmeaType{
	GGA,
	GST,
	RMC,
	NOSTREAM
};

struct GGA_data {
	char header[7];
	char time[10];
	double latitude;
	char latDir[2];
	double longitude;
	char longDir[2];
	int qualityOfFix;
	int numOfSats;
	double HDOP;
	double altitude;
	char altitudeUnit[2];
	double geoidSeperation;
	char geoidSeperationUnit[2];
	double diffage;
	char diffStationID[5];
	char checksum[4];
	double x;
	double y;
	double z;
};

void parseGGA(char *stream, struct GGA_data *out);

#endif
*/
