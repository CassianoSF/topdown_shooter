class GameObject{
    public:
    Coordinate pos;
    Texture texture;
    Color color;
    float height, width, angle;

    GameObject(Coordinate _pos, Texture _tex, Color _color, float _height, float _width, float _angle){
        pos = _pos;
        texture = _tex;
        color = _color;
        height = _height;
        width = _width;
        angle = _angle;
    }
    void render(){
        glPushMatrix();
            glTranslatef(pos.x, pos.y , 0);
            glRotatef(angle-90, 0, 0, 1);
            texture.render();
            glBegin(GL_POLYGON);
                glColor4f(color.r, color.g, color.b, color.a);
                glTexCoord2f(0.0, 0.0);
                glVertex2f( width/2,  height/2);
                glTexCoord2f(0.0, 30.0);
                glVertex2f( width/2, -height/2);
                glTexCoord2f(30.0, 30.0);
                glVertex2f(-width/2, -height/2);
                glTexCoord2f(30.0, 0.0);
                glVertex2f(-width/2,  height/2);
            glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
};
