// g++ -std=c++17 topdown.cpp  -lglut -lGLU -lGL -lpng  -lstdc++fs

#define GLFW_INCLUDE_ES2
#include <GL/glut.h>  
#include <stdio.h>
#include <stdlib.h> 
#include <sstream> 
#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include <png.h>
#include <GL/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <vector>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <experimental/filesystem>
#include <regex>
#include "load_png.cpp"

namespace fs = std::experimental::filesystem;
using namespace std;




// #####################      #####################      #####################    
// #####################      #####################      #####################    
// #####################      #####################      #####################    
// #####################      #####################      #####################    
class Coordenada {
    public:
    float x, y;
    Coordenada(){}  
    Coordenada(float _x, float _y){
        x = _x;
        y = _y;
    }
};


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


class Textura {
    public:
    string animacao;
    int index;
    GLuint data;
    Textura(){}
    void set(GLuint tex, string filename){
        smatch m;
        regex integer("(\\+|-)?[[:digit:]]+");
        regex_search(filename, m, regex("(\\+|-)?[[:digit:]]+"));
        for(auto v: m){
            stringstream geek(v);
            geek >> index;
        }
        regex_search(filename, m, regex("shoot/shoot"));            for(auto v: m){animacao = "shoot_texture";} 
        regex_search(filename, m, regex("flashlight/meleeattack")); for(auto v: m){animacao = "flashlight_meleeattack";} 
        regex_search(filename, m, regex("flashlight/move"));        for(auto v: m){animacao = "flashlight_move";} 
        regex_search(filename, m, regex("flashlight/idle"));        for(auto v: m){animacao = "flashlight_idle";} 
        regex_search(filename, m, regex("feet/walk"));              for(auto v: m){animacao = "feet_walk";} 
        regex_search(filename, m, regex("feet/strafe_left"));       for(auto v: m){animacao = "feet_strafe_left";} 
        regex_search(filename, m, regex("feet/strafe_right"));      for(auto v: m){animacao = "feet_strafe_right";} 
        regex_search(filename, m, regex("feet/run"));               for(auto v: m){animacao = "feet_run";} 
        regex_search(filename, m, regex("feet/idle"));              for(auto v: m){animacao = "feet_idle";} 
        regex_search(filename, m, regex("knife/meleeattack"));      for(auto v: m){animacao = "knife_meleeattack";} 
        regex_search(filename, m, regex("knife/move"));             for(auto v: m){animacao = "knife_move";} 
        regex_search(filename, m, regex("knife/idle"));             for(auto v: m){animacao = "knife_idle";} 
        regex_search(filename, m, regex("handgun/meleeattack"));    for(auto v: m){animacao = "handgun_meleeattack";} 
        regex_search(filename, m, regex("handgun/move"));           for(auto v: m){animacao = "handgun_move";} 
        regex_search(filename, m, regex("handgun/shoot"));          for(auto v: m){animacao = "handgun_shoot";} 
        regex_search(filename, m, regex("handgun/reload"));         for(auto v: m){animacao = "handgun_reload";} 
        regex_search(filename, m, regex("handgun/idle"));           for(auto v: m){animacao = "handgun_idle";} 
        regex_search(filename, m, regex("rifle/meleeattack"));      for(auto v: m){animacao = "rifle_meleeattack";} 
        regex_search(filename, m, regex("rifle/move"));             for(auto v: m){animacao = "rifle_move";} 
        regex_search(filename, m, regex("rifle/shoot"));            for(auto v: m){animacao = "rifle_shoot";} 
        regex_search(filename, m, regex("rifle/reload"));           for(auto v: m){animacao = "rifle_reload";} 
        regex_search(filename, m, regex("rifle/idle"));             for(auto v: m){animacao = "rifle_idle";} 
        regex_search(filename, m, regex("shotgun/meleeattack"));    for(auto v: m){animacao = "shotgun_meleeattack";} 
        regex_search(filename, m, regex("shotgun/move"));           for(auto v: m){animacao = "shotgun_move";} 
        regex_search(filename, m, regex("shotgun/shoot"));          for(auto v: m){animacao = "shotgun_shoot";} 
        regex_search(filename, m, regex("shotgun/reload"));         for(auto v: m){animacao = "shotgun_reload";} 
        regex_search(filename, m, regex("shotgun/idle"));           for(auto v: m){animacao = "shotgun_idle";} 
        cout << filename << endl;
        data = tex;
    }

