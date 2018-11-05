
class GameObject{
    public:
    Coordinate pos;
    Texture tex;
    Color color;
    float height, width, angle;

    GameObject(Coordinate _pos, Texture _tex, Color _color, float _height, float _width, float _angle){
        pos = _pos;
        tex = _tex;
        color = _color;
        height = _height;
        width = _width;
        angle = _angle;
    }

    void render(){
        glPushMatrix();
            glTranslatef(pos.x, pos.y , 0);
            glRotatef(angle-90, 0, 0, 1);
            glBegin(GL_POLYGON);
                glColor4f(color.r, color.g, color.b, color.a);
                glVertex2f( width/2,  height/2);
                glVertex2f( width/2, -height/2);
                glVertex2f(-width/2, -height/2);
                glVertex2f(-width/2,  height/2);
            glEnd();
        glPopMatrix();
    }
};
