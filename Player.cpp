class Player {
    public:
    Coordinate pos;
    Color color;   
    int animation_frames;
    int reload_left;
    int shoot_left;
    float rand_offset, inclinacao;
    string animation, pernas;
    Arma arma;
    Arma inventory[5];

    bool shoot=false, 
         idle=true, 
         walk=false, 
         run=false, 
         reload=false, 
         attack=false;

    Player(Arma _arma, float x, float y, float r, float angle, Color c, Arma _inventory[5]){
        color.set(c);
        pos = Coordinate(x,y);
        inclinacao = angle;
        arma = _arma;
        for (int i = 0; i < 5; ++i){
            inventory[i] = _inventory[i];
        }
    }

    void hundleKeyDown(char key){
        run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT && walk);
        if(key == 'r' || key == 'R'){
            actionReload();
        }
        string itemKeys = "f1234F";
        for(int i=0 ; i<5 ; i++) {
            if(key == itemKeys[i]){
                if (i==5){actionChangeItem(0);}
                actionChangeItem(i);
            }
        }
        string moveKeys = "asdwASDW";
    }

    void actionChangeItem(int num){
        arma = inventory[num];
        reload = false;
    }

    void actionReload(){
        reload = true;
        reload_left = arma.reload_time;
        shoot = false;
        attack = false;
    }

    void update(int game_time){
        stringstream ss;

        if(attack){
            animation_frames = 15;
            ss << arma.name << "_" << "meleeattack";
        }else if (reload && arma.reload_time && reload_left){
            reload_left--;
            animation_frames = 15;
            ss << arma.name << "_" << "reload";
            reload = !(reload_left == 1);

        }else if (shoot && arma.shoot){
            // shoot_left--;
            animation_frames = 3;
            ss << arma.name << "_" << "shoot";
            // shoot = !(shoot_left == 1);

        }else if(walk || run){
            if (run){
                pernas = "feet_run";
            }else
                pernas = "feet_walk";

            animation_frames = 20;
            ss << arma.name << "_" << "move";
        }else if(idle){
            ss << arma.name << "_" << "idle";
            animation_frames = 20;
        }
        animation = ss.str();
    }

    void caminha(int* keyStates){
        float speed_mult = 0.01;
        if (run)
            speed_mult = 0.017;
        float velocidade;
        int move = (keyStates['w'] + keyStates['s'] + keyStates['a'] + keyStates['d'] +
                    keyStates['W'] + keyStates['S'] + keyStates['A'] + keyStates['D']);
        if(move){
            walk = true;
            idle = false;
        }else{
            walk = false;
            run = false;
            idle = true;
        }
        if (move > 1){
            velocidade = 0.7*speed_mult;
        }else{
            velocidade = speed_mult;
        }
        if(keyStates['w'] || keyStates['W'] ){ pos.y += velocidade; }
        if(keyStates['s'] || keyStates['S'] ){ pos.y -= velocidade; }
        if(keyStates['a'] || keyStates['A'] ){ pos.x -= velocidade; }
        if(keyStates['d'] || keyStates['D'] ){ pos.x += velocidade; }
    }

    void rotate(int x, int y){
        y = (-y + 350) * 35;
        x = ( x - 350) * 35;
        if(x != 0 && y !=0){
            inclinacao = (-(GLfloat)atan2(x-pos.x,y-pos.y)/3.1415*180.0)+90;
        } 
    }

    void renderTiro(Texture textures[421], int game_time, int frame_time){
        if (shoot && arma.shoot && !reload && game_time/arma.rate % (frame_time/10) == 0){
            // glPushMatrix();
            //     glRotatef(rand_offset-86, 0, 0, 1);
            //     glBegin(GL_POLYGON);
            //         glColor4f(1, 1, 0, 1);
            //         glVertex2f( +1.03, -0.0);
            //         glVertex2f( +1.03, +100);
            //         glVertex2f( +1.08, +100);
            //         glVertex2f( +1.08, -0.0);
            //     glEnd();
            // glPopMatrix();
            for (int i = 0; i < 421; ++i){
                if(textures[i].animation == "shoot_texture")
                    textures[i].render();
            }
            glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0);
                glVertex2f(  1.3,-3.1);
                glTexCoord2f(0.0, 1.0);
                glVertex2f(  1.3, 0.9);
                glTexCoord2f(1.0, 1.0);
                glVertex2f(  5.3, 0.9);
                glTexCoord2f(1.0, 0.0);
                glVertex2f(  5.3,-3.1);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
    }

    void renderBody(Texture textures[421], int game_time, int frame_time){
        Texture texture;
        for (int i = 0; i < 421; ++i){
            if(textures[i].animation == animation && textures[i].index == (game_time / frame_time) % animation_frames)
                textures[i].render();
        }
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex2f( -2.0,-2.0);
            glTexCoord2f(0.0, 1.0);
            glVertex2f( -2.0, 2.0);
            glTexCoord2f(1.0, 1.0);
            glVertex2f(  2.0, 2.0);
            glTexCoord2f(1.0, 0.0);
            glVertex2f(  2.0,-2.0);
            glEnd();
        glDisable(GL_TEXTURE_2D);

    }

    void renderLegs(Texture textures[421], int game_time, int frame_time){
        glPushMatrix();
            glTranslatef(-0.3, -0.2, -10.0);
            Texture texture;
            for (int i = 0; i < 421; ++i){
                if(idle && textures[i].animation == "feet_idle")
                    textures[i].render();
                else if(textures[i].animation == pernas && textures[i].index == (game_time / frame_time) % 20)
                    textures[i].render();
            }
            glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0);
                glVertex2f( -2.0,-2.0);
                glTexCoord2f(0.0, 1.0);
                glVertex2f( -2.0, 2.0);
                glTexCoord2f(1.0, 1.0);
                glVertex2f(  2.0, 2.0);
                glTexCoord2f(1.0, 0.0);
                glVertex2f(  2.0,-2.0);
                glEnd();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    void render(Texture textures[421], int game_time, int frame_time){
        glPushMatrix();
            glTranslatef(pos.x, pos.y, 0);
            glRotatef(inclinacao, 0, 0, 1);
            renderLegs(textures, game_time, frame_time);
            renderTiro(textures, game_time, frame_time);
            renderBody(textures, game_time, frame_time);
        glPopMatrix();
    }
};
