#ifndef UniversalFunctions3_h
#define UniversalFunctions3_h

    extern long double latitudedegrees3, altitudemeters3, longitudedegrees3, N3; //cartesianX3, cartesianY3, cartesianZ3,
    extern long double latitudeminutes3, longitudeminutes3, latituderadians3, longituderadians3;

    void findStream3(char stream[]);

  void parseGGA3(char GGA[]);
  void parseGST3(char GST[]);
  void parseGNS3(char GNS[]);

    void encodeGST(void);


    void extractTime3(void);
    void skipName3(void);
    void extractLatitude3(void);
    double convertWholeNumber3(char numberstring[]);
    void extractlatDirection3(void);
    void extractlongDirection3(void);
    void extractLongitude3(void);
    void extractLongitude3(void);
    void extractqualityofFix3(void);
    void extractnumofSatellites3(void);
    void extractHDOP3(void);
    void extractAltitude3(void);
    void extractaltitudeUnit3(void);
    void extractgeoidSeperation3(void);
    void extractgeoidsepUnit3(void);
  void extractdiffcorrectionAge3(void);
  void extractdiffStationIDandChecksum3(void);
    void extractPosMode2char3(void);
    void extractnavStatus3(void);

#endif
