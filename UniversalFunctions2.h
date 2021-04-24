
#ifndef UniversalFunctions2_h
#define UniversalFunctions2_h

    extern long double latitudedegrees2, altitudemeters2, longitudedegrees2, N2; //cartesianX2, cartesianY2, cartesianZ2,
    extern long double latitudeminutes2, longitudeminutes2, latituderadians2, longituderadians2;

    void findStream2(char stream[]);
    //void storeCartesian2(void);

  void parseGGA2(char GGA[]);
  void parseGST2(char GST[]);
  void parseGNS2(char GNS[]);

    void encodeGNS(double lat, double lon, double alt);


    void extractTime2(void);
    void skipName2(void);
    void extractLatitude2(void);
    void extractlatDirection2(void);
    void extractlongDirection2(void);
    void extractLongitude2(void);
    void extractLongitude2(void);
    void extractqualityofFix2(void);
    void extractnumofSatellites2(void);
    void extractHDOP2(void);
    void extractAltitude2(void);
    void extractaltitudeUnit2(void);
    void extractgeoidSeperation2(void);
    void extractgeoidsepUnit2(void);
  void extractdiffcorrectionAge2(void);
  void extractdiffStationIDandChecksum2(void);
    void extractPosMode2char2(void);
    void extractnavStatus2(void);

#endif