    void render(){
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glEnable(GL_TEXTURE_2D);
        glTranslatef(0, 0, 7);
        if(animacao == "knife_meleeattack"){
            glTranslatef(0.3,-0.45,6.2);
        }

        if (animacao == "flashlight_meleeattack"){
            glTranslatef(-0.1,-0.1, 2.2);
        }

        if(animacao == "handgun_meleeattack"){
            glTranslatef(0.2,-0.2,3.2);
        }   
        if(animacao == "rifle_meleeattack" || animacao == "shotgun_meleeattack"){
            glTranslatef(0,0,8);
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, data);
    }
};

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




int* keyStates = new int[256];
int screen_w = 1000;
int screen_h = 700;
int tempo = 0;
int frame_time = 120;

//       Cor(r, g, b, a)
Cor    CINZA(0.5, 0.5, 0.5, 1.0);
Cor VERMELHO(1.0, 0.0, 0.0, 1.0);
Cor    VERDE(0.0, 1.0, 0.0, 1.0);
Cor     AZUL(0.0, 0.0, 1.0, 1.0);
Cor  AMARELO(1.0, 1.0, 0.0, 1.0);
Cor    PRETO(0.0, 0.0, 0.0, 1.0);
Cor  LARANJA(1.0, 0.6, 0.3, 1.0);
Cor  BRANCO( 1.0, 1.0, 1.0, 1.0);
//          Arma(bool _shoot,  string _name, int _damage, int _cap, int _num, int _rate, int _reload_time, int _accuracy){
Arma flashlight( false,        "flashlight", 1,           40,       1,        1,         1,                1         );
Arma      knife( false,        "knife",      1,           20,       2,        1,         1,                1         );
Arma    handgun( true,         "handgun",    1,           30,       3,        10,        3000,             2         );
Arma    shotgun( true,         "shotgun",    1,           100,      4,        10,        7000,             6         );
Arma      rifle( true,         "rifle",      1,           40,       5,        2,         4000,             3         );

Texto  texto("(0,0)", BRANCO);
Mira mira;

Textura texturas[421];
Coordenada origem(0,0);
GameObject tiro(origem, texturas[0], AMARELO, 100, 0.1, 0);
GameObject obstaculo_0(Coordenada(20,-40), texturas[0], VERDE, 10, 10, 10);
GameObject obstaculo_1(Coordenada(-10,-30), texturas[0], VERDE, 10, 10, 40);
GameObject obstaculo_2(Coordenada(-2,10), texturas[0], VERDE, 10, 10, -210);

Player player(knife, 0.0f, 0.0f, 10.0f, 0, PRETO);












//################    ##################    ###################    ##################
//################    ##################    ###################    ##################
//################    ##################    ###################    ##################
//################    ##################    ###################    ##################



void myDisplay(void){
    // REDERIZAÇÃO
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glTranslatef(-player.pos.x, -player.pos.y, -35);
    glTranslatef(-player.pos.x, -player.pos.y, -35);
    glRotatef(0, 1, 1, 1);

    mira.render(player.pos.x, player.pos.y);
    texto.render(player.pos.x, player.pos.y);
    obstaculo_0.render();
    obstaculo_1.render();
    obstaculo_2.render();
    player.render(texturas, tempo, frame_time);

    glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
    glutPostRedisplay();
}

