#include "includes.cpp"



class GameObject{
    public:
    Coordenada pos;
    Textura tex;
    Cor cor;
    float altura, largura, angulo;

    GameObject(Coordenada _pos, Textura _tex, Cor _cor, float _altura, float _largura, float _angulo){
        pos = _pos;
        tex = _tex;
        cor = _cor;
        altura = _altura;
        largura = _largura;
        angulo = _angulo;
    }

    void render(){
        glPushMatrix();
            glTranslatef(pos.x, pos.y , 0);
            glRotatef(angulo-90, 0, 0, 1);
            glBegin(GL_POLYGON);
                glColor4f(cor.r, cor.g, cor.b, cor.a);
                glVertex2f( largura/2,  altura/2);
                glVertex2f( largura/2, -altura/2);
                glVertex2f(-largura/2, -altura/2);
                glVertex2f(-largura/2,  altura/2);
            glEnd();
        glPopMatrix();
    }
};