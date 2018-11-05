#include "includes.cpp"

class Cor {
    public:
    float r, g, b, a;
    Cor(): r(0.0f), g(0.0f), b(0.0f), a(0.0f){ }
    Cor(float red ,float green ,float blue, float alpha): r(red), g(green), b(blue), a(alpha){ }

    void set(Cor c){
        a = c.a;
        r = c.r;
        g = c.g;
        b = c.b;      
    }
};
