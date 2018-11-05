#include "includes.cpp"



class Textura {
    public:
    string animacao;
    int index;
    GLuint data;
    Textura(){}
    void set(GLuint tex, string filename){
        smatch m;
        regex integer("(\\+|-)?[[:digit:]]+");
        regex_search(filename, m, regex("(\\+|-)?[[:digit:]]+"));
        for(auto v: m){
            stringstream geek(v);
            geek >> index;
        }
        regex_search(filename, m, regex("shoot/shoot"));            for(auto v: m){animacao = "shoot_texture";} 
        regex_search(filename, m, regex("flashlight/meleeattack")); for(auto v: m){animacao = "flashlight_meleeattack";} 
        regex_search(filename, m, regex("flashlight/move"));        for(auto v: m){animacao = "flashlight_move";} 
        regex_search(filename, m, regex("flashlight/idle"));        for(auto v: m){animacao = "flashlight_idle";} 
        regex_search(filename, m, regex("feet/walk"));              for(auto v: m){animacao = "feet_walk";} 
        regex_search(filename, m, regex("feet/strafe_left"));       for(auto v: m){animacao = "feet_strafe_left";} 
        regex_search(filename, m, regex("feet/strafe_right"));      for(auto v: m){animacao = "feet_strafe_right";} 
        regex_search(filename, m, regex("feet/run"));               for(auto v: m){animacao = "feet_run";} 
        regex_search(filename, m, regex("feet/idle"));              for(auto v: m){animacao = "feet_idle";} 
        regex_search(filename, m, regex("knife/meleeattack"));      for(auto v: m){animacao = "knife_meleeattack";} 
        regex_search(filename, m, regex("knife/move"));             for(auto v: m){animacao = "knife_move";} 
        regex_search(filename, m, regex("knife/idle"));             for(auto v: m){animacao = "knife_idle";} 
        regex_search(filename, m, regex("handgun/meleeattack"));    for(auto v: m){animacao = "handgun_meleeattack";} 
        regex_search(filename, m, regex("handgun/move"));           for(auto v: m){animacao = "handgun_move";} 
        regex_search(filename, m, regex("handgun/shoot"));          for(auto v: m){animacao = "handgun_shoot";} 
        regex_search(filename, m, regex("handgun/reload"));         for(auto v: m){animacao = "handgun_reload";} 
        regex_search(filename, m, regex("handgun/idle"));           for(auto v: m){animacao = "handgun_idle";} 
        regex_search(filename, m, regex("rifle/meleeattack"));      for(auto v: m){animacao = "rifle_meleeattack";} 
        regex_search(filename, m, regex("rifle/move"));             for(auto v: m){animacao = "rifle_move";} 
        regex_search(filename, m, regex("rifle/shoot"));            for(auto v: m){animacao = "rifle_shoot";} 
        regex_search(filename, m, regex("rifle/reload"));           for(auto v: m){animacao = "rifle_reload";} 
        regex_search(filename, m, regex("rifle/idle"));             for(auto v: m){animacao = "rifle_idle";} 
        regex_search(filename, m, regex("shotgun/meleeattack"));    for(auto v: m){animacao = "shotgun_meleeattack";} 
        regex_search(filename, m, regex("shotgun/move"));           for(auto v: m){animacao = "shotgun_move";} 
        regex_search(filename, m, regex("shotgun/shoot"));          for(auto v: m){animacao = "shotgun_shoot";} 
        regex_search(filename, m, regex("shotgun/reload"));         for(auto v: m){animacao = "shotgun_reload";} 
        regex_search(filename, m, regex("shotgun/idle"));           for(auto v: m){animacao = "shotgun_idle";} 
        cout << filename << endl;
        data = tex;
    }

    void render(){
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glEnable(GL_TEXTURE_2D);
        glTranslatef(0, 0, 7);
        if(animacao == "knife_meleeattack"){
            glTranslatef(0.3,-0.45,6.2);
        }

        if (animacao == "flashlight_meleeattack"){
            glTranslatef(-0.1,-0.1, 2.2);
        }

        if(animacao == "handgun_meleeattack"){
            glTranslatef(0.2,-0.2,3.2);
        }   
        if(animacao == "rifle_meleeattack" || animacao == "shotgun_meleeattack"){
            glTranslatef(0,0,8);
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, data);
    }
};