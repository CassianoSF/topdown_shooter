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
#include <vector>

#define          SHOOT_TEXTURE 0
#define FLASHLIGHT_MELEEATTACK 1
#define        FLASHLIGHT_MOVE 2
#define        FLASHLIGHT_IDLE 3
#define              FEET_WALK 4
#define       FEET_STRAFE_LEFT 5
#define      FEET_STRAFE_RIGHT 6
#define               FEET_RUN 7
#define              FEET_IDLE 8
#define      KNIFE_MELEEATTACK 9
#define             KNIFE_MOVE 10
#define             KNIFE_IDLE 11
#define    HANDGUN_MELEEATTACK 12
#define           HANDGUN_MOVE 13
#define          HANDGUN_SHOOT 14
#define         HANDGUN_RELOAD 15
#define           HANDGUN_IDLE 16
#define      RIFLE_MELEEATTACK 17
#define             RIFLE_MOVE 18
#define            RIFLE_SHOOT 19
#define           RIFLE_RELOAD 20
#define             RIFLE_IDLE 21
#define    SHOTGUN_MELEEATTACK 22
#define           SHOTGUN_MOVE 23
#define          SHOTGUN_SHOOT 24
#define         SHOTGUN_RELOAD 25
#define           SHOTGUN_IDLE 26

#define          SKELETON_IDLE 0
#define          SKELETON_MOVE 1
#define        SKELETON_ATTACK 2

namespace fs = std::experimental::filesystem;
using namespace std;

#include "load_png.cpp"
#include "Coordinate.cpp"
#include "Color.cpp"
#include "Texture.cpp"
#include "GameObject.cpp"
#include "Animation.cpp"
#include "Text.cpp"
#include "Arma.cpp"
#include "Mira.cpp"
#include "Player.cpp"
#include "Zombie.cpp"
#include "game.cpp"

