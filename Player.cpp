class Player {
    public:
    Coordinate pos;
    Color color;   
    int animation_frames;
    int reload_left;
    int shoot_left;
    float rand_offset, inclinacao;
    int animation, legs;
    Arma arma;
    Arma inventory[5];
    Animation animations[27];

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
        legs = FEET_IDLE;
        animation = KNIFE_IDLE;
        for (int i = 0; i < 5; ++i){
            inventory[i] = _inventory[i];
        }
    }

    void setAnimations(Animation _animations[27]){
        for (int i = 0; i < 27; ++i){
            animations[i] = _animations[i];
        }
    }

    void hundleKeyDown(char key){
        run = (glutGetModifiers() & GLUT_ACTIVE_SHIFT );
        if(key == 'r' || key == 'R'){
            actionReload();
        }
        string itemKeys = "f1234F";
        for(int i=0 ; i<5 ; i++) {
            if(key == itemKeys[i]){
                if (i==5){actionChangeItem(0);}
                else     {actionChangeItem(i);}
            }
        }
    }

    void hundleKeyUp(int keyStates[256], char key){
        if (key == 'a' || key == 'A'){ keyStates['a'] = 0; keyStates['A'] = 0;}
        if (key == 's' || key == 'S'){ keyStates['s'] = 0; keyStates['S'] = 0;}
        if (key == 'd' || key == 'D'){ keyStates['d'] = 0; keyStates['D'] = 0;}
        if (key == 'w' || key == 'W'){ keyStates['w'] = 0; keyStates['W'] = 0;}
        if (keyStates['a'] + keyStates['s'] + keyStates['d'] + keyStates['w'] + keyStates['A'] + keyStates['S'] + keyStates['D'] + keyStates['W']){
            idle = false;
        }else{
            run = false;
            walk = false;
            idle = true;
        }
    }
    
    void hundleMouseClick(int button, int state){
        if(!reload && arma.shoot){
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                actionShoot();
            }else{
                shoot = false;
            }
        }
        if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
            attack = true;
        }else{
            attack = false;
        }
    }

    void actionShoot(){
        shoot = true;
        rand_offset = (rand() % arma.accuracy) - 2.5;
    }

    void actionChangeItem(int num){
        arma = inventory[num];
        shoot = false;
        attack = false;
        reload = false;
    }

    void actionReload(){
        reload_left = arma.reload_time;
        reload = true;
        shoot = false;
        attack = false;
    }

    void update(int game_time, int frame_time){
        if(attack){
            if(arma.name == "flashlight"){ animation = FLASHLIGHT_MELEEATTACK; }
            if(arma.name == "knife")     { animation = KNIFE_MELEEATTACK;      }
            if(arma.name == "handgun")   { animation = HANDGUN_MELEEATTACK;    }
            if(arma.name == "shotgun")   { animation = SHOTGUN_MELEEATTACK;    }
            if(arma.name == "rifle")     { animation = RIFLE_MELEEATTACK;      }
        }else if (reload && arma.reload_time && reload_left){
            if(arma.name == "handgun")   { animation = HANDGUN_RELOAD;    }
            if(arma.name == "shotgun")   { animation = SHOTGUN_RELOAD;    }
            if(arma.name == "rifle")     { animation = RIFLE_RELOAD;      }
            reload_left--;
            reload = !(reload_left == 1);

        }else if (shoot && game_time/arma.rate % (frame_time/10) == 0){
            if(arma.name == "handgun")   { animation = HANDGUN_SHOOT;    }
            if(arma.name == "shotgun")   { animation = SHOTGUN_SHOOT;    }
            if(arma.name == "rifle")     { animation = RIFLE_SHOOT;      }

        }else if(walk || run){
            if(arma.name == "flashlight"){ animation = FLASHLIGHT_MOVE; }
            if(arma.name == "knife")     { animation = KNIFE_MOVE;      }
            if(arma.name == "handgun")   { animation = HANDGUN_MOVE;    }
            if(arma.name == "shotgun")   { animation = SHOTGUN_MOVE;    }
            if(arma.name == "rifle")     { animation = RIFLE_MOVE;      }
        
        }else if(idle){
            if(arma.name == "flashlight"){ animation = FLASHLIGHT_IDLE; }
            if(arma.name == "knife")     { animation = KNIFE_IDLE;      }
            if(arma.name == "handgun")   { animation = HANDGUN_IDLE;    }
            if(arma.name == "shotgun")   { animation = SHOTGUN_IDLE;    }
            if(arma.name == "rifle")     { animation = RIFLE_IDLE;      }
        }

        if(walk || run){
            if (run){
                legs = FEET_RUN;
            }else{
                legs = FEET_WALK;
            }
        }else{
            legs = FEET_IDLE;
        }
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

    void renderTiro(int game_time, int frame_time){
        if (shoot && !reload && game_time/arma.rate % (frame_time/10) == 0){
            glPushMatrix();
                glRotatef(rand_offset-86, 0, 0, 1);
                glBegin(GL_POLYGON);
                    glColor4f(1, 1, 0, 1);
                    glVertex2f( +1.03, -0.0);
                    glVertex2f( +1.03, +100);
                    glVertex2f( +1.08, +100);
                    glVertex2f( +1.08, -0.0);
                glEnd();
            glPopMatrix();
            animations[SHOOT_TEXTURE].textures[0].render();
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

    void renderBody(int game_time, int frame_time){
        int index = (game_time / frame_time) % animations[animation].frames;
        animations[animation].textures[index].render();
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

    void renderLegs(int game_time, int frame_time){
        glPushMatrix();
            glTranslatef(-0.3, -0.2, -10.0);
            int index = (game_time / frame_time) % animations[legs].frames;
            animations[legs].textures[index].render();
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

    void render(int game_time, int frame_time){
        glPushMatrix();
            glTranslatef(pos.x, pos.y, 0);
            glRotatef(inclinacao, 0, 0, 1);
            renderLegs(game_time, frame_time);
            renderTiro(game_time, frame_time);
            renderBody(game_time, frame_time);
        glPopMatrix();
    }
};
