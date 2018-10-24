


#include <GL/glut.h>  
#include <stdio.h>  

double posicao_huck_x = 0;
double posicao_huck_y = 0;
double posicao_pedra_x = -10;
double posicao_pedra_y = -10;
double angulo_pedra = 10;
GLfloat win;
GLint view_w, view_h;
GLfloat r, g, b;

void cursorMouse(int x, int y){
    sprintf(conteudo_texto, "(%d, %d)", x, y)
    glutPostRedisplay();
}

 void texto(char *string){
    glPushMatrix();
    glRasterPos2f(-win, win-(win*0.08));
    while(*string)
        glutBitmapCaracter(GLUT_BITMAP_TIMES_ROMAN_10, *string++);
    glPopMatrix();
 }

void AlterarCor(int cor){
    switch(cor){
        case 1:
            r = 1.0f;
            g = 0.0f;
            b = 0.0f;
            break;
        case 2:
            r = 0.0f;
            g = 1.0f;
            b = 0.0f;
            break;
        case 3:
            r = 0.0f;
            g = 0.0f;
            b = 1.0f;
            break;

    }
    glutPostRedisplay();
}

void Criarmenu(){
    int menu, submenu;
    submenu = glutCreateMenu(AlterarCor);
    glutAddMenuEntry("Vermelho", 1);
    glutAddMenuEntry("Verde", 2);
    glutAddMenuEntry("Azul", 3);

    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Cor", submenu);

    glutAtachMenu(GLUT_RIGHT_BUTTON);
}

void MenuPrincipal(int opcao){

}

void Mouse(int btn, int state, int x, int y){
    if(button == GLUT_RIGHT_BUTTON)
        if(state == GLUT_DOWN)
            Criarmenu();
}

void Desenha(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
                   
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    glTranslatef(posicao_huck_x, posicao_huck_y, 0);
    glRotated(angulo_pedra,0, 0 ,1);
    
    glBegin(GL_QUADS);
        glColor3f(r,g,b);
        glVertex2f(-25.0, -25.0);
        glVertex2f(-25.0, 25.0);
        glVertex2f(25.0, 25.0);
        glVertex2f(25.0, -25.0);    
    glEnd();
    glPopMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    texto(conteudo_texto);
        //glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void inicializa() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Inicializar a cor de fundo da tela
    win=150.0f;
    r = 1.0f;
    g = 1.0f;
    b = 1.0f;
    strcpy(conteudo_texto, "(0,0)");
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{ 
    // Especifica as dimensões da Viewport
    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);
    view_w = w;
    view_h = h;                   

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (-win, win, -win, win);
}
void atualizar(){
    angulo_pedra +=0.01;
    glutPostRedisplay();
}
void teclado(unsigned char tecla, int x, int y){
    switch(tecla){
        case 'w':
            posicao_huck_y += 1;
            break;
        case 's':
            posicao_huck_y -= 1;
        break;
        case 'd':
            posicao_huck_x += 1;
            break;
        case 'a':
            posicao_huck_x -= 1;
        break;
    }
}
void TeclasEspeciais(int key, int x, int y)
{
    if(key == GLUT_KEY_UP) {
           win -= 10;
           if (win < 10) win = 10;
           glMatrixMode(GL_PROJECTION);
           glLoadIdentity();
           gluOrtho2D (-win, win, -win, win);
    }
    if(key == GLUT_KEY_DOWN) {
           win += 10;
           if (win > 500) win = 500;
           glMatrixMode(GL_PROJECTION);
           glLoadIdentity();
           gluOrtho2D (-win, win, -win, win);
    }
    
    glutPostRedisplay();
}

// Programa Principal 
int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Habilita double buffer(geralmente utilizado com animação) mas pode ser SINGLE também
    glutInitWindowSize(350,300);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Exemplo de Menu e Exibição de Caracteres");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(TeclasEspeciais);
    glutMouseFunc(Mouse); 
    glutPassiveMotionFunc(cursorMouse)
    inicializa();
    glutIdleFunc(atualizar);
    // initGL(); // OpenGL initialization
    glutMainLoop(); // Chama a máquina de estados do OpenGL e processa todas as mensagens
    return 0;
}