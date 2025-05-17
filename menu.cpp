#include <GL/glut.h>
#include <string>
#include <iostream>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>

#include "xo.h"
#include "memory_game.h"
#include "flappy.h"
#include "pong.h" // Include our new Pong header
using namespace std;

enum Screen
{
    MAIN_MENU,
    GAME_MENU,
    GAME_TIC_TAC_TOE,
    GAME_MEMORY,
    GAME_FLAPPY,
    GAME_PONG
};

Screen currentScreen = MAIN_MENU;

int currentWidth = 800;
int currentHeight = 600;

const int DEFAULT_WIDTH = 700;
const int DEFAULT_HEIGHT = 610;

void resize(int w, int h)
{
    currentWidth = w;
    currentHeight = h;
    glutReshapeWindow(w, h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

struct Star
{
    float x, y;
};

Star stars[100];

void initStars()
{
    for (int i = 0; i < 100; ++i)
    {
        stars[i].x = rand() % currentWidth;
        stars[i].y = rand() % currentHeight;
    }
}

void updateStars(int value)
{
    for (int i = 0; i < 100; ++i)
    {
        stars[i].y += 1;
        if (stars[i].y > currentHeight)
            stars[i].y = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateStars, 0);
}

void drawStars()
{
    glColor3f(1, 1, 1);
    glPointSize(2);
    glBegin(GL_POINTS);
    for (int i = 0; i < 100; ++i)
        glVertex2f(stars[i].x, stars[i].y);
    glEnd();
}

struct Button
{
    int x, y, w, h;
    string label;
};

Button startBtn = {DEFAULT_WIDTH / 2 - 100, 200, 200, 50, "Start"};
Button exitBtn = {DEFAULT_WIDTH / 2 - 100, 280, 200, 50, "Exit"};
Button backBtn = {DEFAULT_WIDTH / 2 - 100, 500, 200, 50, "Back"};
Button btnTicTacToe = {DEFAULT_WIDTH / 2 - 100, 120, 200, 50, "Tic Tac Toe"};
Button btnMemory = {DEFAULT_WIDTH / 2 - 100, 190, 200, 50, "Memory Game"};
Button btnFlappy = {DEFAULT_WIDTH / 2 - 100, 260, 200, 50, "Flappy Bird"};
Button btnPong = {DEFAULT_WIDTH / 2 - 100, 330, 200, 50, "Pong"};

void drawText(float x, float y, string text)
{
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void drawButton(Button btn)
{
    glColor3f(0.2f, 0.4f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(btn.x, btn.y);
    glVertex2f(btn.x + btn.w, btn.y);
    glVertex2f(btn.x + btn.w, btn.y + btn.h);
    glVertex2f(btn.x, btn.y + btn.h);
    glEnd();

    glColor3f(1, 1, 1);
    float textX = btn.x + btn.w / 2 - (btn.label.length() * 4.5);
    float textY = btn.y + btn.h / 2 + 5;
    drawText(textX, textY, btn.label);
}

void drawLogo()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(260, 55);
    glVertex2f(440, 55);
    glVertex2f(440, 105);
    glVertex2f(260, 105);
    glEnd();

    glColor3f(1, 1, 1);
    drawText(300, 85, "Game");

    glColor3f(0.2f, 0.4f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(360, 62);
    glVertex2f(405, 62);
    glVertex2f(405, 92);
    glVertex2f(360, 92);
    glEnd();

    glColor3f(0, 0, 0);
    drawText(365, 85, "Hub");
}

bool isInside(Button btn, int mx, int my)
{
    return mx >= btn.x && mx <= btn.x + btn.w && my >= btn.y && my <= btn.y + btn.h;
}

void drawMainMenu()
{
    drawStars();
    drawLogo();
    drawButton(startBtn);
    drawButton(exitBtn);
}

void drawGameMenu()
{
    drawStars();
    drawLogo();
    drawButton(btnTicTacToe);
    drawButton(btnMemory);
    drawButton(btnFlappy);
    drawButton(btnPong);
    drawButton(backBtn);
}

void runTicTacToe()
{
    resize(600, 700);
    glClear(GL_COLOR_BUFFER_BIT);
    XO::runTicTacToeGame();
}

void runMemory()
{
    glClear(GL_COLOR_BUFFER_BIT);
    Memory::runMemoryGame();
}

void runFlappy()
{
    glClear(GL_COLOR_BUFFER_BIT);
    Flappy::runFlappyGame();
}

void runPong()
{
    resize(800, 600); // Set the window size for Pong
    glClear(GL_COLOR_BUFFER_BIT);
    Pong::runPongGame();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.05f, 0.05f, 0.15f, 1);

    switch (currentScreen)
    {
    case MAIN_MENU:
        drawMainMenu();
        break;
    case GAME_MENU:
        drawGameMenu();
        break;
    case GAME_TIC_TAC_TOE:
        runTicTacToe();
        break;
    case GAME_MEMORY:
        runMemory();
        break;
    case GAME_FLAPPY:
        runFlappy();
        break;
    case GAME_PONG:
        runPong();
        break;
    }

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;

    if (currentScreen == MAIN_MENU)
    {
        if (isInside(startBtn, x, y))
            currentScreen = GAME_MENU;
        else if (isInside(exitBtn, x, y))
            exit(0);
    }
    else if (currentScreen == GAME_MENU)
    {
        if (isInside(btnTicTacToe, x, y))
            currentScreen = GAME_TIC_TAC_TOE;
        else if (isInside(btnMemory, x, y))
            currentScreen = GAME_MEMORY;
        else if (isInside(btnFlappy, x, y))
            currentScreen = GAME_FLAPPY;
        else if (isInside(btnPong, x, y))
            currentScreen = GAME_PONG;
        else if (isInside(backBtn, x, y))
        {
            currentScreen = MAIN_MENU;
            resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
        }
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        if (currentScreen != MAIN_MENU && currentScreen != GAME_MENU)
        {
            currentScreen = GAME_MENU;
            resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
            glutDisplayFunc(display);
            glutMouseFunc(mouse);
            glutKeyboardFunc(keyboard);
            PlaySoundW(L"music/menu.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            glutPostRedisplay();
        }
    }
}

void init()
{
    resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    srand(time(0));
    initStars();
    updateStars(0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Game Hub");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    PlaySoundW(L"music/menu.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    glutMainLoop();
    return 0;
}