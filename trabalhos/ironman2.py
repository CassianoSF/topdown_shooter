import pygame
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *


window = 0
width, height = 500, 500

def base():
   glBegin(GL_POLYGON)
   glColor3f(1.0, 1.0, 1.0) 
   glVertex(0,  0 )
   glVertex(0,  10)
   glVertex(10, 0 )
   glVertex(10, 10)
   glEnd()

def draw():
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
   glLoadIdentity()
   refresh2d(height, width

   base()
   glutSwapBuffers()


glutInit()

glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH)

glutInitWindowSize(width, height)

glutInitWindowPosition(0, 0)

window = glutCreateWindow("OPENGL CG"

glutDisplayFunc(draw)

glutIdleFunc(draw)

glutMainLoop() 

draw()







