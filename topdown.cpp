// g++ -std=c++11 topdown.cpp -lglut -lGLU -lGL -lpng

#include <GL/glut.h>  
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include <png.h>
#include <GL/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

using namespace std;

int tempo = 0;
int* keyStates = new int[256];
static GLuint idle_shotgun_0;
static GLuint idle_shotgun_1;
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




class Coordenada {
public:
    float x, y;
    void set(float p_x, float p_y){
        x=p_x;
        y=p_y;
    }
};

class Cor {
public:
    float r, g, b;
    Cor(): r(0.0f), g(0.0f), b(0.0f){ }
    Cor(float red ,float green ,float blue): r(red), g(green), b(blue){ }

    void set(Cor c){
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

class Player {
public:
    Coordenada pos;
    Cor cor;   
    int num_segmentos;
    float raio, inclinacao;
    string caminhando, arma;
    bool shoot;
    Player(float x, float y, float r, int seg, float angulo, Cor c){
        cor.set(c);
        pos.set(x, y);
        num_segmentos=seg;
        inclinacao = angulo;
        raio = r;
        arma = 1;
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

    void render(){
        // cout << "player.x:" << pos. x << endl;
        // cout << "player.y:" << pos. y << endl;
        glPushMatrix();
            glTranslatef(pos.x, pos.y, 0);
            glRotatef(inclinacao, 0, 0, 1);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glEnable(GL_TEXTURE_2D);

            if (tempo %100 <  50){
                glBindTexture(GL_TEXTURE_2D, idle_shotgun_0);
            }else{
                glBindTexture(GL_TEXTURE_2D, idle_shotgun_1);
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
            if (tempo%4 < 1 && shoot){
                rand_offset = (rand() % 5  ) - 2.5;
                glPushMatrix();
                    glRotatef(rand_offset-86, 0, 0, 1);
                    glBegin(GL_POLYGON);
                        glColor4f(1, 1, 0, 1);
                        glVertex2f( +0.88, -0.0);
                        glVertex2f( +0.88, +100);
                        glVertex2f( +0.95, +100);
                        glVertex2f( +0.95, -0.0);
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
                    glVertex2f(pos.x+cx + 0, pos.y+cy + 0);
                }
            glEnd();

            glBegin(GL_LINE_LOOP);
                glColor4f(1, 1, 0, 1);
                for(int i = 0; i < 30; i++){
                    float theta = 2.0f * 3.1415926f * float(i) / float(30);
                    float cx = 0.1 * cosf(theta);
                    float cy = 0.1 * sinf(theta);
                    glVertex2f(pos.x+cx + 0, pos.y+cy + 0);
                }
            glEnd();
        glPopMatrix();
    }
};

// INICIALIZAÇÃO DOS OBJETOS E VARIÁVEIS
Cor    CINZA(0.5,0.5,0.5);
Cor VERMELHO(1.0,0.0,0.0);
Cor    VERDE(0.0,1.0,0.0);
Cor     AZUL(0.0,0.0,1.0);
Cor  AMARELO(1.0,1.0,0.0);
Cor    PRETO(0.0,0.0,0.0);
Cor  LARANJA(1.0,0.6,0.3);
Cor  BRANCO( 1.0,1.0,1.0);
//      Arma(  num, damage, rate, reload, cap, accuracy  )
Arma    faca(  1,   20,     1,    0,       1,  0);
Arma   glock(  1,   30,     3,    6,      11,  2);
Arma    doze(  1,   100,    1,    8,       5,  6);
Arma    ak47(  1,   40 ,    8,    3,      25,  3);
Texto  texto("(0,0)", BRANCO);
Player player(0.0f, 0.0f, 10.0f, 100, 0, PRETO);
Mira mira;

void myDisplay(void){
    // REDERIZAÇÃO
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glTranslatef(-player.pos.x, -player.pos.y, -35);
    glTranslatef(-player.pos.x, -player.pos.y, -35);
    glRotatef(0, 1, 1, 1);

    mira.render(player.pos.x, player.pos.y);
    texto.render(player.pos.x, player.pos.y);


    glBegin(GL_POLYGON);
    glColor4f(1, 1, 0, 1);
    glVertex2f( -4, +2);
    glVertex2f( -4, +4);
    glVertex2f( +4, +4);
    glVertex2f( +4, +2);
    glEnd();
    player.render();


    glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
    glutPostRedisplay();

}

void initTextures(){
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
    // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    char filename[] = "./Top_Down_Survivor/shotgun/idle/survivor-idle_shotgun_0.png";

    idle_shotgun_0 = png_texture_load("./Top_Down_Survivor/shotgun/idle/survivor-idle_shotgun_0.png", NULL, NULL);
    idle_shotgun_1 = png_texture_load("./Top_Down_Survivor/knife/meleeattack/survivor-meleeattack_knife_11.png", NULL, NULL);
}

void myIdle(){
    tempo++;
    player.caminha();
    if (tempo == 1000)
        tempo = 0;
}

void mouseClicks(int button, int state, int x, int y){
    mira.atualiza(x,y);
    player.rotate(x, y);
    player.shoot = state == GLUT_DOWN;
}

void keyDown(unsigned char tecla, int x, int y){
    keyStates[tecla] = 1;
}

void keyUp(unsigned char tecla, int x, int y){
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
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);  // Inicializa o display mode
    glutCreateWindow("PNG texture");             // Inicializa a janaela
    initCallbacks();                             // Inicializa callbacks
    initTextures();                              // Inicializa texturas
    initKeyboard();                              // Inicializa teclado
    initMouse();                                 // Inicializa mouse
    glutMainLoop();                              // Main loop
    return 0;                                    // Nunca retorna
}