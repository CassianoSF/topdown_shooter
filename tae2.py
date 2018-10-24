from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

def display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    glOrtho( 0, 640, 0, 480, -10, 10)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

if __name__ == '__main__':
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH)
    glutInitWindowSize(640,480)
    glutCreateWindow("Hello World :'D")

    glutDisplayFunc(display)
    glutIdleFunc(display)
    glutMainLoop()