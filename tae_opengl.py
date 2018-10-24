# -*- coding: utf-8 -*-

# Cassiano Surdi Franco

# A máquina de estado do OpenGL possui multiplas formas de ser alterada. 
# A cor atual é uma variável de estado. Ao definir a cor atual como branco, vermelho ou 
# qualquer outra cor, cada objeto é desenhado com essa cor até que você defina outra cor atual. 
# A cor atual é apenas uma das muitas variáveis de estado que o OpenGL mantém. 
# Outros controlam coisas como as transformações atuais de visualização e projeção, 
# padrões de linhas e padrões de polígono, modos de desenho de polígonos, 
# convenções de embalagem de pixels, posições e características de luzes e propriedades de 
# materiais dos objetos sendo desenhados. 
# Cada uma dessas aptidões do openGL podem ser ativadas ou desativadas com o comando 
# glEnable() ou glDisable() e o parametro da aptidão.
# são elas:

# GL_ALPHA_TEST
# GL_BLEND
# GL_COLOR_LOGIC_OP
# GL_CLIP_PLANEi
# GL_COLOR_MATERIAL
# GL_CULL_FACE
# GL_DEPTH_TEST
# GL_DITHER
# GL_FOG
# GL_LIGHTi
# GL_LIGHTING
# GL_LINE_SMOOTH
# GL_MULTISAMPLE
# GL_NORMALIZEGL_POINT_SMOOTH
# GL_POINT_SPRITE_OES
# GL_POLYGON_OFFSET_FILL
# GL_RESCALE_NORMAL
# GL_SAMPLE_ALPHA_TO_COVERAGE
# GL_SAMPLE_ALPHA_TO_ONE
# GL_SAMPLE_COVERAGE
# GL_SCISSOR_TEST
# GL_STENCIL_TEST
# GL_TEXTURE_2D

# A maioria dessas aptidões são configuradas por funções especificas, por exemplo:
# GL_BLEND mescla os valores de cores de fragmentos calculados com os valores nos buffers de cores
# conforme os parametros passados para a função glBlendFunc anteriormente.

# Cada variável ou modo de estado tem um valor padrão e, a qualquer momento, você pode consultar
# o sistema para o valor atual de cada variável. Normalmente, você usa um dos seis seguintes comandos 
# para fazer isso: glGetBooleanv(), glGetDoublev(), glGetFloatv(), glGetIntegerv(), glGetPointerv() 
# ou glIsEnabled(). Qual desses comandos você seleciona depende de qual tipo de dados você deseja que 
# a resposta seja fornecida.

# Todas essas informações tem como base o site oficial: https://www.khronos.org/

# Importando dependência
import pygame
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *


# Vertices do cubo
vertices = (
    (1, -1, -1),
    (1, 1, -1),
    (-1, 1, -1),
    (-1, -1, -1),
    (1, -1, 1),
    (1, 1, 1),
    (-1, -1, 1),
    (-1, 1, 1)
    )

# Arestas do cubo
arestas = (
    (0,1),
    (0,3),
    (0,4),
    (2,1),
    (2,3),
    (2,7),
    (6,3),
    (6,4),
    (6,7),
    (5,1),
    (5,4),
    (5,7)
    )


def Cubo(x):
    # Largura das linhas para 4px
    glLineWidth(4)

    # A cada 10 transformações de rotação entra em um dos ifs
    if (x % 10 == 0):
        # Cor Verde
        glColor3f(0.0, 1.0, 0.0)
        # Habilita renderização com filtro
        glEnable(GL_LINE_SMOOTH)
        # Habilita a mescla dos valores de cores de fragmentos calculados com os 
        # valores nos buffers de cores(filtro aritimetico definido anteriormente)
        glEnable(GL_BLEND)
    if ((x+10) % 20 == 0):
        # Cor Vermelha
        glColor3f(1.0, 0.0, 0.0)
        # Desabilita renderização com filtro
        glDisable(GL_LINE_SMOOTH)
        # Desabilita a mescla dos valores de cores de fragmentos calculados com os 
        # valores nos buffers de cores(filtro aritimetico definido anteriormente)
        glDisable(GL_BLEND)
    

    # Renderiza o cubo com base nos vertices e arestas
    glBegin(GL_LINES)
    for aresta in arestas:
        for vertice in aresta:
            glVertex3fv(vertices[vertice])
    glEnd()


def main():
    # inicia a aplicação multimidia
    pygame.init()

    # tamanho da janela 800x600
    display = (800,600)

    # inicializa a janela com renderização OpenGL
    pygame.display.set_mode(display, DOUBLEBUF|OPENGL)

    # configura a perspectiva da janela
    # parametros: 
    #     -angulo de visão
    #     -apecto x/y
    #     -distancia visivel mais proxima
    #     -distancia visivel mais longe
    gluPerspective(45, (display[0]/display[1]), 0.1, 50.0)

    # Multiplica a matriz por uma matriz de translação (x=0, y=0, z=-6) afastando o conteúdo
    glTranslatef(0.0,0.0, -6)

    # glBlendFunc é uma funcão que especifica a aritimética dos pixels com base nos parametros
    # (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) são parametros uteis para renderizar de forma suavizado
    # não 'serrilhando' os pontos e linha (anti aliasing)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

    # for de 0 a 1000
    for x in range(0, 1000):
        # Fecha a janela se apertar em fechar
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        # Multiplica a matriz por uma matriz de rotação 
        glRotatef(1, 3, 1, 1)

        # Limpa o buffer dos valores atuais (prepara para limpar a tela)        
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)

        # Renderiza o cubo
        Cubo(x)

        # Atualiza o conteudo do display com o cubo renderizado
        pygame.display.flip()

        # Espera 100ms
        pygame.time.wait(10)

# Chama função main
main()




# Python: https://www.python.org/downloads/
# para executar no terminal:
# > python local/do/arquivo/tae_opengl.py