#include "includes.cpp"
#include "Coordenada.cpp"



class Mira {
    public:
    Coordenada pos;

    void atualiza(int x, int y){
        pos.y = (-y + 350.0f) / 35;
        pos.x = ( x - 350.0f) / 35;
    }

    void render(float x, float y){
        glPushMatrix();
            glTranslatef(pos.x + x, pos.y + y , 0);
            glBegin(GL_LINE_LOOP);
                glColor4f(1, 1, 0, 1);
                for(int i = 0; i < 30; i++){
                    float theta = 2.0f * 3.1415926f * float(i) / float(30);
                    float cx = 0.5 * cosf(theta);
                    float cy = 0.5 * sinf(theta);
                    glVertex2f(pos.x + cx, pos.y + cy);
                }
            glEnd();

            glBegin(GL_LINE_LOOP);
                glColor4f(1, 1, 0, 1);
                for(int i = 0; i < 30; i++){
                    float theta = 2.0f * 3.1415926f * float(i) / float(30);
                    float cx = 0.1 * cosf(theta);
                    float cy = 0.1 * sinf(theta);
                    glVertex2f(pos.x + cx, pos.y + cy);
                }
            glEnd();
        glPopMatrix();
    }
};