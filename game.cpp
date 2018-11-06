
int* keyStates = new int[256]; // Estado das teclas 0 e 1
int screen_w = 1000;           // largura da tela
int screen_h = 700;            // altura da tela
int game_clock = 0;            // incremento do updateGame
int frame_time = 100;          // divisor do clock (frame window)
int time_flag = 0;             // flag de atualização


// MODELS


//       Color(r, g, b, a)
Color    CINZA(0.5, 0.5, 0.5, 1.0);
Color VERMELHO(1.0, 0.0, 0.0, 1.0);
Color    VERDE(0.0, 1.0, 0.0, 1.0);
Color     AZUL(0.0, 0.0, 1.0, 1.0);
Color  AMARELO(1.0, 1.0, 0.0, 1.0);
Color    PRETO(0.0, 0.0, 0.0, 1.0);
Color  LARANJA(1.0, 0.6, 0.3, 1.0);
Color  BRANCO( 1.0, 1.0, 1.0, 1.0);
//         Arma( bool _shoot,  string _name, int _damage, int _cap, int _num, int _rate, int _reload_time, int _accuracy){
Arma flashlight( false,        "flashlight", 1,           40,       1,        1,         1,                1         );
Arma      knife( false,        "knife",      1,           20,       2,        1,         1,                1         );
Arma    handgun( true,         "handgun",    1,           30,       3,        100,       3000,             2         );
Arma    shotgun( true,         "shotgun",    1,           100,      4,        200,       7000,             6         );
Arma      rifle( true,         "rifle",      1,           40,       5,        10,        4000,             3         );

Arma inventory[5] = { flashlight, knife, handgun, shotgun, rifle };

Text  texto("(0,0)", BRANCO);
Mira mira;

Texture textures[(421+43)];
Coordinate origem(0,0);
GameObject tiro(origem, textures[0], AMARELO, 100, 0.1, 0);
GameObject obstaculo_0(Coordinate(20,-40), textures[0], VERDE, 10, 10, 10);
GameObject obstaculo_1(Coordinate(-10,-30), textures[0], VERDE, 10, 10, 40);
GameObject obstaculo_2(Coordinate(-2,10), textures[0], VERDE, 10, 10, -210);

Animation player_animations[27];
Animation zombie_animations[3];

Player player(knife, 0.0f, 0.0f, 10.0f, 0, PRETO, inventory);
Zombie zombie(4,4,100,50);

//################    ##################    ###################    ##################
//################    ##################    ###################    ##################
//################    ##################    ###################    ##################
//################    ##################    ###################    ##################



// REDERIZAÇÃO
void renderGame(void){
    glLoadIdentity();                                     // Reseta matiz
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Limpa o buffer de cores
    glTranslatef(-player.pos.x, -player.pos.y, -35);      // Posiciona o senário deslocado da camera

    // Renderiza objetos
    mira.render(player.pos.x, player.pos.y);
    texto.render(player.pos.x, player.pos.y);
    obstaculo_0.render();
    obstaculo_1.render();
    obstaculo_2.render();
    player.render(game_clock, frame_time);
    zombie.render(textures, game_clock, frame_time);
    // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glFlush();  
    glutSwapBuffers();
    glutPostRedisplay();
}

void updateGame(){
    game_clock++;
    player.caminha(keyStates);
    player.update(game_clock, frame_time);
    zombie.update(game_clock, frame_time, player);
    if (game_clock == (421+43)*10000){
        game_clock = 0;
    }
    
    // if (time_flag != game_clock / frame_time){
    //     time_flag = game_clock / frame_time;
    //     cout << game_clock / frame_time << endl;
    // }
}

void keyDown(unsigned char key, int x, int y){
    keyStates[key] = 1;
    player.hundleKeyDown(key);
}

void keyUp(unsigned char key, int x, int y){
    keyStates[key] = 0;
    player.hundleKeyUp(keyStates, key);
}

void mouseClick(int button, int state, int x, int y){
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    player.rotate(x,y);
    player.hundleMouseClick(button, state);
}

