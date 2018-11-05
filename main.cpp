#define GLFW_INCLUDE_ES2
#include <GL/glut.h>  
#include <stdio.h>
#include <stdlib.h> 
#include <sstream> 
#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include <png.h>
#include <GL/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <vector>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <experimental/filesystem>
#include <regex>

namespace fs = std::experimental::filesystem;
using namespace std;

#include "load_png.cpp"
#include "Coordinate.cpp"
#include "Color.cpp"
#include "Texture.cpp"
#include "Text.cpp"
#include "Arma.cpp"
#include "Mira.cpp"
#include "Player.cpp"
#include "GameObject.cpp"
#include "game.cpp"
