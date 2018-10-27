#include <GL/glut.h>  
#include <stdio.h>
#include <iostream>
#include <stdlib.h> 
#include <string>
#include <math.h>

using namespace std;

class Coordenada{
public:
    float x, y;
};

class Cor{
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

class Retangulo{
public:
    Coordenada posicao;
    Cor cor;
    float altura, largura;
    
    Retangulo(float x, float y, float alt, float larg, Cor c){
        cor.set(c);
        posicao.x = x;
        posicao.y = y;
        altura  = alt;
        largura = larg;
    }

    float top()  { return posicao.y + altura;  }
    float bot()  { return posicao.y;           }
    float right(){ return posicao.x + largura; }
    float left() { return posicao.x;           }

    void caminha(string direcao){
        if(direcao == "UP")    { posicao.y += 1; }
        if(direcao == "DOWN")  { posicao.y -= 1; }
        if(direcao == "LEFT")  { posicao.x -= 1; }
        if(direcao == "RIGHT") { posicao.x += 1; }
    }

    bool colide(Retangulo obj){
        return(!((left()>obj.right() || right()<obj.left()) || 
                  (bot()>obj.top()   ||   top()<obj.bot())));
    }

    void render(){
        glPushMatrix();
        glBegin(GL_POLYGON);
        glColor3f(cor.r, cor.g, cor.b);
        glVertex2f(left(), bot());
        glVertex2f(left(), top());
        glVertex2f(right(), top());
        glVertex2f(right(), bot());    
        glEnd();
        glPopMatrix();
    }
};

class Texto{
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


    void render(GLfloat win){
        glColor3f(cor.r, cor.g, cor.b);
        glPushMatrix();
        glRasterPos2f(-win, win-(win*0.08));
        for (int i = 0; i < sizeof conteudo_texto; ++i){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, conteudo_texto[i]);
        }
        glPopMatrix();
    }
};

class Circulo{
public:
    Coordenada posicao;
    Cor cor;   
    int num_segmentos;
    float raio;

    Circulo(float x, float y, float r, int seg, Cor c){
        cor.set(c);
        posicao.x = x;
        posicao.y = y;
        num_segmentos=seg;
        raio = r;
    } 

    void render(){
        glPushMatrix();
        glBegin(GL_POLYGON);
        glColor3f(cor.r, cor.g, cor.b);
        for(int i = 0; i < num_segmentos; i++){
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segmentos);
            float cx = raio * cosf(theta);
            float cy = raio * sinf(theta);
            glVertex2f(cx + posicao.x, cy + posicao.y);
        }
        glEnd();
        glPopMatrix();
    }
};

Cor    CINZA(0.5,0.5,0.5);
Cor VERMELHO(1.0,0.0,0.0);
Cor    VERDE(0.0,1.0,0.0);
Cor     AZUL(0.0,0.0,1.0);
Cor  AMARELO(1.0,1.0,0.0);
Cor    PRETO(0.0,0.0,0.0);
Cor  LARANJA(1.0,0.6,0.3);
GLfloat win = 250.0f;

Texto  texto("(0,0)", PRETO);
Retangulo player(           0.0f,    0.0f,  20.0f,  20.0f, AMARELO); 
Retangulo base(           -60.0f, -250.0f,  50.0f, 120.0f, LARANJA);
Retangulo base_contorno(  -62.0f, -252.0f,  54.0f, 124.0f, PRETO);
Retangulo pedra(           40.0f,   40.0f,  30.0f,  30.0f, CINZA);
Retangulo braco_1(         -10.0f,-200.0f, 100.0f,  40.0f, AMARELO);
Circulo eixo1(              0.0f, -200.0f,  20.0f,    100, AMARELO);
Circulo eixo1_contorno(    -0.0f, -200.0f,  22.0f,    100, PRETO);
Circulo eixo1_miolo(       -0.0f, -200.0f,   8.0f,    100, CINZA);

Retangulo target = player;


void Desenha(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    glClear(GL_COLOR_BUFFER_BIT);
    pedra.render();
    player.render();
    braco_1.render();
    base_contorno.render();
    base.render();
    eixo1_contorno.render();
    eixo1.render();
    eixo1_miolo.render();
    texto.render(win);
    glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
}



// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h){ 
    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);        

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (-win, win, -win, win);
}

void Atualizar(){
    if(player.colide(pedra))
        player.cor.set(VERMELHO);
    else
        player.cor.set(AMARELO);
    glutPostRedisplay();
}

void teclado(unsigned char tecla, int x, int y){
    switch(tecla){
        case 'w':
            // posicao_huck_x -= 1;
        break;
    }
}

void TeclasEspeciais(int key, int x, int y){
    if(key == GLUT_KEY_UP) {
        player.caminha("UP");
    }
    if(key == GLUT_KEY_DOWN) {
        player.caminha("DOWN");
    }
    if(key == GLUT_KEY_LEFT) {
        player.caminha("LEFT");
    }
    if(key == GLUT_KEY_RIGHT) {
        player.caminha("RIGHT");
    }
    glutPostRedisplay();
}

void cursormouse(int x, int y){
    y = -y + 350;
    x = x - 350;
    char temp[20];
    sprintf(temp, " (%d, %d)", x, y);
    texto.set_texto(temp);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);                   // Inicializa GLUT
    glutInitDisplayMode(GLUT_DOUBLE);        // Habilita double buffer(geralmente utilizado com animação) mas pode ser SINGLE também
    glutInitWindowSize(700, 700);            // Inicializa tamanho da janela
    glutInitWindowPosition(100,10);          // Posição inicial da janela na tela    
    glutCreateWindow("Exemplo Aula");        // Cria janela com titulo
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(TeclasEspeciais); 
    glutPassiveMotionFunc(cursormouse);
    glutIdleFunc(Atualizar);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    // Inicializar a cor de fundo da tela
    glutMainLoop();                          // Chama a máquina de estados do OpenGL e processa todas as mensagens
    return 0;
}