void mouseMove(int x, int y){
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    player.rotate(x,y);
    mira.update(x,y);
    char temp[100];
    sprintf(temp, " (%d, %d)  %f", x, y, player.inclinacao);
    texto.set_texto(temp);
}

void mouseDrag(int x, int y){
    mira.update(x,y);
    player.rotate(x, y);
}

void loadAnimations(){
    for (int i = 0; i < 421+43; ++i){
        if(textures[i].animation == "shoot_texture"){   
            player_animations[SHOOT_TEXTURE].textures[textures[i].index] = textures[i];
            player_animations[SHOOT_TEXTURE].frames++;
            player_animations[SHOOT_TEXTURE].name = "shoot_texture";
        }
        else if(textures[i].animation == "flashlight_meleeattack"){   
            player_animations[FLASHLIGHT_MELEEATTACK].textures[textures[i].index] = textures[i];
            player_animations[FLASHLIGHT_MELEEATTACK].frames++;
            player_animations[FLASHLIGHT_MELEEATTACK].name = "flashlight_meleeattack";
        }
        else if(textures[i].animation == "flashlight_move"){   
            player_animations[FLASHLIGHT_MOVE].textures[textures[i].index] = textures[i];
            player_animations[FLASHLIGHT_MOVE].frames++;
            player_animations[FLASHLIGHT_MOVE].name = "flashlight_move";
        }
        else if(textures[i].animation == "flashlight_idle"){   
            player_animations[FLASHLIGHT_IDLE].textures[textures[i].index] = textures[i];
            player_animations[FLASHLIGHT_IDLE].frames++;
            player_animations[FLASHLIGHT_IDLE].name = "flashlight_idle";
        }
        else if(textures[i].animation == "feet_walk"){   
            player_animations[FEET_WALK].textures[textures[i].index] = textures[i];
            player_animations[FEET_WALK].frames++;
            player_animations[FEET_WALK].name = "feet_walk";
        }
        else if(textures[i].animation == "feet_strafe_left"){   
            player_animations[FEET_STRAFE_LEFT].textures[textures[i].index] = textures[i];
            player_animations[FEET_STRAFE_LEFT].frames++;
            player_animations[FEET_STRAFE_LEFT].name = "feet_strafe_left";
        }
        else if(textures[i].animation == "feet_strafe_right"){   
            player_animations[FEET_STRAFE_RIGHT].textures[textures[i].index] = textures[i];
            player_animations[FEET_STRAFE_RIGHT].frames++;
            player_animations[FEET_STRAFE_RIGHT].name = "feet_strafe_right";
        }
        else if(textures[i].animation == "feet_run"){   
            player_animations[FEET_RUN].textures[textures[i].index] = textures[i];
            player_animations[FEET_RUN].frames++;
            player_animations[FEET_RUN].name = "feet_run";
        }
        else if(textures[i].animation == "feet_idle"){   
            player_animations[FEET_IDLE].textures[textures[i].index] = textures[i];
            player_animations[FEET_IDLE].frames++;
            player_animations[FEET_IDLE].name = "feet_idle";
        }
        else if(textures[i].animation == "knife_meleeattack"){   
            player_animations[KNIFE_MELEEATTACK].textures[textures[i].index] = textures[i];
            player_animations[KNIFE_MELEEATTACK].frames++;
            player_animations[KNIFE_MELEEATTACK].name = "knife_meleeattack";
        }
        else if(textures[i].animation == "knife_move"){   
            player_animations[KNIFE_MOVE].textures[textures[i].index] = textures[i];
            player_animations[KNIFE_MOVE].frames++;
            player_animations[KNIFE_MOVE].name = "knife_move";
        }
        else if(textures[i].animation == "knife_idle"){   
            player_animations[KNIFE_IDLE].textures[textures[i].index] = textures[i];
            player_animations[KNIFE_IDLE].frames++;
            player_animations[KNIFE_IDLE].name = "knife_idle";
        }
        else if(textures[i].animation == "handgun_meleeattack"){   
            player_animations[HANDGUN_MELEEATTACK].textures[textures[i].index] = textures[i];
            player_animations[HANDGUN_MELEEATTACK].frames++;
            player_animations[HANDGUN_MELEEATTACK].name = "handgun_meleeattack";
        }
        else if(textures[i].animation == "handgun_move"){   
            player_animations[HANDGUN_MOVE].textures[textures[i].index] = textures[i];
            player_animations[HANDGUN_MOVE].frames++;
            player_animations[HANDGUN_MOVE].name = "handgun_move";
        }
        else if(textures[i].animation == "handgun_shoot"){   
            player_animations[HANDGUN_SHOOT].textures[textures[i].index] = textures[i];
            player_animations[HANDGUN_SHOOT].frames++;
            player_animations[HANDGUN_SHOOT].name = "handgun_shoot";
        }
        else if(textures[i].animation == "handgun_reload"){   
            player_animations[HANDGUN_RELOAD].textures[textures[i].index] = textures[i];
            player_animations[HANDGUN_RELOAD].frames++;
            player_animations[HANDGUN_RELOAD].name = "handgun_reload";
        }
        else if(textures[i].animation == "handgun_idle"){   
            player_animations[HANDGUN_IDLE].textures[textures[i].index] = textures[i];
            player_animations[HANDGUN_IDLE].frames++;
            player_animations[HANDGUN_IDLE].name = "handgun_idle";
        }
        else if(textures[i].animation == "rifle_meleeattack"){   
            player_animations[RIFLE_MELEEATTACK].textures[textures[i].index] = textures[i];
            player_animations[RIFLE_MELEEATTACK].frames++;
            player_animations[RIFLE_MELEEATTACK].name = "rifle_meleeattack";
        }
        else if(textures[i].animation == "rifle_move"){   
            player_animations[RIFLE_MOVE].textures[textures[i].index] = textures[i];
            player_animations[RIFLE_MOVE].frames++;
            player_animations[RIFLE_MOVE].name = "rifle_move";
        }
        else if(textures[i].animation == "rifle_shoot"){   
            player_animations[RIFLE_SHOOT].textures[textures[i].index] = textures[i];
            player_animations[RIFLE_SHOOT].frames++;
            player_animations[RIFLE_SHOOT].name = "rifle_shoot";
        }
        else if(textures[i].animation == "rifle_reload"){   
            player_animations[RIFLE_RELOAD].textures[textures[i].index] = textures[i];
            player_animations[RIFLE_RELOAD].frames++;
            player_animations[RIFLE_RELOAD].name = "rifle_reload";
        }
        else if(textures[i].animation == "rifle_idle"){   
            player_animations[RIFLE_IDLE].textures[textures[i].index] = textures[i];
            player_animations[RIFLE_IDLE].frames++;
            player_animations[RIFLE_IDLE].name = "rifle_idle";
        }
        else if(textures[i].animation == "shotgun_meleeattack"){   
            player_animations[SHOTGUN_MELEEATTACK].textures[textures[i].index] = textures[i];
            player_animations[SHOTGUN_MELEEATTACK].frames++;
            player_animations[SHOTGUN_MELEEATTACK].name = "shotgun_meleeattack";
        }
        else if(textures[i].animation == "shotgun_move"){   
            player_animations[SHOTGUN_MOVE].textures[textures[i].index] = textures[i];
            player_animations[SHOTGUN_MOVE].frames++;
            player_animations[SHOTGUN_MOVE].name = "shotgun_move";
        }
        else if(textures[i].animation == "shotgun_shoot"){   
            player_animations[SHOTGUN_SHOOT].textures[textures[i].index] = textures[i];
            player_animations[SHOTGUN_SHOOT].frames++;
            player_animations[SHOTGUN_SHOOT].name = "shotgun_shoot";
        }
        else if(textures[i].animation == "shotgun_reload"){   
            player_animations[SHOTGUN_RELOAD].textures[textures[i].index] = textures[i];
            player_animations[SHOTGUN_RELOAD].frames++;
            player_animations[SHOTGUN_RELOAD].name = "shotgun_reload";
        }
        else if(textures[i].animation == "shotgun_idle"){   
            player_animations[SHOTGUN_IDLE].textures[textures[i].index] = textures[i];
            player_animations[SHOTGUN_IDLE].frames++;
            player_animations[SHOTGUN_IDLE].name = "shotgun_idle";
        }



        else if(textures[i].animation == "skeleton_idle"){   
            zombie_animations[SKELETON_IDLE].textures[textures[i].index] = textures[i];
            zombie_animations[SKELETON_IDLE].frames++;
            zombie_animations[SKELETON_IDLE].name = "skeleton_idle";
        }
        else if(textures[i].animation == "skeleton_move"){   
            zombie_animations[SKELETON_MOVE].textures[textures[i].index] = textures[i];
            zombie_animations[SKELETON_MOVE].frames++;
            zombie_animations[SKELETON_MOVE].name = "skeleton_move";
        }
        else if(textures[i].animation == "skeleton_attack"){   
            zombie_animations[SKELETON_ATTACK].textures[textures[i].index] = textures[i];
            zombie_animations[SKELETON_ATTACK].frames++;
            zombie_animations[SKELETON_ATTACK].name = "skeleton_attack";
        }
    }
    player.setAnimations(player_animations);
}

