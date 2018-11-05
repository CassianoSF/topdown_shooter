#include "includes.cpp"  
#include "load_png.cpp"

namespace fs = std::experimental::filesystem;
using namespace std;




// #####################      #####################      #####################    
// #####################      #####################      #####################    
// #####################      #####################      #####################    
// #####################      #####################      #####################    
class Coordinate {
    public:
    float x, y;
    Coordinate(){}  
    Coordinate(float _x, float _y){
        x = _x;
        y = _y;
    }
};

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

class Texture {
    public:
    string animation;
    int index;
    GLuint data;
    Texture(){}
    void set(GLuint tex, string filename){
        smatch m;
        regex integer("(\\+|-)?[[:digit:]]+");
        regex_search(filename, m, regex("(\\+|-)?[[:digit:]]+"));
        for(auto v: m){
            stringstream geek(v);
            geek >> index;
        }
        regex_search(filename, m, regex("shoot/shoot"));            for(auto v: m){animation = "shoot_texture";} 
        regex_search(filename, m, regex("flashlight/meleeattack")); for(auto v: m){animation = "flashlight_meleeattack";} 
        regex_search(filename, m, regex("flashlight/move"));        for(auto v: m){animation = "flashlight_move";} 
        regex_search(filename, m, regex("flashlight/idle"));        for(auto v: m){animation = "flashlight_idle";} 
        regex_search(filename, m, regex("feet/walk"));              for(auto v: m){animation = "feet_walk";} 
        regex_search(filename, m, regex("feet/strafe_left"));       for(auto v: m){animation = "feet_strafe_left";} 
        regex_search(filename, m, regex("feet/strafe_right"));      for(auto v: m){animation = "feet_strafe_right";} 
        regex_search(filename, m, regex("feet/run"));               for(auto v: m){animation = "feet_run";} 
        regex_search(filename, m, regex("feet/idle"));              for(auto v: m){animation = "feet_idle";} 
        regex_search(filename, m, regex("knife/meleeattack"));      for(auto v: m){animation = "knife_meleeattack";} 
        regex_search(filename, m, regex("knife/move"));             for(auto v: m){animation = "knife_move";} 
        regex_search(filename, m, regex("knife/idle"));             for(auto v: m){animation = "knife_idle";} 
        regex_search(filename, m, regex("handgun/meleeattack"));    for(auto v: m){animation = "handgun_meleeattack";} 
        regex_search(filename, m, regex("handgun/move"));           for(auto v: m){animation = "handgun_move";} 
        regex_search(filename, m, regex("handgun/shoot"));          for(auto v: m){animation = "handgun_shoot";} 
        regex_search(filename, m, regex("handgun/reload"));         for(auto v: m){animation = "handgun_reload";} 
        regex_search(filename, m, regex("handgun/idle"));           for(auto v: m){animation = "handgun_idle";} 
        regex_search(filename, m, regex("rifle/meleeattack"));      for(auto v: m){animation = "rifle_meleeattack";} 
        regex_search(filename, m, regex("rifle/move"));             for(auto v: m){animation = "rifle_move";} 
        regex_search(filename, m, regex("rifle/shoot"));            for(auto v: m){animation = "rifle_shoot";} 
        regex_search(filename, m, regex("rifle/reload"));           for(auto v: m){animation = "rifle_reload";} 
        regex_search(filename, m, regex("rifle/idle"));             for(auto v: m){animation = "rifle_idle";} 
        regex_search(filename, m, regex("shotgun/meleeattack"));    for(auto v: m){animation = "shotgun_meleeattack";} 
        regex_search(filename, m, regex("shotgun/move"));           for(auto v: m){animation = "shotgun_move";} 
        regex_search(filename, m, regex("shotgun/shoot"));          for(auto v: m){animation = "shotgun_shoot";} 
        regex_search(filename, m, regex("shotgun/reload"));         for(auto v: m){animation = "shotgun_reload";} 
        regex_search(filename, m, regex("shotgun/idle"));           for(auto v: m){animation = "shotgun_idle";} 
        cout << filename << endl;
        data = tex;
    }

