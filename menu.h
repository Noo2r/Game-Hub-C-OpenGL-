#ifndef MAIN_MENU_H
#define MAIN_MENU_H

enum Screen {
    MAIN_MENU,
    GAME_MENU,
    GAME_TIC_TAC_TOE,
    GAME_MEMORY,
    GAME_FLAPPY,
    GAME_PONG
};

extern Screen currentScreen;

void resize(int w, int h);
void display();
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

#endif
