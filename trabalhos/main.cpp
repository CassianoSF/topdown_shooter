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
        glTranslatef(left(), bot(), 0.0f);
        glRotatef(inclinacao, 0, 0, 1);
        glBegin(GL_POLYGON);
        glColor3f(cor.r, cor.g, cor.b);
        glVertex2f(0, 0);
        glVertex2f(0, altura);
        glVertex2f(largura, altura);
        glVertex2f(largura, 0);    
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

int abre_e_fecha=0;

Texto  texto("(0,0)", PRETO);
Retangulo base(     -60.0f, -250.0f,  50.0f, 120.0f,  0.0f, LARANJA);
Retangulo braco1(   -10.0f,        -200.0f,       150.0f,  20.0f,  0.0f, LARANJA);
Retangulo braco2(    braco1.left(), braco1.top(), 100.0f,  20.0f,  0.0f, LARANJA);
Retangulo braco3(    braco2.left(), braco2.top(), 50.0f,   14.0f,  0.0f, LARANJA);
Retangulo garra1(   braco3.left(), braco3.top(),   20.0f,   4.0f,  0.0f, PRETO);
Retangulo garra2(   braco3.left() + 11, braco3.top(),   20.0f,   4.0f,  0.0f, PRETO);
Circulo eixo1(       0.0f,  -200.0f,  25.0f,    100, AMARELO);
Circulo eixo1_miolo(-0.0f,  -200.0f,   8.0f,    100, CINZA);
Circulo eixo2(       0.0f,  -50.0f,   22.0f,    100, AMARELO);
Circulo eixo2_miolo(-0.0f,  -50.0f,    7.0f,    100, CINZA);
Circulo eixo3(       0.0f,   50.0f,   20.0f,    100, AMARELO);
Circulo eixo3_miolo(-0.0f,   50.0f,    6.0f,    100, CINZA);
// Retangulo target = player;


void Desenha(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    glClear(GL_COLOR_BUFFER_BIT);    
    braco1.render();    
    base.render();    
    braco2.render();
    braco3.render();
    garra1.render();
    garra2.render();
    eixo1.render();
    eixo1_miolo.render();
    eixo2.render();
    eixo2_miolo.render();   
    eixo3.render();
    eixo3_miolo.render();
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
    // if(player.colide(pedra))
    //     player.cor.set(VERMELHO);
    // else
    //     player.cor.set(AMARELO);
    // glutPostRedisplay();
}


