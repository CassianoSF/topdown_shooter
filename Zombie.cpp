#include <math.h>

class Zombie{
    public:

    Coordinate pos;
    int life, angle;
    string animation;
    bool attack, move, idle;
    float speed;
    int action_left;
    string action;
    // int aggro;

    Zombie(float _x, float _y, int _life, int _angle){
        pos.x = _x;
        pos.y = _y;
        life = _life;
        angle = _angle;
        idle = true;
        // move = true;
        action_left = 0;
        speed = 0.001;
        animation = "skeleton_idle";
    }

    void update(int game_clock, int frame_time, Player player){
        float diffx = player.pos.x - pos.x;
        float diffy = player.pos.y - pos.y;
        float distance        = sqrt((pow(diffy, 2) + pow(diffx, 2)));
        float angle_to_player = (-(GLfloat)atan2(diffx, diffy)/3.1415*180.0)-angle;
        bool cond = angle_to_player < -145 && angle_to_player > -220 && distance < 15;
        if (distance < 2 && playerOnSight(player)){
            attack = true;
        }else{
            attack = false;
        }

        if(playerOnSight(player)){
            // cout << "playerOnSight" << endl;
            action = "follow";
            action_left = 400;
            move = true;
            idle = false;
        }
        if(move){
            pos.x = pos.x + speed*cosf((angle-90) * 3.1415 / 180);
            pos.y = pos.y + speed*sinf((angle-90) * 3.1415 / 180);
        }
        if(action_left){ 
            if(game_clock % (frame_time/4) == 0){
                action_left--;
                // cout << action << endl;
                if (action == "turn_left"){
                    angle = angle + 1;
                }else if(action == "turn_right"){
                    angle = angle - 1;
                }else if(action == "follow"){
                    float diffx = player.pos.x - pos.x;
                    float diffy = player.pos.y - pos.y;
                    float angle_to_player = (-(GLfloat)atan2(diffx, diffy)/3.1415*180.0)-180;
                    angle = angle_to_player;
                }
            }
        } 
        else{
            string todo[5] = {"turn_left", "turn_right", "none"};
            action = todo[rand()%3];
            action_left = 50;
        }


        if (attack){
            animation = "skeleton_attack";

        }else if(move){
            animation = "skeleton_move";

        }else if(idle){
            animation = "skeleton_idle";
        }
    }

    bool playerOnSight(Player player){
        float diffx = player.pos.x - pos.x;
        float diffy = player.pos.y - pos.y;
        float distance        = sqrt((pow(diffy, 2) + pow(diffx, 2)));
        float angle_to_player = (-(GLfloat)atan2(diffx, diffy)/3.1415*180.0)-angle;
        bool cond = angle_to_player < -145 && angle_to_player > -220 && distance < 15;
        if(cond){
            return true;
        }
        return false;
    }

    void render(Texture textures[421+43], int game_clock, int frame_time){
        glPushMatrix();
            glTranslatef(pos.x, pos.y, -1.0);
            glRotatef(angle-90, 0, 0, 1);
            Texture texture;
            for (int i = 0; i < 421+43; ++i){
                if(textures[i].animation == animation && textures[i].index == (game_clock / frame_time) % (animation == "skeleton_attack" ? 9 : 17))
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
            // glPushMatrix();
            //     // glRotatef(-45, 0, 0, 0);
            //     glBegin(GL_POLYGON);
            //         glColor4f(1, 1, 0, 0.1);
            //         glVertex2f(0, 0);
            //          for(int i = -3; i < 27; i++){
            //             if (i < 3){
            //                 float theta = 2.0f * 3.1415926f * float(i) / float(30);
            //                 float cx = 15 * cosf(theta);
            //                 float cy = 15 * sinf(theta);
            //                 glVertex2f(cx, cy);
            //             }
            //         }
            //         glVertex2f(0, 0);
            //     glEnd();
            // glPopMatrix();
        glPopMatrix();

    }

};
