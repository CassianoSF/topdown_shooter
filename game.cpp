
int* keyStates = new int[256]; // Estado das teclas 0 e 1
int screen_w = 1000;           // largura da tela
int screen_h = 700;            // altura da tela
int game_clock = 0;            // incremento do updateGame
int frame_time = 50;          // divisor do clock (frame window)
int time_flag = 0;             // flag de atualização
int level = 1;

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
//         Arma( bool _shoot,  string _name, int _damage, int _cap,    int _num, int _rate, int _reload_time, int _accuracy){
Arma flashlight( false,        "flashlight", 1,           99999*9999,  1,        1,         1,                1         );
Arma      knife( false,        "knife",      1,           99999*9999,  2,        1,         1,                1         );
Arma    handgun( true,         "handgun",    10,          12*20,       3,        100,       10,               2         );
Arma    shotgun( true,         "shotgun",    50,          12*5,        4,        200,       40,               6         );
Arma      rifle( true,         "rifle",      10,          3*30,        5,        10,        20,               3         );

Arma inventory[5] = { flashlight, knife, handgun, shotgun, rifle };

Text  texto("", BRANCO);
Mira mira;

Texture textures[(421+43+3)];
Coordinate origem(0,0);
GameObject tiro(origem, textures[0], AMARELO, 100, 0.1, 0);
GameObject obstaculo_0(Coordinate(20,-40), textures[0], VERDE, 10, 10, 10);
GameObject obstaculo_1(Coordinate(-10,-30), textures[0], VERDE, 10, 10, 40);
GameObject obstaculo_2(Coordinate(-2,10), textures[0], VERDE, 10, 10, -210);
GameObject the_floor(origem, textures[0], BRANCO, 8000, 8000, 0);
GameObject blood(origem, textures[0], BRANCO, 5, 5, 0);

Animation player_animations[27];
Animation zombie_animations[3];

Player player(knife, 0.0f, 0.0f, 10.0f, 0, PRETO, inventory);


int zombies_size = 0;
Zombie* zombies = new Zombie[zombies_size];


//################    ##################    ###################    ##################
//################    ##################    ###################    ##################
//################    ##################    ###################    ##################
//################    ##################    ###################    ##################

void random_zombies(int _level){
    int temp_zombies_size = _level;
    Zombie* temp_zombies = new Zombie[temp_zombies_size];
    for (int i = 0; i < zombies_size; i++){
        temp_zombies[i] = zombies[i];
    }
    for (int i = zombies_size; i < temp_zombies_size; i++){
        temp_zombies[i].set(
            (rand() % 100)-50,
            (rand() % 100)-50,
            100,
            50,
            i,
            0.004 * _level
        );
    }
    delete [] zombies;
    zombies_size = temp_zombies_size;
    zombies = temp_zombies;
}


// REDERIZAÇÃO
void renderGame(void){
    glLoadIdentity();                                     // Reseta matiz
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Limpa o buffer de cores
    if(player.didShoot(game_clock, frame_time))
        glTranslatef(-player.pos.x+0.11, -player.pos.y+0.11, -35);      // Posiciona o senário deslocado da camera    
    else
        glTranslatef(-player.pos.x, -player.pos.y, -35);      // Posiciona o senário deslocado da camera
    the_floor.render_floor();
    // Renderiza objetos
    mira.render(player.pos.x, player.pos.y);
    texto.render(player.pos.x, player.pos.y);
    obstaculo_0.render();
    obstaculo_1.render();
    obstaculo_2.render();
    player.render(game_clock, frame_time);
    for (int i = 0; i < zombies_size; ++i){
        zombies[i].render(game_clock, frame_time, blood, zombie_animations);
    }

    // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glFlush();  
    glutSwapBuffers();
}

void updateGame(){
    texto.set_texto(to_string(player.arma.bullets/3));
    
    for (int i = 0; i < zombies_size; ++i){
        zombies[i].update(game_clock, frame_time, player, zombies);
    }
    game_clock++;
    player.caminha(keyStates, obstaculo_2);
    player.update(game_clock, frame_time);
    if (game_clock == (421+43+3)*10000){
        game_clock = 0;
    }


    // if (game_clock % frame_time == 0){
    //     time_flag ++;
    //     cout << time_flag << endl;
    // }
    if(game_clock == 1){
        level++;
        random_zombies(level);
    }
    
    if(game_clock%5000 == 0){
        level++;
        random_zombies(level);
    }
    
    glutPostRedisplay();    
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
    player.rotate(x,y);
    mira.update(x,y);
}

void mouseDrag(int x, int y){
    mira.update(x,y);
    player.rotate(x, y);
}

void loadAnimations(){
    for (int i = 0; i < 421+43+3; ++i){
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
    std::string path = "./textures/";
    string path_string_array[(421+43+3)];
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
                regex_search(str_filename, m, regex("the_floor/floor"));        for(auto v: m){the_floor.texture = textures[i];} 
                regex_search(str_filename, m, regex("the_floor/blood"));        for(auto v: m){blood.texture = textures[i];} 
                regex_search(str_filename, m, regex("/crate"));                 for(auto v: m){
                    obstaculo_0.texture = textures[i];
                    obstaculo_1.texture = textures[i];
                    obstaculo_2.texture = textures[i];
                } 
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