#ifndef calculations_h
#define calculations_h

extern long double latitudedegreescombined, longitudedegreescombined, altitudemeterscombined;

void getCartesian(double lat, double lng, double altitude);
void storeCartesian(void);
void combinePositions(long double lat, long double lon, long double alt, long double lat2, long double lon2, long double alt2, long double lat3, long double lon3, long double alt3 );

void cartesian1(long double lat, long double lon, long double alt);
void cartesian2(long double lat, long double lon, long double alt);
void cartesian3(long double lat, long double lon, long double alt);

#endif
