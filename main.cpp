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
};

class Objeto{
public:
    Coordenada posicao;
    Cor cor;
    float altura, largura;
    
    Objeto(float x, float y, float alt, float larg, float r, float g, float b){
        cor.r = r;
        cor.g = g;
        cor.b = b;
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

    void set_cor(float r, float g, float b){
        cor.r = r;
        cor.g = g;
        cor.b = b;
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
    char conteudo_texto[20];
    Texto(char *conteudo){
        strcpy(conteudo_texto, conteudo);
    }

    void render(GLfloat win, char *string){
        glPushMatrix();
        glRasterPos2f(-win, win-(win*0.08));
        while(*string)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,*string++);
        glPopMatrix();
    }
};


GLfloat win;

Texto  texto("(0,0)");
Objeto player(0.0f,  0.0f, 20.0f, 20.0f, 1.0f, 1.0f, 1.0f); 
Objeto pedra(40.0f, 40.0f, 30.0f, 30.0f, 1.0f, 1.0f, 0.0f);

void Desenha(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    glClear(GL_COLOR_BUFFER_BIT);
    pedra.render();
    player.render();
    glColor3f(1.0f,1.0f,1.0f);
    texto.render(win);
    //glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void inicializa() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Inicializar a cor de fundo da tela
    win=150.0f;
    strcpy(conteudo_texto, "(0,0)");
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
        player.set_cor(0.5f,0.5f,0.5f);
    }else{
        player.set_cor(1.0f,0.0f,0.0f);
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
    //glutPassiveMotionFunc(cursormouse);
    inicializa();
    glutIdleFunc(atualizar);
    // initGL(); // OpenGL initialization
    glutMainLoop(); // Chama a máquina de estados do OpenGL e processa todas as mensagens
    return 0;
}