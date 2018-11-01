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
bool senta_o_dedo = false;
int* keyStates = new int[256];
GLubyte *textureImage[10];


bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
     png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
 
    if ((fp = fopen(name, "rb")) == NULL)
        return false;
 
    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);
 
    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }
 
    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }
 
    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return false;
    }
 
    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);
 
    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);
 
    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
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
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
 
    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
 
    /* Close the file */
    fclose(fp);
 
    /* That's it */
    return true;
}

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
        float velocidade;
        if ((keyStates['w'] + keyStates['s'] + keyStates['a'] + keyStates['d']) > 1 ||
            (keyStates['W'] + keyStates['S'] + keyStates['A'] + keyStates['D']) > 1){
            velocidade = 0.7*0.01;
        }else{
            velocidade = 0.01;
        }
        if(keyStates['w'] || keyStates['W'] ){ posicao.y += velocidade; }
        if(keyStates['s'] || keyStates['S'] ){ posicao.y -= velocidade; }
        if(keyStates['a'] || keyStates['A'] ){ posicao.x -= velocidade; }
        if(keyStates['d'] || keyStates['D'] ){ posicao.x += velocidade; }
    }

    void rotate(int x, int y){
        y = -y + 350;
        x =  x - 350;
        if(x != 0 && y !=0){
            inclinacao = (-(GLfloat)atan2(x-posicao.x,y-posicao.y)/3.1415*180.0)+90;
        } 
    }

    void render(){
        int rand_offset = (rand() % 5  ) - 2.5;
        glPushMatrix();
        glTranslatef(posicao.x, posicao.y, 0);
        glRotatef(inclinacao, 0, 0, 1);
        if (tempo%8 < 3 && senta_o_dedo){
            glPushMatrix();
            glRotatef(rand_offset-90, 0, 0, 1);
            glBegin(GL_POLYGON);
            glColor4f(1, 1, 0, 1);
            glVertex2f( +0.8, -0.0);
            glVertex2f( +0.8, +100);
            glVertex2f( +1.0, +100);
            glVertex2f( +1.0, -0.0);
            glEnd();
            glPopMatrix();
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
        glColor3f(0, 0, 0);
        for(int i = 0; i < 30; i++){
            float theta = 2.0f * 3.1415926f * float(i) / float(30);
            float cx = 0.5 * cosf(theta);
            float cy = 0.5 * sinf(theta);
            glVertex2f(cx + 0, cy + 0);
        }
        glEnd();
        glBegin(GL_LINE_LOOP);
        glColor3f(0.3, 0, 0);
        for(int i = 0; i < 30; i++){
            float theta = 2.0f * 3.1415926f * float(i) / float(30);
            float cx = 0.1 * cosf(theta);
            float cy = 0.1 * sinf(theta);
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
Cor  BRANCO( 1.0,1.0,1.0);

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
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0, 0, -35);
    glRotatef(0, 1, 1, 1);

    // mira.render();
    player.render();

    // texto.render();

    glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
    glutPostRedisplay();

}

void Refresh(){
    tempo++;
    player.caminha();
    if (tempo == 100)
        tempo = 0;
}

void mouseClicks(int button, int state, int x, int y){
    player.rotate(x, y);
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
    mira.posicao.x=x;
    mira.posicao.y=y;
    player.rotate(x, y);
    y = -y + 350;
    x = x - 350;
    char temp[100];
    sprintf(temp, " (%d, %d)  %f", x, y, player.inclinacao);
    texto.set_texto(temp);
}

void atirando(int x, int y){
    player.rotate(x, y);
    mira.posicao.x=x;
    mira.posicao.y=y;
}

void loadTextures(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // glEnable(GL_DEPTH_TEST);
    // The following two lines enable semi transparent
    // glEnable(GL_BLEND);
    // glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    int width, height;
    bool hasAlpha;
    char idle_shotgun_0[] = "./Top_Down_Survivor/shotgun/idle/survivor-idle_shotgun_0.png";
    bool success = loadPngImage(idle_shotgun_0, width, height, hasAlpha, &textureImage[0]);
    
    std::cout << "Image loaded " << width << " " << height << " alpha " << hasAlpha << std::endl;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, width, height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureImage[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
}

void myReshape(GLsizei w, GLsizei h){ 
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0 * (GLfloat) w / (GLfloat) h, 1.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
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
    glutInit(&argc, argv);                      // Inicializa GLUT
    glutInitWindowSize(1300, 700);               // Inicializa tamanho da janela
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("PNG texture");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(Render);
    glutIdleFunc(Refresh);                      // Seta função de atualização
    loadTextures();
    initKeyboard();                             // Inicializa teclado
    initMouse();                                // Inicializa mouse
    std::cout << "Use mouse drag to rotate." << std::endl;
    glutMainLoop();
    return 0;
}