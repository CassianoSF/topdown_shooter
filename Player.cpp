#include "includes.cpp"
#include "Cor.cpp"
#include "Coordenada.cpp"
#include "Arma.cpp"
using namespace std;


class Player {
    public:
    Coordenada pos;
    Cor cor;   
    int animacao_frames;
    int reload_left;
    int shoot_left;
    float rand_offset;
    float raio, inclinacao;
    string animacao, pernas;
    Arma arma;


    bool shoot=false, 
         idle=true, 
         walk=false, 
         run=false, 
         reload=false, 
         attack=false;

    Player(Arma _arma, float x, float y, float r, float angulo, Cor c){
        cor.set(c);
        pos = Coordenada(x,y);
        inclinacao = angulo;
        raio = r;
        arma = _arma;
    }

    void atualiza(int tempo){

        stringstream ss;

        if(attack){
            animacao_frames = 15;
            ss << arma.name << "_" << "meleeattack";
        }else if (reload && arma.reload_time && reload_left){
            reload_left--;
            animacao_frames = 15;
            ss << arma.name << "_" << "reload";
            reload = !(reload_left == 1);

        }else if (shoot && arma.shoot){
            shoot_left--;
            animacao_frames = 3;
            ss << arma.name << "_" << "shoot";
            shoot = !(shoot_left == 1);

        }else if(walk || run){
            if (walk){
                pernas = "feet_run";
            }else
                pernas = "feet_walk";

            animacao_frames = 20;
            ss << arma.name << "_" << "move";
        }else if(idle){
            ss << arma.name << "_" << "idle";
            animacao_frames = 20;
        }
        animacao = ss.str();
    }

    void caminha(int* keyStates){
        float speed_mult = 0.01;
        if (run)
            speed_mult = 0.017;
        float velocidade;
        int move = (keyStates['w'] + keyStates['s'] + keyStates['a'] + keyStates['d'] +
                    keyStates['W'] + keyStates['S'] + keyStates['A'] + keyStates['D']);
        if(move){
            walk = true;
        }else{
            walk = false;
        }
        if (move > 1){
            velocidade = 0.7*speed_mult;
        }else{
            velocidade = speed_mult;
        }
        if(keyStates['w'] || keyStates['W'] ){ pos.y += velocidade; }
        if(keyStates['s'] || keyStates['S'] ){ pos.y -= velocidade; }
        if(keyStates['a'] || keyStates['A'] ){ pos.x -= velocidade; }
        if(keyStates['d'] || keyStates['D'] ){ pos.x += velocidade; }
    }

    void rotate(int x, int y){
        y = (-y + 350) * 35;
        x = ( x - 350) * 35;
        if(x != 0 && y !=0){
            inclinacao = (-(GLfloat)atan2(x-pos.x,y-pos.y)/3.1415*180.0)+90;
        } 
    }

    void renderTiro(Textura texturas[421], int tempo, int frame_time){
        if (shoot && arma.shoot && !reload && (tempo / frame_time) % arma.rate == 1){
            glPushMatrix();
                for (int i = 0; i < 421; ++i){
                    if(texturas[i].animacao == "shoot_texture")
                        texturas[i].render();
                }
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0, 0.0);
                    glVertex2f(  1.3,-3.1);
                    glTexCoord2f(0.0, 1.0);
                    glVertex2f(  1.3, 0.9);
                    glTexCoord2f(1.0, 1.0);
                    glVertex2f(  5.3, 0.9);
                    glTexCoord2f(1.0, 0.0);
                    glVertex2f(  5.3,-3.1);
                glEnd();
                glDisable(GL_TEXTURE_2D);
                glRotatef(rand_offset-86, 0, 0, 1);
                glBegin(GL_POLYGON);
                    glColor4f(1, 1, 0, 1);
                    glVertex2f( +1.03, -0.0);
                    glVertex2f( +1.03, +100);
                    glVertex2f( +1.08, +100);
                    glVertex2f( +1.08, -0.0);
                glEnd();
            glPopMatrix();
        }
    }

    void render(Textura texturas[421], int tempo, int frame_time){
        glPushMatrix();
            glTranslatef(pos.x, pos.y, 0);
            glRotatef(inclinacao, 0, 0, 1);
            renderTiro(texturas, tempo, frame_time);
            Textura texture;
            for (int i = 0; i < 421; ++i){
                if(texturas[i].animacao == animacao && texturas[i].index == (tempo / frame_time) % animacao_frames)
                    texturas[i].render();
            }
            // glEnable(GL_DEPTH_TEST);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0);
                glVertex2f( -2.0,-2.0);
                glTexCoord2f(0.0, 1.0);
                glVertex2f( -2.0, 2.0);
                glTexCoord2f(1.0, 1.0);
                glVertex2f(  2.0, 2.0);
                glTexCoord2f(1.0, 0.0);
                glVertex2f(  2.0,-2.0);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
};