void loadTextures(){
    glClearColor(0,0,0,1);
    // glDepthMask(GL_FALSE);

    std::string path = "./_player/";
    string path_string_array[421];
    string str_temp; 
    int i = 0;
    for (auto & p : fs::directory_iterator(path)){
        for (auto & pp : fs::directory_iterator(p)){
            for (auto & ppp : fs::directory_iterator(pp)){
                str_temp = ppp.path().string().c_str();
                path_string_array[i] = ppp.path().string().c_str();
                char *path_char_array = new char[path_string_array[i].size()+1];
                strncpy(path_char_array, path_string_array[i].c_str(), path_string_array[i].size());
                texturas[i].set(png_texture_load(path_char_array, NULL, NULL), str_temp);
                i++;
            }
        }
    }
}

void myIdle(){
    tempo++;
    player.atualiza(tempo);
    player.caminha(keyStates);
    if (tempo == 421*frame_time)
        tempo = 0;
}

void mouseClicks(int button, int state, int x, int y){
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    player.rotate(x,y);
    if(!player.reload){
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            player.shoot = true;
            player.rand_offset = (rand() % player.arma.accuracy) - 2.5;
            player.shoot_left = 3 * frame_time;
        }else{
            player.shoot = false;
        }
        tempo = 0;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        player.attack = true;
        tempo = 0;
    }else{
        player.attack = false;
    }
    
}

void keyDown(unsigned char tecla, int x, int y){
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    keyStates[tecla] = 1;
    if(tecla == 'r' || tecla == 'R'){
        player.reload = true;
        player.reload_left = player.arma.reload_time;
        player.shoot = false;
        player.attack = false;
    }
    if(tecla == '1')
        player.arma = knife;
    if(tecla == '2')
        player.arma = handgun;
    if(tecla == '3')
        player.arma = shotgun;
    if(tecla == '4')
        player.arma = rifle;
    if(tecla == 'f' || tecla == 'F' )
        player.arma = flashlight;
}

void keyUp(unsigned char tecla, int x, int y){
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    if (tecla == 'a' || tecla == 'A'){ keyStates['a'] = 0; keyStates['A'] = 0;}
    if (tecla == 's' || tecla == 'S'){ keyStates['s'] = 0; keyStates['S'] = 0;}
    if (tecla == 'd' || tecla == 'D'){ keyStates['d'] = 0; keyStates['D'] = 0;}
    if (tecla == 'w' || tecla == 'W'){ keyStates['w'] = 0; keyStates['W'] = 0;}
    keyStates[tecla] = 0;
}

void cursormouse(int x, int y){
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    mira.atualiza(x,y);
    player.rotate(x,y);
    char temp[100];
    sprintf(temp, " (%d, %d)  %f", x, y, player.inclinacao);
    texto.set_texto(temp);
}

void atirando(int x, int y){
    mira.atualiza(x,y);
    player.rotate(x, y);
}

void myReshape(GLsizei w, GLsizei h){ 
    glViewport(0, 0, screen_w, screen_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0 * (GLfloat) w / (GLfloat) h, 1.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
}

void initCallbacks(){
    glutReshapeFunc(myReshape);
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);                      // Seta função de atualização
}

void initKeyboard(){
    glutKeyboardFunc(keyDown);                  // Callback ao pressionar tecla
    glutKeyboardUpFunc(keyUp);                  // Callback ao soltar tecla
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);      // Desativa reperição de tecla
}

void initMouse(){
    glutPassiveMotionFunc(cursormouse);         // Callback do cursor movendo 
    glutMouseFunc(mouseClicks);                 // Callback do click
    glutMotionFunc(atirando);                   // Callback do drag
    glutSetCursor(GLUT_CURSOR_NONE);            // Esconde cursor
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc, argv);                       // Inicializa GLUT
    glutInitWindowSize(screen_w, screen_h);      // Inicializa tamanho da janela
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);  // Inicializa o display mode
    glutCreateWindow("PNG texture");             // Inicializa a janaela
    initCallbacks();                             // Inicializa callbacks
    loadTextures();                              // Inicializa texturas
    initKeyboard();                              // Inicializa teclado
    initMouse();                                 // Inicializa mouse
    glutMainLoop();                              // Main loop
    return 0;                                    // Nunca retorna
}