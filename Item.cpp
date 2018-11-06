class Item{
    public:
    string name;
    bool shoot;
    int num, damage, cap, reload_time, accuracy;
    int rate;
    Item(){}
    Item(bool _shoot, string _name, int _damage, int _cap, int _num, int _rate, int _reload_time, int _accuracy){
        reload_time = _reload_time;
        accuracy    = _accuracy;
        damage      = _damage;
        shoot       = _shoot;
        name        = _name;
        rate        = _rate;
        num         = _num;
        cap         = _cap;
    }
};
