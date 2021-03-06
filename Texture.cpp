class Texture {
    public:
    string animation;
    int index;
    GLuint data;
    Texture(){}

    void render(){
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glEnable(GL_TEXTURE_2D);
        glTranslatef(0, 0, 0);
        if(animation == "knife_meleeattack"){
            glTranslatef(0.3,-0.45, 6.2);
        }

        if (animation == "flashlight_meleeattack"){
            glTranslatef(-0.1, 0.2, 2.2);
        }

        if(animation == "handgun_meleeattack"){
            glTranslatef(0.2,-0.2, 3.2);
        }   
        if(animation == "rifle_meleeattack" || animation == "shotgun_meleeattack"){
            glTranslatef(0, 0, 8);
        }
        if(animation == "skeleton_move" || animation == "skeleton_attack"){
            glTranslatef(0, 0, 5);
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, data);
    }
};
