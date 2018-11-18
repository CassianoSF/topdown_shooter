class Text {
    public:
    string text_content;
    Color color;
    Coordinate pos;

    Text(string conteudo, Color c, float _x, float _y){
        text_content = conteudo;
        color.set(c);
        pos.x = _x;
        pos.y = _y;
    }

    void set_texto(string content){
        text_content = content;
    }

    void render(float x, float y){
        glPushMatrix();
        glTranslatef(pos.x+x, pos.y+y, 0);
        glColor3f(color.r, color.g, color.b);
        glRasterPos2f(1,1);
        for (int i = 0; i < 3; ++i){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text_content[i]);
        }
        glPopMatrix();
    }
};
