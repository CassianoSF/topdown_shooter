// g++ -std=c++11 texture.cpp -lglut -lGLU -lGL -lrt  -lSOIL -lGLEW -lsfml-window -lsfml-system

#include <GL/glut.h>  
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include <png.h>
 

using namespace std;

bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
 
    if ((fp = fopen(name, "rb")) == NULL)
        return false;
 
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
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

GLfloat win = 250.0f;
int tempo = 0;
bool senta_o_dedo = false;
int* keyStates = new int[256];
GLubyte *textureImage;


class Coordenada {
public:
    float x, y;
    void set(float _x, float _y){
        x=_x;
        y=_y;
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
        glColor3f(cor.r, cor.g, cor.b);
        glPushMatrix();
        glRasterPos2f(-win, win-(win*0.08));
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
    Coordenada posicao;
    Cor cor;   
    int num_segmentos;
    float raio, inclinacao;
    string caminhando, arma;

    Player(float x, float y, float r, int seg, float angulo, Cor c){
        cor.set(c);
        posicao.x = x;
        posicao.y = y;
        num_segmentos=seg;
        inclinacao = angulo;
        raio = r;
        arma = 1;
    } 

    void caminha(){
        int velocidade;
        if ((keyStates['w'] + keyStates['s'] + keyStates['a'] + keyStates['d']) > 1 ||
            (keyStates['W'] + keyStates['S'] + keyStates['A'] + keyStates['D']) > 1){
            velocidade = 0.7*3;
        }else{
            velocidade = 3;
        }
        if(keyStates['w'] || keyStates['W'] ){ posicao.y += velocidade; }
        if(keyStates['s'] || keyStates['S'] ){ posicao.y -= velocidade; }
        if(keyStates['a'] || keyStates['A'] ){ posicao.x -= velocidade; }
        if(keyStates['d'] || keyStates['D'] ){ posicao.x += velocidade; }
    }

    void mira(int x, int y){
        y = -y + 350;
        x =  x - 350;
        if(x != 0 && y !=0){
            inclinacao = -(GLfloat)atan2(x-posicao.x,y-posicao.y)/3.1415*180.0;
        } 
    }

    void render(){

        int rand_offset = (rand() % 5  ) - 2.5;
        glPushMatrix();
        glTranslatef(posicao.x, posicao.y, 0);
        glRotatef(inclinacao, 0, 0, 1);

        if (tempo%8 < 4 && senta_o_dedo){
            glPushMatrix();
            glRotatef(rand_offset, 0, 0, 1);
            glBegin(GL_POLYGON);
            glColor3f(1, 1, 0);
            glVertex2f(0 - 1 + 8, 0 + 0);
            glVertex2f(0 - 1 + 8, 0 + 800);
            glVertex2f(0 + 1 + 8, 0 + 800);
            glVertex2f(0 + 1 + 8, 0 + 0);
            glEnd();
            glPopMatrix();
        }
        
        glBegin(GL_POLYGON);
        glColor3f(0, 0, 0);
        glVertex2f(0 - 1.5 + 8, 0 + 0);
        glVertex2f(0 - 1.5 + 8, 0 + 30);
        glVertex2f(0 + 1.5 + 8, 0 + 30);
        glVertex2f(0 + 1.5 + 8, 0 + 0);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0, 0.5, 0);
        for(int i = 0; i < num_segmentos; i++){
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segmentos);
            float cx = raio*2   * cosf(theta);
            float cy = raio*1.3 * sinf(theta);
            glVertex2f(cx + 0, cy + 0);
        }
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(cor.r, cor.g, cor.b);
        for(int i = 0; i < num_segmentos; i++){
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segmentos);
            float cx = raio * cosf(theta);
            float cy = raio * sinf(theta);
            glVertex2f(cx + 0, cy + (0-3));
        }
        glEnd();
        glPopMatrix();
    }
};

class Mira {
public:
    Coordenada posicao;

