# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

posicao_huck_x = 0
posicao_huck_y = 0
posicao_pedra_x = -10
posicao_pedra_y = -10
angulo_pedra = 10
r = 0
g = 0
b = 1

def AlterarCor(cor):
    if(cor == 1):
        r = 1.0
        g = 0.0
        b = 0.0
    elif(cor == 2):
        r = 0.0
        g = 1.0
        b = 0.0
    elif(cor == 3):
        r = 0.0
        g = 0.0
        b = 1.0
    glutPostRedisplay()

def Criarmenu():
    submenu = glutCreateMenu(AlterarCor)
    glutAddMenuEntry("Vermelho", 1)
    glutAddMenuEntry("Verde", 2)
    glutAddMenuEntry("Azul", 3)

    menu = glutCreateMenu(MenuPrincipal)
    glutAddSubMenu("Cor", submenu)

    glutAtachMenu(GLUT_RIGHT_BUTTON)

def MenuPrincipal(opcao):
    pass

def Mouse(btn, state, x, y):
    if(button == GLUT_RIGHT_BUTTON):
        if(state == GLUT_DOWN):
            Criarmenu()

def Desenha():
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
                   
    glClear(GL_COLOR_BUFFER_BIT)
    glPushMatrix()

    glTranslatef(posicao_huck_x, posicao_huck_y, 0)
    glRotated(angulo_pedra,0, 0 ,1)
    
    glBegin(GL_QUADS)
    glColor3f(r,g,b)
    glVertex2f(-25.0, -25.0)
    glVertex2f(-25.0, 25.0)
    glVertex2f(25.0, 25.0)
    glVertex2f(25.0, -25.0)
    glEnd()
    glPopMatrix()
    
    glutSwapBuffers()

def inicializa():
    glClearColor(0.0, 0.0, 0.0, 1.0)
    win=150.0

def AlteraTamanhoJanela(w, h) :
    glViewport(0, 0, w, h)
    view_w = w
    view_h = h
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D (-win, win, -win, win)

def atualizar():
    global angulo_pedra
    angulo_pedra = angulo_pedra + 0.01
    glutPostRedisplay()

def teclado(tecla, x, y):
    if tecla == 'w':
        posicao_huck_y = posicao_huck_y + 1

    elif tecla == 's':
        posicao_huck_y -= 1

    elif tecla == 'd':
        posicao_huck_x = posicao_huck_x + 1

    elif tecla == 'a':
        posicao_huck_x -= 1

def TeclasEspeciais(key, x, y):
    if(key == GLUT_KEY_UP):
        win -= 10
        if (win < 10):
            win = 10
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluOrtho2D (-win, win, -win, win)

    if(key == GLUT_KEY_DOWN):
        win = win + 10
        if (win > 500):
            win = 500
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity()
        gluOrtho2D (-win, win, -win, win)
    
    glutPostRedisplay()

def AlterarCor(cor):
    if cor == 1:
        r = 1.0
        g = 0.0
        b = 0.0
    elif cor == 2:
        r = 0.0
        g = 1.0
        b = 0.0
    elif cor == 3:
        r = 0.0
        g = 0.0
        b = 1.0
    glutPostRedisplay()

def Criarmenu():
    submenu = glutCreateMenu(AlterarCor)
    glutAddMenuEntry("Vermelho", 1)
    glutAddMenuEntry("Verde", 2)
    glutAddMenuEntry("Azul", 3)

    menu = glutCreateMenu(submenu)
    glutAddSubMenu("Cor", submenu)

    glutAtachMenu(GLUT_RIGHT_BUTTON)

# def MenuPrincipal(opcao):
#   pass

def Mouse(btn, state, x, y):
    if(button == GLUT_RIGHT_BUTTON):
        if(state == GLUT_DOWN):
            Criarmenu()

def Desenha():
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
                   
    glClear(GL_COLOR_BUFFER_BIT)
    glPushMatrix()

    glTranslatef(posicao_huck_x, posicao_huck_y, 0)
    glRotated(angulo_pedra,0, 0 ,1)
    
    glBegin(GL_QUADS)
    glColor3f(r,g,b)
    glVertex2f(-25.0, -25.0)
    glVertex2f(-25.0, 25.0)
    glVertex2f(25.0, 25.0)
    glVertex2f(25.0, -25.0)
    glEnd()
    glPopMatrix()
    
    glutSwapBuffers()


def inicializa():
    glClearColor(0.0, 0.0, 0.0, 1.0)
    win=150.0

def AlteraTamanhoJanela(w, h): 
    glViewport(0, 0, w, h)
    view_w = w
    view_h = h

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D (-win, win, -win, win)

def atualizar():
    global angulo_pedra
    angulo_pedra = angulo_pedra + 0.01
    glutPostRedisplay()

def teclado(tecla, x, y):
    if tecla == 'w':
        posicao_huck_y = posicao_huck_y + 1

    elif tecla == 's':
        posicao_huck_y = posicao_huck_y - 1

    elif tecla == 'd':
        posicao_huck_x = posicao_huck_x + 1

    elif tecla == 'a':
        posicao_huck_x = posicao_huck_x - 1


def TeclasEspeciais(key, x, y):
    if(key == GLUT_KEY_UP):
        win -= 10
        if (win < 10):
            win = 10
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluOrtho2D (-win, win, -win, win)

    if(key == GLUT_KEY_DOWN):
        win = win + 10
        if (win > 500): 
            win = 500
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluOrtho2D (-win, win, -win, win)
    
    glutPostRedisplay()

def main():
    glutInit()
    glutInitDisplayMode(GLUT_DOUBLE)
    glutInitWindowSize(350,300)
    glutInitWindowPosition(10,10)
    glutCreateWindow("Exemplo de Menu e Exibição de Caracteres")
    glutDisplayFunc(Desenha)
    glutReshapeFunc(AlteraTamanhoJanela)
    glutKeyboardFunc(teclado)
    glutSpecialFunc(TeclasEspeciais)
    glutMouseFunc(Mouse)
    inicializa()
    glutIdleFunc(atualizar)
    glutMainLoop()

main()