    void render(){
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glEnable(GL_TEXTURE_2D);
        glTranslatef(0, 0, 1);
        if(animation == "knife_meleeattack"){
            glTranslatef(0.3,-0.45,6.2);
        }

        if (animation == "flashlight_meleeattack"){
            glTranslatef(-0.1,-0.1, 2.2);
        }

        if(animation == "handgun_meleeattack"){
            glTranslatef(0.2,-0.2,3.2);
        }   
        if(animation == "rifle_meleeattack" || animation == "shotgun_meleeattack"){
            glTranslatef(0,0,8);
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, data);
    }
};

class Text {
    public:
    string text_content;
    Color color;

    Text(string conteudo, Color c){
        text_content = conteudo;
        color.set(c);
    }

    void set_texto(string content){
        text_content = content;
    }

    void render(float x, float y){
        glPushMatrix();
        glTranslatef(x-20.5, y+18, 0);
        glColor3f(color.r, color.g, color.b);
        glRasterPos2f(1,1);
        for (int i = 0; i < sizeof text_content; ++i){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text_content[i]);
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
    Coordinate pos;

    void update(int x, int y){
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
    Coordinate pos;
    Color color;   
    int animation_frames;
    int reload_left;
    int shoot_left;
    float rand_offset;
    float raio, inclinacao;
    string animation, pernas;
    Arma arma;


    bool shoot=false, 
         idle=true, 
         walk=false, 
         run=false, 
         reload=false, 
         attack=false;

    Player(Arma _arma, float x, float y, float r, float angle, Color c){
        color.set(c);
        pos = Coordinate(x,y);
        inclinacao = angle;
        raio = r;
        arma = _arma;
    }

    void update(int game_time){
        cout << pernas << endl;
        stringstream ss;

        if(attack){
            animation_frames = 15;
            ss << arma.name << "_" << "meleeattack";
        }else if (reload && arma.reload_time && reload_left){
            reload_left--;
            animation_frames = 15;
            ss << arma.name << "_" << "reload";
            reload = !(reload_left == 1);

        }else if (shoot && arma.shoot){
            shoot_left--;
            animation_frames = 3;
            ss << arma.name << "_" << "shoot";
            shoot = !(shoot_left == 1);

        }else if(walk || run){
            if (run){
                pernas = "feet_run";
            }else
                pernas = "feet_walk";

            animation_frames = 20;
            ss << arma.name << "_" << "move";
        }else if(idle){
            ss << arma.name << "_" << "idle";
            animation_frames = 20;
        }
        animation = ss.str();
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

    void renderTiro(Texture textures[421], int game_time, int frame_time){
        if (shoot && arma.shoot && !reload && (game_time / frame_time) % arma.rate == 1){
            glPushMatrix();
                for (int i = 0; i < 421; ++i){
                    if(textures[i].animation == "shoot_texture")
                        textures[i].render();
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

    void renderBody(Texture textures[421], int game_time, int frame_time){
        Texture texture;
        for (int i = 0; i < 421; ++i){
            if(textures[i].animation == animation && textures[i].index == (game_time / frame_time) % animation_frames)
                textures[i].render();
        }
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

    }

    void renderLegs(Texture textures[421], int game_time, int frame_time){
        glPushMatrix();
            glTranslatef(0,0, -3);
            Texture texture;
            for (int i = 0; i < 421; ++i){
                if(idle && textures[i].animation == "feet_idle")
                    textures[i].render();
                else if(textures[i].animation == pernas && textures[i].index == (game_time / frame_time) % 20)
                    textures[i].render();
            }
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

    void render(Texture textures[421], int game_time, int frame_time){
        glPushMatrix();
            glTranslatef(pos.x, pos.y, 0);
            glRotatef(inclinacao, 0, 0, 1);
            renderLegs(textures, game_time, frame_time);
            renderTiro(textures, game_time, frame_time);
            renderBody(textures, game_time, frame_time);
        glPopMatrix();
    }
};

class GameObject{
    public:
    Coordinate pos;
    Texture tex;
    Color color;
    float height, width, angle;

    GameObject(Coordinate _pos, Texture _tex, Color _color, float _height, float _width, float _angle){
        pos = _pos;
        tex = _tex;
        color = _color;
        height = _height;
        width = _width;
        angle = _angle;
    }

    void render(){
        glPushMatrix();
            glTranslatef(pos.x, pos.y , 0);
            glRotatef(angle-90, 0, 0, 1);
            glBegin(GL_POLYGON);
                glColor4f(color.r, color.g, color.b, color.a);
                glVertex2f( width/2,  height/2);
                glVertex2f( width/2, -height/2);
                glVertex2f(-width/2, -height/2);
                glVertex2f(-width/2,  height/2);
            glEnd();
        glPopMatrix();
    }
};




int* keyStates = new int[256];
int screen_w = 1000;
int screen_h = 700;
int game_time = 0;
int frame_time = 100;

//       Color(r, g, b, a)
Color    CINZA(0.5, 0.5, 0.5, 1.0);
Color VERMELHO(1.0, 0.0, 0.0, 1.0);
Color    VERDE(0.0, 1.0, 0.0, 1.0);
Color     AZUL(0.0, 0.0, 1.0, 1.0);
Color  AMARELO(1.0, 1.0, 0.0, 1.0);
Color    PRETO(0.0, 0.0, 0.0, 1.0);
Color  LARANJA(1.0, 0.6, 0.3, 1.0);
Color  BRANCO( 1.0, 1.0, 1.0, 1.0);
//          Arma(bool _shoot,  string _name, int _damage, int _cap, int _num, int _rate, int _reload_time, int _accuracy){
Arma flashlight( false,        "flashlight", 1,           40,       1,        1,         1,                1         );
Arma      knife( false,        "knife",      1,           20,       2,        1,         1,                1         );
Arma    handgun( true,         "handgun",    1,           30,       3,        10,        3000,             2         );
Arma    shotgun( true,         "shotgun",    1,           100,      4,        10,        7000,             6         );
Arma      rifle( true,         "rifle",      1,           40,       5,        2,         4000,             3         );

Text  texto("(0,0)", BRANCO);
Mira mira;

Texture textures[421];
Coordinate origem(0,0);
GameObject tiro(origem, textures[0], AMARELO, 100, 0.1, 0);
GameObject obstaculo_0(Coordinate(20,-40), textures[0], VERDE, 10, 10, 10);
GameObject obstaculo_1(Coordinate(-10,-30), textures[0], VERDE, 10, 10, 40);
GameObject obstaculo_2(Coordinate(-2,10), textures[0], VERDE, 10, 10, -210);

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
    player.render(textures, game_time, frame_time);

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
                textures[i].set(png_texture_load(path_char_array, NULL, NULL), str_temp);
                i++;
            }
        }
    }
}

void myIdle(){
    game_time++;
    player.update(game_time);
    player.caminha(keyStates);
    if (game_time == 421*frame_time)
        game_time = 0;
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
        game_time = 0;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        player.attack = true;
        game_time = 0;
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

    if (keyStates['a'] + keyStates['s'] + keyStates['d'] + keyStates['w'] + keyStates['A'] + keyStates['S'] + keyStates['D'] + keyStates['W']){
        player.idle = false;
    }else{
        player.run = false;
        player.walk = false;
        player.idle = true;
    }
}

void keyUp(unsigned char tecla, int x, int y){
    keyStates[tecla] = 0;
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    if (tecla == 'a' || tecla == 'A'){ keyStates['a'] = 0; keyStates['A'] = 0;}
    if (tecla == 's' || tecla == 'S'){ keyStates['s'] = 0; keyStates['S'] = 0;}
    if (tecla == 'd' || tecla == 'D'){ keyStates['d'] = 0; keyStates['D'] = 0;}
    if (tecla == 'w' || tecla == 'W'){ keyStates['w'] = 0; keyStates['W'] = 0;}
    if (keyStates['a'] + keyStates['s'] + keyStates['d'] + keyStates['w'] + keyStates['A'] + keyStates['S'] + keyStates['D'] + keyStates['W']){
        player.idle = false;
    }else{
        player.run = false;
        player.walk = false;
        player.idle = true;
    }

}

void cursormouse(int x, int y){
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    mira.update(x,y);
    player.rotate(x,y);
    char temp[100];
    sprintf(temp, " (%d, %d)  %f", x, y, player.inclinacao);
    texto.set_texto(temp);
}

void atirando(int x, int y){
    mira.update(x,y);
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
    glutIdleFunc(myIdle);                      // Seta função de updateção
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
    loadTextures();                              // Inicializa textures
    initKeyboard();                              // Inicializa teclado
    initMouse();                                 // Inicializa mouse
    glutMainLoop();                              // Main loop
    return 0;                                    // Nunca retorna
}