void loadTextures(){
    glClearColor(0,0,0,1);
    std::string path = "./_player/";
    string path_string_array[(421+43)];
    string str_filename; 
    int i = 0;
    for (auto & p : fs::directory_iterator(path)){
        for (auto & pp : fs::directory_iterator(p)){
            for (auto & ppp : fs::directory_iterator(pp)){
                str_filename = ppp.path().string().c_str();
                path_string_array[i] = ppp.path().string().c_str();
                char *filename = new char[path_string_array[i].size()+1];
                strncpy(filename, path_string_array[i].c_str(), path_string_array[i].size());
                textures[i].data = png_texture_load(filename, NULL, NULL);

                smatch m;
                regex integer("(\\+|-)?[[:digit:]]+");
                regex_search(str_filename, m, regex("(\\+|-)?[[:digit:]]+"));
                for(auto v: m){
                    stringstream geek(v);
                    geek >> textures[i].index;
                }
                regex_search(str_filename, m, regex("shoot/shoot"));            for(auto v: m){textures[i].animation = "shoot_texture";} 
                regex_search(str_filename, m, regex("flashlight/meleeattack")); for(auto v: m){textures[i].animation = "flashlight_meleeattack";} 
                regex_search(str_filename, m, regex("flashlight/move"));        for(auto v: m){textures[i].animation = "flashlight_move";} 
                regex_search(str_filename, m, regex("flashlight/idle"));        for(auto v: m){textures[i].animation = "flashlight_idle";} 
                regex_search(str_filename, m, regex("feet/walk"));              for(auto v: m){textures[i].animation = "feet_walk";} 
                regex_search(str_filename, m, regex("feet/strafe_left"));       for(auto v: m){textures[i].animation = "feet_strafe_left";} 
                regex_search(str_filename, m, regex("feet/strafe_right"));      for(auto v: m){textures[i].animation = "feet_strafe_right";} 
                regex_search(str_filename, m, regex("feet/run"));               for(auto v: m){textures[i].animation = "feet_run";} 
                regex_search(str_filename, m, regex("feet/idle"));              for(auto v: m){textures[i].animation = "feet_idle";} 
                regex_search(str_filename, m, regex("knife/meleeattack"));      for(auto v: m){textures[i].animation = "knife_meleeattack";} 
                regex_search(str_filename, m, regex("knife/move"));             for(auto v: m){textures[i].animation = "knife_move";} 
                regex_search(str_filename, m, regex("knife/idle"));             for(auto v: m){textures[i].animation = "knife_idle";} 
                regex_search(str_filename, m, regex("handgun/meleeattack"));    for(auto v: m){textures[i].animation = "handgun_meleeattack";} 
                regex_search(str_filename, m, regex("handgun/move"));           for(auto v: m){textures[i].animation = "handgun_move";} 
                regex_search(str_filename, m, regex("handgun/shoot"));          for(auto v: m){textures[i].animation = "handgun_shoot";} 
                regex_search(str_filename, m, regex("handgun/reload"));         for(auto v: m){textures[i].animation = "handgun_reload";} 
                regex_search(str_filename, m, regex("handgun/idle"));           for(auto v: m){textures[i].animation = "handgun_idle";} 
                regex_search(str_filename, m, regex("rifle/meleeattack"));      for(auto v: m){textures[i].animation = "rifle_meleeattack";} 
                regex_search(str_filename, m, regex("rifle/move"));             for(auto v: m){textures[i].animation = "rifle_move";} 
                regex_search(str_filename, m, regex("rifle/shoot"));            for(auto v: m){textures[i].animation = "rifle_shoot";} 
                regex_search(str_filename, m, regex("rifle/reload"));           for(auto v: m){textures[i].animation = "rifle_reload";} 
                regex_search(str_filename, m, regex("rifle/idle"));             for(auto v: m){textures[i].animation = "rifle_idle";} 
                regex_search(str_filename, m, regex("shotgun/meleeattack"));    for(auto v: m){textures[i].animation = "shotgun_meleeattack";} 
                regex_search(str_filename, m, regex("shotgun/move"));           for(auto v: m){textures[i].animation = "shotgun_move";} 
                regex_search(str_filename, m, regex("shotgun/shoot"));          for(auto v: m){textures[i].animation = "shotgun_shoot";} 
                regex_search(str_filename, m, regex("shotgun/reload"));         for(auto v: m){textures[i].animation = "shotgun_reload";} 
                regex_search(str_filename, m, regex("shotgun/idle"));           for(auto v: m){textures[i].animation = "shotgun_idle";} 
                
                regex_search(str_filename, m, regex("skeleton-idle"));          for(auto v: m){textures[i].animation = "skeleton_idle";} 
                regex_search(str_filename, m, regex("skeleton-attack"));        for(auto v: m){textures[i].animation = "skeleton_attack";} 
                regex_search(str_filename, m, regex("skeleton-move"));          for(auto v: m){textures[i].animation = "skeleton_move";} 
                cout << "Loading...  " << str_filename << endl;
                i++;
            }
        }
    }
}

