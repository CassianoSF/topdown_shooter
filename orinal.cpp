#include <GL/glut.h>  
#include <stdio.h>
#include <iostream>
#include <stdlib.h> 


using namespace std;
double angulo_pedra = 10;
GLfloat win,r,g,b;
GLint view_w, view_h, posicao_huck_x = 0, posicao_huck_y = 0, posicao_pedra_x = 0, posicao_pedra_y = 0,  posicao_pedra_x_aux = 20, posicao_pedra_y_aux = 20;
char conteudo_texto[20];


void texto(char *string){
	glPushMatrix();
	glRasterPos2f(-win, win-(win*0.08));
	while(*string)
glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,*string++);
	glPopMatrix();
}
void Desenha(void)
{
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
                   
    glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(posicao_huck_x, posicao_huck_y, 0);
	//glRotated(angulo_pedra,0, 0 ,1);
	
	glBegin(GL_QUADS);
		glColor3f(r,g,b);
       	glVertex2f(0, 0);
       	glVertex2f(0, 25.0);
       	glVertex2f(25.0, 25.0);
       	glVertex2f(25.0, 0);    
	glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(posicao_pedra_x, posicao_pedra_y, 0);
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);
       	glVertex2f(40.0, 40.0);
       	glVertex2f(60.0, 40.0);
       	glVertex2f(60.0, 60.0);
       	glVertex2f(40.0, 60.0);    
	glEnd();
	glPopMatrix();
	
	glColor3f(1.0f,1.0f,1.0f);
	texto(conteudo_texto);
	
	   	//glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
 	glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void inicializa() {
   	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Inicializar a cor de fundo da tela
	win=150.0f;
	r = 1.0f;
	g = 0.0f;
	b = 0.0f;
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
		posicao_pedra_y += 1;
    }
    if(key == GLUT_KEY_DOWN) {
          posicao_pedra_y -= 1;
    }
    if(key == GLUT_KEY_LEFT) {
    	posicao_pedra_x -= 1;
    }
    if(key == GLUT_KEY_RIGHT) {
    	posicao_pedra_x += 1;
    }
    
    glutPostRedisplay();
}

int main(int argc, char** argv) {
	
	glutInit(&argc, argv); // Inicializa GLUT
   	glutInitDisplayMode(GLUT_DOUBLE); // Habilita double buffer(geralmente utilizado com animação) mas pode ser SINGLE também
    glutInitWindowSize(350,300);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Exemplo Aula");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
	glutKeyboardFunc(teclado);
	glutSpecialFunc(TeclasEspeciais); 
	//glutPassiveMotionFunc(cursormouse);
	
	inicializa();
	glutIdleFunc(atualizar);
	
	// initGL(); // OpenGL initialization
  	   glutMainLoop(); // Chama a máquina de estados do OpenGL e processa todas as mensagens
   	
	   return 0;
}