    void render(){
        glPushMatrix();
        glTranslatef(posicao.x, posicao.y, 0);
        glBegin(GL_LINE_LOOP);
        glColor3f(0.3, 0, 0);
        for(int i = 0; i < 30; i++){
            float theta = 2.0f * 3.1415926f * float(i) / float(30);
            float cx = 5 * cosf(theta);
            float cy = 5 * sinf(theta);
            glVertex2f(cx + 0, cy + 0);
        }
        glEnd();
        glBegin(GL_LINE_LOOP);
        glColor3f(0.3, 0, 0);
        for(int i = 0; i < 30; i++){
            float theta = 2.0f * 3.1415926f * float(i) / float(30);
            float cx = 2 * cosf(theta);
            float cy = 2 * sinf(theta);
            glVertex2f(cx + 0, cy + 0);
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

//      Arma(  num, damage, rate, reload, cap, accuracy  )
Arma    faca(  1,   20,     1,    0,       1,  0);
Arma   glock(  1,   30,     3,    6,      11,  2);
Arma    doze(  1,   100,    1,    8,       5,  6);
Arma    ak47(  1,   40 ,    8,    3,      25,  3);

Texto  texto("(0,0)", PRETO);
Player player(0.0f, 0.0f, 10.0f, 100, 0, PRETO);

Mira mira;

void Render(void){
    // REDERIZAÇÃO
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    glClear(GL_COLOR_BUFFER_BIT);
    player.render();
    texto.render();
    mira.render();
    glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
    glutPostRedisplay();

}


// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h){ 
    glViewport(0, 0, w, h); // Especifica as dimensões da Viewport
    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (-win, win, -win, win);
}

void Atualizar(){
    tempo++;
    player.caminha();
    if (tempo == 100)
        tempo = 0;
}



void volta (string dir){
    // if(
    //     player.colide(obstaculo1) || 
    //     player.colide(obstaculo2)
    // ){
    //     player.caminha(dir);
    //     player.caminha(dir);
    //     player.caminha(dir);
    // }
}

void mouseClicks(int button, int state, int x, int y){
    player.mira(x, y);
    if (state == GLUT_DOWN)
        senta_o_dedo = true;
    else
        senta_o_dedo = false;
}


void keyDown(unsigned char tecla, int x, int y){
    keyStates[tecla] = 1;
}

void keyUp(unsigned char tecla, int x, int y){
    keyStates[tecla] = 0;
}

void cursormouse(int x, int y){
    mira.posicao.x=x - 350;
    mira.posicao.y=-y + 350;
    player.mira(x, y);
    y = -y + 350;
    x = x - 350;
    char temp[100];
    sprintf(temp, " (%d, %d)  %f", x, y, player.inclinacao);
    texto.set_texto(temp);
}

void atirando(int x, int y){
    player.mira(x, y);
    mira.posicao.x=x - 350;
    mira.posicao.y=-y + 350;
}

void loadTextures(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    // The following two lines enable semi transparent
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    int width, height;
    bool hasAlpha;
    char filename[] = "sample.png";
    bool success = loadPngImage(filename, width, height, hasAlpha, &textureImage);
    if (!success) {
        std::cout << "Unable to load png file" << std::endl;
        return;
    }
    std::cout << "Image loaded " << width << " " << height << " alpha " << hasAlpha << std::endl;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, width,
                 height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
                 textureImage);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
}

void initKeyboard(){
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutSetCursor(GLUT_CURSOR_NONE);
}

void initWindow(){
    glutInitDisplayMode(GLUT_DOUBLE);        // Habilita double buffer(geralmente utilizado com animação) mas pode ser SINGLE também
    glutInitWindowSize(700, 700);            // Inicializa tamanho da janela
    glutInitWindowPosition(100,10);          // Posição inicial da janela na tela    
    glutCreateWindow("Exemplo Aula");        // Cria janela com titulo
    glutReshapeFunc(AlteraTamanhoJanela);    // Altera tamanho da janela
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    // Inicializar a cor de fundo da tela
}

void initMouse(){
    glutPassiveMotionFunc(cursormouse);      // Callback do cursor movendo 
    glutMouseFunc(mouseClicks);              // Callback do click
    glutMotionFunc(atirando);                // Callback do drag
    // glutSpecialFunc(TeclasEspeciais); 
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc, argv);                   // Inicializa GLUT
    initWindow();                            // Inicializa janela
    initKeyboard();                          // Inicializa teclado
    initMouse();                             // Inicializa mouse
    // loadTextures();                          // Carrega texturas

    glutDisplayFunc(Render);                 // Seta função de renderização
    glutIdleFunc(Atualizar);                 // Seta função de atualização
    glutMainLoop();                          // Chama a máquina de estados do OpenGL e processa todas as mensagens
    return 0;                                // nunca retorna
}