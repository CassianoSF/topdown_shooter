
class Arma{
    public:
    string name;
    bool shoot;
    int num, damage, cap, reload_time, accuracy;
    int rate;
    Arma(){}
    Arma(bool _shoot, string _name, int _damage, int _cap, int _num, int _rate, int _reload_time, int _accuracy){
        shoot     =     _shoot;
        name     =     _name;
        num      =      _num;
        damage   =   _damage;
        rate     =     _rate;
        reload_time   =   _reload_time;
        cap      =      _cap;
        accuracy = _accuracy;
    }
};
