




int* keyStates = new int[256];
int screen_w = 1000;
int screen_h = 700;
int game_time = 0;
int frame_time = 100;

//       Color(r, g, b, a)
Color    CINZA(0.5, 0.5, 0.5, 1.0);
Color VERMELHO(1.0, 0.0, 0.0, 1.0);
Color    VERDE(0.0, 1.0, 0.0, 1.0);
Color     AZUL(0.0, 0.0, 1.0, 1.0);
Color  AMARELO(1.0, 1.0, 0.0, 1.0);
Color    PRETO(0.0, 0.0, 0.0, 1.0);
Color  LARANJA(1.0, 0.6, 0.3, 1.0);
Color  BRANCO( 1.0, 1.0, 1.0, 1.0);
//          Arma(bool _shoot,  string _name, int _damage, int _cap, int _num, int _rate, int _reload_time, int _accuracy){
Arma flashlight( false,        "flashlight", 1,           40,       1,        1,         1,                1         );
Arma      knife( false,        "knife",      1,           20,       2,        1,         1,                1         );
Arma    handgun( true,         "handgun",    1,           30,       3,        10,        3000,             2         );
Arma    shotgun( true,         "shotgun",    1,           100,      4,        10,        7000,             6         );
Arma      rifle( true,         "rifle",      1,           40,       5,        2,         4000,             3         );

Text  texto("(0,0)", BRANCO);
Mira mira;

Texture textures[421];
Coordinate origem(0,0);
GameObject tiro(origem, textures[0], AMARELO, 100, 0.1, 0);
GameObject obstaculo_0(Coordinate(20,-40), textures[0], VERDE, 10, 10, 10);
GameObject obstaculo_1(Coordinate(-10,-30), textures[0], VERDE, 10, 10, 40);
GameObject obstaculo_2(Coordinate(-2,10), textures[0], VERDE, 10, 10, -210);

Player player(knife, 0.0f, 0.0f, 10.0f, 0, PRETO);


//################    ##################    ###################    ##################
//################    ##################    ###################    ##################
//################    ##################    ###################    ##################
//################    ##################    ###################    ##################



void myDisplay(void){
    // REDERIZAÇÃO
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glTranslatef(-player.pos.x, -player.pos.y, -35);
    glTranslatef(-player.pos.x, -player.pos.y, -35);
    glRotatef(0, 1, 1, 1);

    mira.render(player.pos.x, player.pos.y);
    texto.render(player.pos.x, player.pos.y);
    obstaculo_0.render();
    obstaculo_1.render();
    obstaculo_2.render();
    player.render(textures, game_time, frame_time);

    glFlush();  // Requisita que o buffer usado para as operações de renderização seja exibido na tela
    glutSwapBuffers();
    glutPostRedisplay();
}

void loadTextures(){
    glClearColor(0,0,0,1);
    // glDepthMask(GL_FALSE);

    std::string path = "./_player/";
    string path_string_array[421];
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
                cout << "Loading...  " << str_filename << endl;
                i++;
            }
        }
    }
}

void myIdle(){
    game_time++;
    player.update(game_time);
    player.caminha(keyStates);
    if (game_time == 421*frame_time)
        game_time = 0;
}

void mouseClicks(int button, int state, int x, int y){
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    player.rotate(x,y);
    if(!player.reload){
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            player.shoot = true;
            player.rand_offset = (rand() % player.arma.accuracy) - 2.5;
            player.shoot_left = 3 * frame_time;
        }else{
            player.shoot = false;
        }
        game_time = 0;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        player.attack = true;
        game_time = 0;
    }else{
        player.attack = false;
    }
    
}

void keyDown(unsigned char tecla, int x, int y){
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    keyStates[tecla] = 1;
    if(tecla == 'r' || tecla == 'R'){
        player.reload = true;
        player.reload_left = player.arma.reload_time;
        player.shoot = false;
        player.attack = false;
    }
    if(tecla == '1')
        player.arma = knife;
    if(tecla == '2')
        player.arma = handgun;
    if(tecla == '3')
        player.arma = shotgun;
    if(tecla == '4')
        player.arma = rifle;
    if(tecla == 'f' || tecla == 'F' )
        player.arma = flashlight;

    if (keyStates['a'] + keyStates['s'] + keyStates['d'] + keyStates['w'] + keyStates['A'] + keyStates['S'] + keyStates['D'] + keyStates['W']){
        player.idle = false;
    }else{
        player.run = false;
        player.walk = false;
        player.idle = true;
    }
}

void keyUp(unsigned char tecla, int x, int y){
    keyStates[tecla] = 0;
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    if (tecla == 'a' || tecla == 'A'){ keyStates['a'] = 0; keyStates['A'] = 0;}
    if (tecla == 's' || tecla == 'S'){ keyStates['s'] = 0; keyStates['S'] = 0;}
    if (tecla == 'd' || tecla == 'D'){ keyStates['d'] = 0; keyStates['D'] = 0;}
    if (tecla == 'w' || tecla == 'W'){ keyStates['w'] = 0; keyStates['W'] = 0;}
    if (keyStates['a'] + keyStates['s'] + keyStates['d'] + keyStates['w'] + keyStates['A'] + keyStates['S'] + keyStates['D'] + keyStates['W']){
        player.idle = false;
    }else{
        player.run = false;
        player.walk = false;
        player.idle = true;
    }

}

void cursormouse(int x, int y){
    player.run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
    mira.update(x,y);
    player.rotate(x,y);
    char temp[100];
    sprintf(temp, " (%d, %d)  %f", x, y, player.inclinacao);
    texto.set_texto(temp);
}

void atirando(int x, int y){
    mira.update(x,y);
    player.rotate(x, y);
}

void myReshape(GLsizei w, GLsizei h){ 
    glViewport(0, 0, screen_w, screen_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0 * (GLfloat) w / (GLfloat) h, 1.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
}

void initCallbacks(){
    glutReshapeFunc(myReshape);
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);                      // Seta função de updateção
}

void initKeyboard(){
    glutKeyboardFunc(keyDown);                  // Callback ao pressionar tecla
    glutKeyboardUpFunc(keyUp);                  // Callback ao soltar tecla
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);      // Desativa reperição de tecla
}

void initMouse(){
    glutPassiveMotionFunc(cursormouse);         // Callback do cursor movendo 
    glutMouseFunc(mouseClicks);                 // Callback do click
    glutMotionFunc(atirando);                   // Callback do drag
    glutSetCursor(GLUT_CURSOR_NONE);            // Esconde cursor
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc, argv);                       // Inicializa GLUT
    glutInitWindowSize(screen_w, screen_h);      // Inicializa tamanho da janela
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);  // Inicializa o display mode
    glutCreateWindow("PNG texture");             // Inicializa a janaela
    initCallbacks();                             // Inicializa callbacks
    loadTextures();                              // Inicializa textures
    initKeyboard();                              // Inicializa teclado
    initMouse();                                 // Inicializa mouse
    glutMainLoop();                              // Main loop
    return 0;                                    // Nunca retorna
}