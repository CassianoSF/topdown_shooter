class Text {
    public:
    string text_content;
    Color color;

    Text(string conteudo, Color c){
        text_content = conteudo;
        color.set(c);
    }

    void set_texto(string content){
        text_content = content;
    }

    void render(float x, float y){
        glPushMatrix();
        glTranslatef(x-20.5, y+18, 0);
        glColor3f(color.r, color.g, color.b);
        glRasterPos2f(1,1);
        for (int i = 0; i < 2; ++i){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text_content[i]);
        }
        glPopMatrix();
    }
};
