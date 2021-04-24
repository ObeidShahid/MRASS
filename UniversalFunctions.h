#ifndef UniversalFunctions_h
#define UniversalFunctions_h

    extern long double cartesianX, cartesianY, cartesianZ, latitudedegrees, altitudemeters, longitudedegrees, N;
    extern long double latitudeminutes, longitudeminutes, latituderadians, longituderadians;

    void findStream(char stream[],int size);
    void parseGGA(char GGA[]);
    void extractTime(void);
    void skipName(void);
    void extractLatitude(void);
    void extractlatDirection(void);
    void extractlongDirection(void);
    void extractLongitude(void);
    void extractLongitude(void);
    void extractqualityofFix(void);
    void extractnumofSatellites(void);
    void extractHDOP(void);
    void extractAltitude(void);
    void extractaltitudeUnit(void);
    void extractgeoidSeperation(void);
    void extractgeoidsepUnit(void);
  void extractdiffcorrectionAge(void);
  void extractdiffStationIDandChecksum(void);

  void encodeYPR(double yaw, double pitch, double roll, double heading);



#endif
