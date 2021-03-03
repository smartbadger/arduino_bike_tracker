class Location {
    float _latMov;
    float _lonMov;
    float _latArd;
    float _lonArd;
    public: 
        Location(float latM = 0 , float longM = 0, float latArd = 0, float longArd = 0){
            _latMov = latM;
            _lonMov = longM;
            _latArd = latArd;
            _lonArd = longArd;
        }
};