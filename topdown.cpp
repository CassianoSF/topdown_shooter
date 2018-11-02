// g++ -std=c++11 topdown.cpp -lglut -lGLU -lGL -lpng

#include <GL/glut.h>  
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include <png.h>
using namespace std;

GLfloat win = 250.0f;
int tempo = 0;
int* keyStates = new int[256];
GLubyte *idle_shotgun_0;
int screen_w = 700;
int screen_h = 700;
int rand_offset;
GLuint textureID;

bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
     png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
    if ((fp = fopen(name, "rb")) == NULL)
        return false;
 
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, sig_read);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
 
    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    outWidth = width;
    outHeight = height;
 
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
 
    for (int i = 0; i < outHeight; i++) {
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return true;
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

    void render(){
        glPushMatrix();
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
            velocidade = 0.7*0.01;
        }else{
            velocidade = 0.01;
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
            glEnable(GL_TEXTURE_2D);
            glActiveTexture(textureID);
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
            if (tempo%5 < 1 && shoot){
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
Texto  texto("(0,0)", PRETO);
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


    glBegin(GL_POLYGON);
    glColor4f(1, 1, 0, 1);
    glVertex2f( -4, +2);
    glVertex2f( -4, +4);
    glVertex2f( +4, +4);
    glVertex2f( +4, +2);
    glEnd();
    player.render();

    // texto.render();

    glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
    glutPostRedisplay();

}

void initTextures(){
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    // The following two lines enable semi transparent
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    char filename[] = "./Top_Down_Survivor/shotgun/idle/survivor-idle_shotgun_0.png";
    bool hasAlpha;
    int width, height;
    bool success = loadPngImage(filename, width, height, hasAlpha, &idle_shotgun_0);
    std::cout << "Image loaded " << width << " " << height << " alpha " << hasAlpha << std::endl;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, width, height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, idle_shotgun_0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glShadeModel(GL_FLAT);
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