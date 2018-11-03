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

namespace fs = std::experimental::filesystem;
using namespace std;

int tempo = 0;
int frame_time = 20;
int* keyStates = new int[256];
int screen_w = 700;
int screen_h = 700;
int rand_offset;
GLuint textureID;


GLuint png_texture_load(const char * file_name, int * width, int * height)
{
    // This function was originally written by David Grayson for
    // https://github.com/DavidEGrayson/ahrs-visualizer

    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    if (width){ *width = temp_width; }
    if (height){ *height = temp_height; }

    //printf("%s: %lux%lu %d\n", file_name, temp_width, temp_height, color_type);

    if (bit_depth != 8)
    {
        fprintf(stderr, "%s: Unsupported bit depth %d.  Must be 8.\n", file_name, bit_depth);
        return 0;
    }

    GLint format;
    switch(color_type)
    {
    case PNG_COLOR_TYPE_RGB:
        format = GL_RGB;
        break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        format = GL_RGBA;
        break;
    default:
        fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
        return 0;
    }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_byte ** row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    for (unsigned int i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return texture;
}



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
    int num, damage, cap;
    float rate, reload, accuracy;
    Arma(int _damage, int _cap, int _num, float _rate, float _reload, float _accuracy){
        num      =      _num;
        damage   =   _damage;
        rate     =     _rate;
        reload   =   _reload;
        cap      =      _cap;
        accuracy = _accuracy;
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
        cout << animacao << endl;
        data = tex;
    }
};

class Player {
    public:
    Coordenada pos;
    Cor cor;   
    int num_segmentos, animacao_frames;
    float raio, inclinacao;
    string caminhando, arma, animacao;

    bool shoot;

    Player(float x, float y, float r, int seg, float angulo, Cor c){
        cor.set(c);
        pos = Coordenada(x,y);
        num_segmentos=seg;
        inclinacao = angulo;
        raio = r;
        arma = "flashlight";
    }

    void troca_arma(string nome){
        arma = nome;
    }

