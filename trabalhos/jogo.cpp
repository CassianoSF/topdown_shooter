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


// INICIALIZAÇÃO DOS OBJETOS E VARIÁVEIS
Cor    CINZA(0.5,0.5,0.5);
Cor VERMELHO(1.0,0.0,0.0);
Cor    VERDE(0.0,1.0,0.0);
Cor     AZUL(0.0,0.0,1.0);
Cor  AMARELO(1.0,1.0,0.0);
Cor    PRETO(0.0,0.0,0.0);
Cor  LARANJA(1.0,0.6,0.3);
GLfloat win = 250.0f;
int tempo = 0;
Texto  texto("(0,0)", PRETO);
Retangulo obstaculo1(-250.0f,       50.0f,  75.0f, 500.0f,  0.0f, LARANJA);
Retangulo obstaculo2(-50.0f,       -150.0f, 75.0f, 500.0f,  0.0f, LARANJA);
Retangulo caixa1(    -150.0f,       0.0f,   40.0f, 40.0f,  0.0f, VERMELHO);
Retangulo caixa2(     110.0f,      -60.0f,  40.0f, 40.0f,  0.0f, VERMELHO);
Retangulo caixa3(    -50.0f,        200.0f, 40.0f, 40.0f,  0.0f, VERMELHO);
Retangulo caixa4(     110.0f,       145.0f, 40.0f, 40.0f,  0.0f, VERMELHO);
Retangulo player(      110.0f,     -210.0f, 40.0f, 40.0f,  0.0f,    PRETO);
Retangulo objetivo(   -140.0f,      165.0f, 40.0f, 40.0f,  0.0f, AMARELO);

void Desenha(void){
    // REDERIZAÇÃO
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    glClear(GL_COLOR_BUFFER_BIT);
    obstaculo1.render();    
    obstaculo2.render();    
    caixa1.render();    
    caixa2.render();    
    caixa3.render();    
    caixa4.render();    
    player.render();    
    objetivo.render();    
    glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
    glutPostRedisplay();

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
    // FLAG PARA DEFINICAO DA DIREÇÃO
    tempo++;
    if(tempo > 800){
        tempo = 0;
    }

    //MOVIMENTAÇÃO DOS OBSTACULOS
    if (tempo<400){
        caixa1.caminha("RIGHT");
        caixa1.inclinacao += 1;
        caixa2.caminha("LEFT");
        caixa2.inclinacao += 1;
        caixa3.caminha("RIGHT");
        caixa3.inclinacao += 1;
        caixa4.caminha("LEFT");
        caixa4.inclinacao += 1;
    }
    else if (tempo>400){
        caixa1.caminha("LEFT");
        caixa1.inclinacao += 1;
        caixa2.caminha("RIGHT");
        caixa2.inclinacao += 1;
        caixa3.caminha("LEFT");
        caixa3.inclinacao += 1;
        caixa4.caminha("RIGHT");
        caixa4.inclinacao += 1;
    }


    if(
        player.colide(caixa1) ||
        player.colide(caixa2) ||
        player.colide(caixa3) ||
        player.colide(caixa4)
    ){
        // RESETA POSIÇÃO
        player.posicao.x = 210.0f;
        player.posicao.y = -210.0f;
    }

        // FICA VERDE SE GANHAR
    if (player.colide(objetivo)){
        player.cor.set(VERDE);
    }
}



void volta (string dir){
    if(
        player.colide(obstaculo1) || 
        player.colide(obstaculo2)
    ){
        player.caminha(dir);
        player.caminha(dir);
        player.caminha(dir);
    }
}

void teclado(unsigned char tecla, int x, int y){
    // VERIFICA COLISÕES    
        
        switch(tecla){
            // Controle do player

            case 'w':
                player.caminha("UP");
                player.caminha("UP");
                player.caminha("UP");
                volta("DOWN");
                break;
            case 's':
                player.caminha("DOWN");
                player.caminha("DOWN");
                player.caminha("DOWN");
                volta("UP");
                break;
            case 'a':
                player.caminha("LEFT");
                player.caminha("LEFT");
                player.caminha("LEFT");
                volta("RIGHT");
                break;
            case 'd':
                player.caminha("RIGHT");
                player.caminha("RIGHT");
                player.caminha("RIGHT");
                volta("LEFT");
                break;
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
    // glutSpecialFunc(TeclasEspeciais); 
    glutPassiveMotionFunc(cursormouse);
    glutIdleFunc(Atualizar);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    // Inicializar a cor de fundo da tela
    glutMainLoop();                          // Chama a máquina de estados do OpenGL e processa todas as mensagens
    return 0;
}