void reshapeGame(GLsizei w, GLsizei h){ 
    glViewport(0, 0, screen_w, screen_h);       // Normaliza tamanho da janela com o display
    glMatrixMode(GL_PROJECTION);                // ... ?
    glLoadIdentity();                           // Reseta matiz
    GLfloat aspect = (GLfloat) w / (GLfloat) h; // Proporção da tela
    gluPerspective(60.0, aspect, 1.0, 60.0);    // Austa proporção e perspectiva da camera
    glMatrixMode(GL_MODELVIEW);                 // ... ?
}

void initCallbacks(){
    glutReshapeFunc(reshapeGame);               // Seta reshapeGame como calback da ReshapeFunc
    glutDisplayFunc(renderGame);                // Seta renderGame como calback da DisplayFunc
    glutIdleFunc(updateGame);                   // Seta updateGame como calback da IdleFunc
}

void initKeyboard(){
    glutKeyboardFunc(keyDown);                  // Callback ao pressionar tecla
    glutKeyboardUpFunc(keyUp);                  // Callback ao soltar tecla
    // glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);      // Desativa reperição de tecla
}

void initMouse(){
    glutPassiveMotionFunc(mouseMove);            // Callback do cursor movendo 
    glutMouseFunc(mouseClick);                  // Callback do click
    glutMotionFunc(mouseDrag);                   // Callback do drag
    glutSetCursor(GLUT_CURSOR_NONE);             // Esconde cursor
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc, argv);                       // Inicializa GLUT
    glutInitWindowSize(screen_w, screen_h);      // Inicializa tamanho da janela
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH); // Inicializa o display mode
    glutCreateWindow("PNG texture");             // Inicializa a janaela
    initCallbacks();                             // Inicializa callbacks
    loadTextures();                              // Inicializa textures
    loadAnimations();                            // Inicializa animações
    initKeyboard();                              // Inicializa teclado
    initMouse();                                 // Inicializa mouse
    glutMainLoop();                              // Main loop
    return 0;                                    // Nunca retorna
}