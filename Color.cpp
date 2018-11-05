class Color {
    public:
    float r, g, b, a;
    Color(): r(0.0f), g(0.0f), b(0.0f), a(0.0f){ }
    Color(float red ,float green ,float blue, float alpha): r(red), g(green), b(blue), a(alpha){ }

    void set(Color c){
        a = c.a;
        r = c.r;
        g = c.g;
        b = c.b;      
    }
};
