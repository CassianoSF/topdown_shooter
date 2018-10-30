import pygame
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

window = 0
width, height = 500, 500

def refresh2d(width, height):
   glViewport(0, 0, width, height)
   glMatrixMode(GL_PROJECTION)
   glLoadIdentity()
   glOrtho(0.0, width, 0.0, height, 0.0, 1.0)
   glMatrixMode (GL_MODELVIEW)
   glLoadIdentity()

def base():
   glBegin(GL_POLYGON)
   glColor3f(1, 1, 1) 
   glVertex(10,10)
   glVertex(10,20)
   glVertex(20,10)
   glVertex(20,20)
   glEnd()

def draw():
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
   glLoadIdentity()
   refresh2d(height, width)
   base()
   glutSwapBuffers()


glutInit()
glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH)
glutInitWindowSize(width, height)
glutInitWindowPosition(0, 0)
window = glutCreateWindow("OPENGL CG")
glutDisplayFunc(draw)
glutIdleFunc(draw)
glutMainLoop()                      
draw()