#include <math.h>

class Zombie{
    public:

    Coordinate pos;
    float life, angle;
    int animation;
    bool attack, move, idle, take_hit, died;
    float speed;
    int action_left;
    string action;
    int id;
    float distance;
    // int aggro;

    Zombie(){}

    Zombie(float _x, float _y, int _life, int _angle, int _id, float _speed){
        id = _id;
        pos.x = _x;
        pos.y = _y;
        life = _life;
        angle = 90;
        idle = true;
        // move = true;
        action_left = 0;
        speed = _speed;
        animation = SKELETON_IDLE;
        distance = 10;
    }

    void set(float _x, float _y, int _life, int _angle, int _id, float _speed){
        id = _id;
        pos.x = _x;
        pos.y = _y;
        life = _life;
        angle = 90;
        idle = true;
        // move = true;
        action_left = 0;
        speed = _speed;
        animation = SKELETON_IDLE;

    }

    // void colisionZombieZombie(Zombie other_zombie){
    //     float diffx = pos.x - other_zombie.pos.x;
    //     float diffy = pos.y - other_zombie.pos.y;
    //     float distance = sqrt((pow(diffy, 2) + pow(diffx, 2)));
    //     float angle_to_other_zombie = angle - ((-(GLfloat)atan2(diffx, diffy)/3.1415*180.0));
    //     bool cond = ((angle_to_other_zombie < -145 && angle_to_other_zombie > -220) || 
    //                  (angle_to_other_zombie >  145 && angle_to_other_zombie < 220));
    //     if (cond && distance < 4 && other_zombie.id != id){
    //         pos.x = pos.x - speed*2*cosf((-angle_to_other_zombie) * 3.1415 / 180);
    //         pos.y = pos.y - speed*2*sinf((-angle_to_other_zombie) * 3.1415 / 180);
    //     }

    // }

    bool colideObj(GameObject objs[], int objs_size, float x_ahead, float y_ahead){
        for (int i = 0; i < objs_size; ++i){
            float dx = objs[i].pos.x - pos.x + x_ahead;
            float dy = objs[i].pos.y - pos.y + y_ahead;
            float distance = sqrt((pow(dy, 2) + pow(dx, 2)));
            float angle_to = -(objs[i].angle - ((-(GLfloat)atan2(dx, dy)/3.1415*180.0)));
            if (angle_to>360){angle_to -=360;}
            if (angle_to<0)  {angle_to +=360;}
            if((angle_to>135 && angle_to<225) ||
               (angle_to>0   && angle_to<45)  ||
               (angle_to>315 && angle_to<360)){
                if(distance < ((objs[i].height/2.0)*(0.5+0.5*sqrt(pow(sinf(angle_to*3.1415/180), 2))))){
                    return true;
                }
            }else{
                if(distance < ((objs[i].height/2.0)*(0.5+0.5*sqrt(pow(cosf(angle_to*3.1415/180), 2))))){
                    return true;
                }
            }
        }
        return false;
    }


    void update(int game_clock, int frame_time, Player player, Zombie all_zombies[], GameObject objs[], int objs_size){
        if(life < 0){
            died = true;
        }
        if(!died){


            if(game_clock % frame_time == 0){
                float diffx = player.pos.x - pos.x;
                float diffy = player.pos.y - pos.y;
                distance        = sqrt((pow(diffy, 2) + pow(diffx, 2)));
                float angle_to_player = angle - ((-(GLfloat)atan2(diffx, diffy)/3.1415*180.0));
                bool cond = ((angle_to_player < -145 && angle_to_player > -220) || (angle_to_player > 145 && angle_to_player < 220)) && distance < 15;
                if(cond){
                    action = "follow";
                    action_left = 10000;
                    move = true;
                    idle = false;
                    if (distance < 2){
                        attack = true;
                        player.life -=1;
                    }else{
                        attack = false;
                    }
                }

                float diffx2 = pos.x - player.pos.x;
                float diffy2 = pos.y - player.pos.y;
                float angle_to_zombie = int(player.inclinacao - ((-(GLfloat)atan2(diffx2, diffy2)/3.1415*180.0))) % 360;
                bool on_player_sight = (angle_to_zombie < 90+player.arma.accuracy+5 && angle_to_zombie > 90-player.arma.accuracy) ||
                                       (angle_to_zombie > 45 && angle_to_zombie < 135 && distance < 4);
                if (on_player_sight && player.didShoot(game_clock, frame_time)){
                    life -= player.arma.damage;
                    take_hit = true;
                }else{
                    take_hit = false;
                }


                if (attack)  {animation = SKELETON_ATTACK;}
                else if(move){animation = SKELETON_MOVE;}
                else if(idle){animation = SKELETON_IDLE;}
            }
            
            if(take_hit){
                action = "follow";
                move = true;
                if (player.arma.name == "shotgun"){
                    pos.x = pos.x - 0.03*cosf((angle-90) * 3.1415 / 180);
                    pos.y = pos.y - 0.03*sinf((angle-90) * 3.1415 / 180);
                }
                pos.x = pos.x - 0.02*cosf((angle-90) * 3.1415 / 180);
                pos.y = pos.y - 0.02*sinf((angle-90) * 3.1415 / 180);
            }

            if(move && distance > 1){
                float x_ahead = speed*cosf((angle-90) * 3.1415 / 180);
                float y_ahead = speed*sinf((angle-90) * 3.1415 / 180);
                if(!colideObj(objs, objs_size, x_ahead, y_ahead)){
                    pos.x = pos.x + x_ahead;
                    pos.y = pos.y + y_ahead;
                }else{
                    string todo[2] = {"turn_left", "turn_right"};
                    action = todo[rand()%2];
                }

            }
            if(angle == 360 || angle == -360)
                angle = 0;
            if(action_left){ 
                action_left--;
                if (action == "turn_left"){
                    angle = angle + 0.05;
                }else if(action == "turn_right"){
                    angle = angle - 0.05;
                }else if(action == "follow"){
                    float diffx = player.pos.x - pos.x;
                    float diffy = player.pos.y - pos.y;
                    float angle_to_player = (-(GLfloat)atan2(diffx, diffy)/3.1415*180.0)-180;
                    angle = angle_to_player;
                }
            } 
            else{
                string todo[3] = {"turn_left", "turn_right", "none"};
                action = todo[rand()%3];
                action_left = 2000;
            }
        }
    }


    void render(int game_clock, int frame_time, GameObject blood, Animation animations[3]){
        if(take_hit){
            blood.pos.x = pos.x;
            blood.pos.y = pos.y;
            blood.render();
        }
        if(!died){
            glPushMatrix();
                glTranslatef(pos.x, pos.y, -1.0);
                glRotatef(angle-90, 0, 0, 1);
                int index = (game_clock / frame_time) % animations[animation].frames;
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
            glPopMatrix();
        }
    }

};
