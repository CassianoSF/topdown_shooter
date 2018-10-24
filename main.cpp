#include <GL/glut.h>  
#include <stdio.h>
#include <iostream>
#include <stdlib.h> 
#include <cstring>
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

class Objeto{
public:
    Coordenada posicao;
    Cor cor;
    float altura, largura;
    
    Objeto(float x, float y, float alt, float larg, Cor c){
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

    bool colide(Objeto obj){
        return(!((left()>obj.right() || right()<obj.left()) || 
                  (bot()>obj.top()   ||   top()<obj.bot())));
    }

    void render(){
        glPushMatrix();
        glBegin(GL_QUADS);
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

Cor    CINZA(0.5,0.5,0.5);
Cor VERMELHO(1.0,0.0,0.0);
Cor    VERDE(0.0,1.0,0.0);
Cor     AZUL(0.0,0.0,1.0);
Cor  AMARELO(1.0,1.0,0.0);
Cor    PRETO(0.0,0.0,0.0);

GLfloat win = 250.0f;
Texto  texto("(0,0)", PRETO);
Objeto player(0.0f,  0.0f, 20.0f, 20.0f, AMARELO); 
Objeto pedra(40.0f, 40.0f, 30.0f, 30.0f, CINZA);

void Desenha(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    glClear(GL_COLOR_BUFFER_BIT);
    pedra.render();
    player.render();
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

void atualizar(){
    if(player.colide(pedra)){
        player.cor.set(VERMELHO);
    }else{
        player.cor.set(AMARELO);
    }
    glutPostRedisplay();
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

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Habilita double buffer(geralmente utilizado com animação) mas pode ser SINGLE também
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Exemplo Aula");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    // glutKeyboardFunc(teclado);
    glutSpecialFunc(TeclasEspeciais); 
    // glutPassiveMotionFunc(cursormouse);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Inicializar a cor de fundo da tela
    glutIdleFunc(atualizar);
    glutMainLoop(); // Chama a máquina de estados do OpenGL e processa todas as mensagens
    return 0;
}