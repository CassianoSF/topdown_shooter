class Zombie{
	public:

	Coordinate pos;
	int life, speed, angle;
	string animation;
	bool attack, move, idle;

	Zombie(float _x, float _y, int _life, int _angle){
		pos.x = _x;
		pos.y = _y;
		life = _life;
		angle = _angle;
		idle = true;
		animation = "skeleton_idle";
	}
	void update(int game_clock, int frame_time, Player player){
		pos.x = pos.x + 0.01*cosf(player.pos.x - pos.x);
		pos.y = pos.y + 0.01*sinf(player.pos.y - pos.y);
		if (attack){
			animation = "skeleton_attack";

		}else if(move){
			animation = "skeleton_move";

		}else if(idle){
			animation = "skeleton_idle";

		}
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
        glPopMatrix();
	}

};