    void caminha(){
        float velocidade;
        if ((keyStates['w'] + keyStates['s'] + keyStates['a'] + keyStates['d']) > 1 ||
            (keyStates['W'] + keyStates['S'] + keyStates['A'] + keyStates['D']) > 1){
            velocidade = 0.7*0.04;
        }else{
            velocidade = 0.04;
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

    void render(Textura texturas[420]){
        // cout << "player.x:" << pos. x << endl;
        // cout << "player.y:" << pos. y << endl;
        glPushMatrix();
            glTranslatef(pos.x, pos.y, 0);
            glRotatef(inclinacao, 0, 0, 1);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glEnable(GL_TEXTURE_2D);
                Textura texture;
                for (int i = 0; i < 420; ++i){
                    if(texturas[i].animacao == animacao && texturas[i].index == (tempo / frame_time) % animacao_frames)
                        texture = texturas[i];
                }
                if(animacao == "knife_meleeattack"){
                    glTranslatef(0.2,-0.8,6);
                }
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_BLEND);
                glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBindTexture(GL_TEXTURE_2D, texture.data);
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
            if (tempo%40 < 20 && shoot){
                rand_offset = (rand() % 5  ) - 2.5;
                glPushMatrix();
                    glRotatef(rand_offset-86, 0, 0, 1);
                    glBegin(GL_POLYGON);
                        glColor4f(1, 1, 0, 1);
                        glVertex2f( +1.10, -0.0);
                        glVertex2f( +1.10, +100);
                        glVertex2f( +1.15, +100);
                        glVertex2f( +1.15, -0.0);
                    glEnd();
                glPopMatrix();
            }
        glPopMatrix();


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

//       Cor(r, g, b, a)
Cor    CINZA(0.5, 0.5, 0.5, 1.0);
Cor VERMELHO(1.0, 0.0, 0.0, 1.0);
Cor    VERDE(0.0, 1.0, 0.0, 1.0);
Cor     AZUL(0.0, 0.0, 1.0, 1.0);
Cor  AMARELO(1.0, 1.0, 0.0, 1.0);
Cor    PRETO(0.0, 0.0, 0.0, 1.0);
Cor  LARANJA(1.0, 0.6, 0.3, 1.0);
Cor  BRANCO( 1.0, 1.0, 1.0, 1.0);
//      Arma(  num, damage, rate, reload, cap, accuracy  )
Arma    faca(  1,   20,     1,    0,      1,   0         );
Arma   glock(  1,   30,     3,    6,      11,  2         );
Arma    doze(  1,   100,    1,    8,      5,   6         );
Arma    ak47(  1,   40,     8,    3,      25,  3         );

Texto  texto("(0,0)", BRANCO);
Player player(0.0f, 0.0f, 10.0f, 100, 0, PRETO);
Mira mira;

Textura texturas[420];
Coordenada origem(0,0);
GameObject tiro(origem, texturas[0], AMARELO, 100, 0.1, 0);
GameObject obstaculo_0(Coordenada(20,-40), texturas[0], PRETO, 10, 10, 10);
GameObject obstaculo_1(Coordenada(-10,-30), texturas[0], PRETO, 10, 10, 40);
GameObject obstaculo_2(Coordenada(-2,10), texturas[0], PRETO, 10, 10, -210);



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
    player.render(texturas);
    obstaculo_0.render();
    obstaculo_1.render();
    obstaculo_2.render();

    glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
    glutPostRedisplay();
}

void loadTextures(){
    glClearColor(1,1,1,1);
    // glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::string path = "./_player/";
    string path_string_array[420];
    string str_temp; 
    int i = 0;
    for (auto & p : fs::directory_iterator(path)){
        for (auto & pp : fs::directory_iterator(p)){
            for (auto & ppp : fs::directory_iterator(pp)){
                str_temp = ppp.path().string().c_str();
                path_string_array[i] = ppp.path().string().c_str();
                char *path_char_array_dos_inferno = new char[path_string_array[i].size()+1];
                strncpy(path_char_array_dos_inferno, path_string_array[i].c_str(), path_string_array[i].size());
                // textures[i] = png_texture_load(path_char_array_dos_inferno, NULL, NULL);
                texturas[i].set(png_texture_load(path_char_array_dos_inferno, NULL, NULL), str_temp);
                i++;
            }
        }
    }

    // char filename[] = "./_player/shotgun/idle/survivor-idle_shotgun_0.png";
    // textures[1] = png_texture_load("./_player/knife/meleeattack/survivor-meleeattack_knife_11.png", NULL, NULL);
}

void myIdle(){
    tempo++;
    player.caminha();
    if (tempo == 420*frame_time)
        tempo = 0;
}

void mouseClicks(int button, int state, int x, int y){
    mira.atualiza(x,y);
    player.rotate(x,y);
    player.shoot = state == GLUT_DOWN;
    if (player.arma == "shotgun"){
        player.animacao = "shotgun_shoot";
        player.animacao_frames = 3;
    }
    if (player.arma == "handgun"){
        player.animacao = "handgun_shoot";
        player.animacao_frames = 3;
    }
    if (player.arma == "rifle"){
        player.animacao = "rifle_shoot";
        player.animacao_frames = 3;
    }
    if (player.arma == "knife"){
        player.animacao = "knife_meleeattack";
        player.animacao_frames = 15;
    }
    if (player.arma == "flashlight"){
        player.animacao = "flashlight_meleeattack";
        player.animacao_frames = 15;
    }
}

void keyDown(unsigned char tecla, int x, int y){
    keyStates[tecla] = 1;
    if(tecla == '1')
        player.troca_arma("knife");
    if(tecla == '2')
        player.troca_arma("handgun");
    if(tecla == '3')
        player.troca_arma("shotgun");
    if(tecla == '4')
        player.troca_arma("rifle");
    if(tecla == 'f' || tecla == 'F' )
        player.troca_arma("flashlight");
}

void keyUp(unsigned char tecla, int x, int y){
    //
    //
    //
    keyStates[tecla] = 0;
}

void cursormouse(int x, int y){
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