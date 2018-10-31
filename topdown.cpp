#include <GL/glut.h>  
#include <stdio.h>
#include <iostream>
#include <stdlib.h> 
#include <string>
#include <math.h>
#include <time.h>

using namespace std;



GLfloat win = 250.0f;
int tempo = 0;
bool senta_o_dedo = false;



class Coordenada {
public:
    float x, y;
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

class Retangulo {
public:
    Coordenada posicao;
    Cor cor;
    float altura, largura, inclinacao;
    
    Retangulo(float x, float y, float alt, float larg, float angulo, Cor c){
        cor.set(c);
        posicao.x = x;
        posicao.y = y;
        altura  = alt;
        largura = larg;
        inclinacao = angulo;
    }

    float top()  { return posicao.y - altura/2;  }
    float bot()  { return posicao.y + altura/2;  }
    float right(){ return posicao.x - largura/2; }
    float left() { return posicao.x + largura/2; }

    void caminha(string direcao, float velocidade){
        if(direcao == "UP")    { posicao.y += velocidade; }
        if(direcao == "DOWN")  { posicao.y -= velocidade; }
        if(direcao == "LEFT")  { posicao.x -= velocidade; }
        if(direcao == "RIGHT") { posicao.x += velocidade; }
    }

    bool colide(Retangulo obj){
        return(!((left()>obj.right() || right()<obj.left()) || 
          (bot()>obj.top()   ||   top()<obj.bot())));
    }

    void render(){
        glPushMatrix();
        glTranslatef(left(), bot(), 0.0f);
        glRotatef(inclinacao, 0, 0, 1);
        glBegin(GL_POLYGON);
        glColor3f(cor.r, cor.g, cor.b);
        glVertex2f(-largura/2, -altura/2);
        glVertex2f(-largura/2, altura/2);
        glVertex2f(largura/2,  altura/2);
        glVertex2f(largura/2, -altura/2);    
        glEnd();
        glPopMatrix();
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

class Player {
public:
    Coordenada posicao;
    Cor cor;   
    int num_segmentos;
    float raio, inclinacao;

    Player(float x, float y, float r, int seg, float angulo, Cor c){
        cor.set(c);
        posicao.x = x;
        posicao.y = y;
        num_segmentos=seg;
        inclinacao = angulo;
        raio = r;
    } 

    void caminha(string direcao){
        if(direcao == "UP")    { posicao.y += 1; }
        if(direcao == "DOWN")  { posicao.y -= 1; }
        if(direcao == "LEFT")  { posicao.x -= 1; }
        if(direcao == "RIGHT") { posicao.x += 1; }
    }

    void mira(int x, int y){
        y = -y + 350;
        x =  x - 350;
        if(x != 0 && y !=0){
            inclinacao = -(GLfloat)atan2(x,y)/3.1415*180.0;
        } 
    }

    void render(){

        int rand_offset = (rand() % 5  ) - 2.5;
        glPushMatrix();
        glRotatef(inclinacao, 0, 0, 1);

        if (tempo%8 < 4 && senta_o_dedo){
            glPushMatrix();
            glRotatef(rand_offset, 0, 0, 1);
            glBegin(GL_POLYGON);
            glColor3f(1, 1, 0);
            glVertex2f(posicao.x - 1 + 8, posicao.y + 0);
            glVertex2f(posicao.x - 1 + 8, posicao.y + 400);
            glVertex2f(posicao.x + 1 + 8, posicao.y + 400);
            glVertex2f(posicao.x + 1 + 8, posicao.y + 0);
            glEnd();
            glPopMatrix();
        }
        
        glBegin(GL_POLYGON);
        glColor3f(0, 0, 0);
        glVertex2f(posicao.x - 1.5 + 8, posicao.y + 0);
        glVertex2f(posicao.x - 1.5 + 8, posicao.y + 30);
        glVertex2f(posicao.x + 1.5 + 8, posicao.y + 30);
        glVertex2f(posicao.x + 1.5 + 8, posicao.y + 0);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0, 0.5, 0);
        for(int i = 0; i < num_segmentos; i++){
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segmentos);
            float cx = raio*2   * cosf(theta);
            float cy = raio*1.3 * sinf(theta);
            glVertex2f(cx + posicao.x, cy + posicao.y);
        }
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(cor.r, cor.g, cor.b);
        for(int i = 0; i < num_segmentos; i++){
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segmentos);
            float cx = raio * cosf(theta);
            float cy = raio * sinf(theta);
            glVertex2f(cx + posicao.x, cy + (posicao.y-3));
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

Texto  texto("(0,0)", PRETO);

Player player(0.0f, 0.0f, 10.0f, 100, 0, PRETO);

void Render(void){
    // REDERIZAÇÃO
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    glClear(GL_COLOR_BUFFER_BIT);
    player.render();
    texto.render();
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

void teclado(unsigned char tecla, int x, int y){
    switch(tecla){
        case 'w':
            // player.caminha("UP", 3);
            // volta("DOWN");
        break;
        case 's':
            // player.caminha("DOWN", 3);
            // volta("UP");
        break;
        case 'a':
            // player.caminha("LEFT", 3);
            // volta("RIGHT");
        break;
        case 'd':
            // player.caminha("RIGHT", 3);
            // volta("LEFT");
        break;
    }
    glutPostRedisplay();
}

void cursormouse(int x, int y){
    player.mira(x, y);
    y = -y + 350;
    x = x - 350;
    char temp[100];
    sprintf(temp, " (%d, %d)  %f", x, y, player.inclinacao);
    texto.set_texto(temp);
}

void mouseClicks(int button, int state, int x, int y){
    player.mira(x, y);
    if (state == GLUT_DOWN)
        senta_o_dedo = true;
    else
        senta_o_dedo = false;
}

void shooting(int x, int y){
    player.mira(x, y);
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc, argv);                   // Inicializa GLUT
    glutInitDisplayMode(GLUT_DOUBLE);        // Habilita double buffer(geralmente utilizado com animação) mas pode ser SINGLE também
    glutInitWindowSize(700, 700);            // Inicializa tamanho da janela
    glutInitWindowPosition(100,10);          // Posição inicial da janela na tela    
    glutCreateWindow("Exemplo Aula");        // Cria janela com titulo
    glutDisplayFunc(Render);                 // Seta função de renderização
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(teclado);
    // glutSpecialFunc(TeclasEspeciais); 
    glutPassiveMotionFunc(cursormouse);
    glutMouseFunc(mouseClicks);
    glutMotionFunc(shooting);           
    glutIdleFunc(Atualizar);                 // Seta função de atualização
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    // Inicializar a cor de fundo da tela
    glutMainLoop();                          // Chama a máquina de estados do OpenGL e processa todas as mensagens
    return 0;                                // nunca retorna
}