// Função de entrada do teclado que calcula as posições dos braços com base na inclinação
void teclado(unsigned char tecla, int x, int y){
    switch(tecla){
        // Controla o braço 3 para o sentido anti-horário
        case 'q':
            braco3.inclinacao += 1;
            garra1.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+78)*3.1415/180);
            garra1.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+78)*3.1415)/180);
            garra1.inclinacao = braco3.inclinacao;
            
            garra2.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+90)*3.1415/180);
            garra2.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+90)*3.1415)/180);
            garra2.inclinacao = braco3.inclinacao;
            break;
        // Controla o braço 3 para o sentido horário
        case 'w':
            braco3.inclinacao -= 1;
            garra1.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+78)*3.1415/180);
            garra1.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+78)*3.1415)/180);
            garra1.inclinacao = braco3.inclinacao;
            
            garra2.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+90)*3.1415/180);
            garra2.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+90)*3.1415)/180);
            garra2.inclinacao = braco3.inclinacao;
            break;
        // Controla o braço 2 para o sentido anti-horário
        case 'a':
            braco2.inclinacao += 1;
            braco3.inclinacao += 1;
            braco3.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+87)*3.1415/180);
            braco3.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+87)*3.1415)/180);
            eixo3.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+85)*3.1415/180);
            eixo3.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+85)*3.1415)/180);
            eixo3_miolo.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+85)*3.1415/180);
            eixo3_miolo.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+85)*3.1415)/180);
            garra1.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+78)*3.1415/180);
            garra1.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+78)*3.1415)/180);
            garra1.inclinacao = braco3.inclinacao;
            
            garra2.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+90)*3.1415/180);
            garra2.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+90)*3.1415)/180);
            garra2.inclinacao = braco3.inclinacao;
            break;
        // Controla o braço 2 para o sentido horário
        case 's':
            braco2.inclinacao -= 1;
            braco3.inclinacao -= 1;
            braco3.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+87)*3.1415/180);
            braco3.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+87)*3.1415)/180);
            eixo3.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+85)*3.1415/180);
            eixo3.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+85)*3.1415)/180);
            eixo3_miolo.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+85)*3.1415/180);
            eixo3_miolo.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+85)*3.1415)/180);
            garra1.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+78)*3.1415/180);
            garra1.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+78)*3.1415)/180);
            garra1.inclinacao = braco3.inclinacao;
            
            garra2.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+90)*3.1415/180);
            garra2.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+90)*3.1415)/180);
            garra2.inclinacao = braco3.inclinacao;
            break;
        // Controla o braço 3 para o sentido anti-horário
        case 'z':
            braco1.inclinacao += 1;
            braco2.inclinacao += 1;
            braco2.posicao.x = braco1.altura  * cosf((braco1.inclinacao+90)*3.1415/180) - 10;
            braco2.posicao.y = braco1.altura * sinf(((braco1.inclinacao+90)*3.1415)/180) - 200;
            eixo2.posicao.x = braco1.altura  * cosf((braco1.inclinacao+86)*3.1415/180) - 10;
            eixo2.posicao.y = braco1.altura * sinf(((braco1.inclinacao+86)*3.1415)/180) - 200;
            eixo2_miolo.posicao.x = braco1.altura  * cosf((braco1.inclinacao+86)*3.1415/180) - 10;
            eixo2_miolo.posicao.y = braco1.altura * sinf(((braco1.inclinacao+86)*3.1415)/180) - 200;
            braco3.inclinacao += 1;
            braco3.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+87)*3.1415/180);
            braco3.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+87)*3.1415)/180);
            eixo3.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+85)*3.1415/180);
            eixo3.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+85)*3.1415)/180);
            eixo3_miolo.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+85)*3.1415/180);
            eixo3_miolo.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+85)*3.1415)/180);
            garra1.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+78)*3.1415/180);
            garra1.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+78)*3.1415)/180);
            garra1.inclinacao = braco3.inclinacao;
            
            garra2.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+90)*3.1415/180);
            garra2.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+90)*3.1415)/180);
            garra2.inclinacao = braco3.inclinacao;
            break;
        case 'x':
        // Controla o braço 3 para o sentido horário
            braco1.inclinacao -= 1;
            braco2.inclinacao -= 1;
            braco2.posicao.x = braco1.altura  * cosf((braco1.inclinacao+90)*3.1415/180) - 10;
            braco2.posicao.y = braco1.altura * sinf(((braco1.inclinacao+90)*3.1415)/180) - 200;
            eixo2.posicao.x = braco1.altura  * cosf((braco1.inclinacao+86)*3.1415/180) - 10;
            eixo2.posicao.y = braco1.altura * sinf(((braco1.inclinacao+86)*3.1415)/180) - 200;
            eixo2_miolo.posicao.x = braco1.altura  * cosf((braco1.inclinacao+86)*3.1415/180) - 10;
            eixo2_miolo.posicao.y = braco1.altura * sinf(((braco1.inclinacao+86)*3.1415)/180) - 200;
            braco3.inclinacao -= 1;
            braco3.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+87)*3.1415/180);
            braco3.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+87)*3.1415)/180);
            eixo3.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+85)*3.1415/180);
            eixo3.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+85)*3.1415)/180);
            eixo3_miolo.posicao.x = braco2.posicao.x + braco2.altura  * cosf((braco2.inclinacao+85)*3.1415/180);
            eixo3_miolo.posicao.y = braco2.posicao.y + braco2.altura * sinf(((braco2.inclinacao+85)*3.1415)/180);
            
            garra1.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+78)*3.1415/180);
            garra1.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+78)*3.1415)/180);
            garra1.inclinacao = braco3.inclinacao;
            
            garra2.posicao.x = braco3.posicao.x + braco3.altura  * cosf((braco3.inclinacao+90)*3.1415/180);
            garra2.posicao.y = braco3.posicao.y + braco3.altura * sinf(((braco3.inclinacao+90)*3.1415)/180);
            garra2.inclinacao = braco3.inclinacao;
            break;
        case ' ':
        // Controla abrir e fechar da garra 
            if (abre_e_fecha < 10){
                abre_e_fecha +=1;
                garra1.posicao.x = garra1.posicao.x + (garra2.posicao.x - garra1.posicao.x)/10;
                garra1.posicao.y = garra1.posicao.y + (garra2.posicao.y - garra1.posicao.y)/10;
                garra2.posicao.x = garra2.posicao.x + (garra1.posicao.x - garra2.posicao.x)/10;
                garra2.posicao.y = garra2.posicao.y + (garra1.posicao.y - garra2.posicao.y)/10;
            } 
            else{
                abre_e_fecha +=1;
                garra1.posicao.x = garra1.posicao.x - (garra2.posicao.x - garra1.posicao.x)/10;
                garra1.posicao.y = garra1.posicao.y - (garra2.posicao.y - garra1.posicao.y)/10;
                garra2.posicao.x = garra2.posicao.x - (garra1.posicao.x - garra2.posicao.x)/10;
                garra2.posicao.y = garra2.posicao.y - (garra1.posicao.y - garra2.posicao.y)/10;
                if (abre_e_fecha > 20){
                    abre_e_fecha = 0;
                }
            }
            break;

    }
    glutPostRedisplay();
}

