#include "includes.cpp"

class Texto {
    public:
    string conteudo_texto;
    Cor cor;

    Texto(string conteudo, Cor c){
        conteudo_texto = conteudo;
        cor.set(c);
    }

    void set_texto(string content){
        conteudo_texto = content;
    }

    void render(float x, float y){
        glPushMatrix();
        glTranslatef(x-20.5, y+18, 0);
        glColor3f(cor.r, cor.g, cor.b);
        glRasterPos2f(1,1);
        for (int i = 0; i < sizeof conteudo_texto; ++i){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, conteudo_texto[i]);
        }
        glPopMatrix();
    }
};