void TeclasEspeciais(int key, int x, int y){
    if(key == GLUT_KEY_UP) {
        // player.caminha("UP");
    }
    if(key == GLUT_KEY_DOWN) {
        // player.caminha("DOWN");
    }
    if(key == GLUT_KEY_LEFT) {
        // player.caminha("LEFT");
    }
    if(key == GLUT_KEY_RIGHT) {
        // player.caminha("RIGHT");
    }
    glutPostRedisplay();
}

void cursormouse(int x, int y){
    y = -y + 350;
    x = x - 350;
    char temp[20];
    sprintf(temp, " (%d, %d)", x, y);
    texto.set_texto(temp);
    player.inclinacao = atan((x/y))
        glutPostRedisplay();

}

void processMainMenu(int option){
    switch(option){
        case 1:
            braco1.cor.set(VERMELHO);
            break;
        case 2:
            braco1.cor.set(VERDE);
            break;
        case 3:
            braco1.cor.set(AZUL);
            break;
        case 4:
            braco2.cor.set(VERMELHO);
            break;
        case 5:
            braco2.cor.set(VERDE);
            break;
        case 6:
            braco2.cor.set(AZUL);
            break;
        case 7:
            braco3.cor.set(VERMELHO);
            break;
        case 8:
            braco3.cor.set(VERDE);
            break;
        case 9:
            braco3.cor.set(AZUL);
            break;
    }
    glutPostRedisplay();
}

void createPopupMenus(){ 
    int mainMenu = glutCreateMenu(processMainMenu); 
    glutAddMenuEntry("Braco 1 - Vermelho", 1);
    glutAddMenuEntry("Braco 1 - Verde",    2);
    glutAddMenuEntry("Braco 1 - Azul",     3);
    glutAddMenuEntry("Braco 2 - Vermelho", 4);
    glutAddMenuEntry("Braco 2 - Verde",    5);
    glutAddMenuEntry("Braco 2 - Azul",     6);
    glutAddMenuEntry("Braco 3 - Vermelho", 7);
    glutAddMenuEntry("Braco 3 - Verde",    8);
    glutAddMenuEntry("Braco 3 - Azul",     9);
    glutAttachMenu(GLUT_RIGHT_BUTTON); 
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);                   // Inicializa GLUT
    glutInitDisplayMode(GLUT_DOUBLE);        // Habilita double buffer(geralmente utilizado com animação) mas pode ser SINGLE também
    glutInitWindowSize(700, 700);            // Inicializa tamanho da janela
    glutInitWindowPosition(100,10);          // Posição inicial da janela na tela    
    glutCreateWindow("Exemplo Aula");        // Cria janela com titulo
    createPopupMenus();
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