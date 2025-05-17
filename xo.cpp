#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

namespace XO {

int grid[3][3] = {0};
int currentPlayer = 1;
bool gameOver = false;
int winner = 0;
int win1 = 0, win2 = 0;

void resetGame() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            grid[i][j] = 0;
    currentPlayer = 1;
    gameOver = false;
    winner = 0;
    glutPostRedisplay();
}

void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawX(int row, int col) {
    float x = col * 200 + 50;
    float y = row * 200 + 100;
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + 100, y + 100);
    glVertex2f(x + 100, y);
    glVertex2f(x, y + 100);
    glEnd();
}

void drawO(int row, int col) {
    float x = col * 200 + 100;
    float y = row * 200 + 150;
    float radius = 50;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float theta = i * 2.0f * 3.14159f / 100;
        glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
    }
    glEnd();
}

void drawButton(float x, float y, float w, float h, string label) {
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    glColor3f(0, 1, 0);
    glRasterPos2f(x + 15, y + 30);
    for (char c : label)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

int checkWin() {
    for (int i = 0; i < 3; i++) {
        if (grid[i][0] != 0 && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2])
            return grid[i][0];
        if (grid[0][i] != 0 && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i])
            return grid[0][i];
    }
    if (grid[0][0] != 0 && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
        return grid[0][0];
    if (grid[0][2] != 0 && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0])
        return grid[0][2];
    return 0;
}

bool isDraw() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i][j] == 0) return false;
    return true;
}

void drawTextXO(float x, float y, string text) {
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void displayXO() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(600, 0);
    glVertex2f(600, 50);
    glVertex2f(0, 50);
    glEnd();

    glColor3f(0, 1, 0);
    drawTextXO(30, 30, "Player 1 Wins: " + to_string(win1));
    drawTextXO(430, 30, "Player 2 Wins: " + to_string(win2));

    glColor3f(0.5f, 0.5f, 0.5f);
    drawLine(200, 50, 200, 600);
    drawLine(400, 50, 400, 600);
    drawLine(0, 250, 600, 250);
    drawLine(0, 450, 600, 450);

    glColor3f(1, 1, 1);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            if (grid[i][j] == 1)
                drawX(i, j);
            else if (grid[i][j] == 2)
                drawO(i, j);
        }

    glColor3f(0, 1, 0);
    if (gameOver) {
        string msg = winner ? "Player " + to_string(winner) + " Wins!" : "Draw!";
        drawTextXO(100, 650, msg);
    }

    drawButton(400, 620, 90, 50, "Restart");

    glutSwapBuffers();
}

void mouseXO(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    if (x >= 400 && x <= 490 && y >= 620 && y <= 670) {
        resetGame();
        return;
    }

    if (gameOver) return;

    int row = (y - 50) / 200;
    int col = x / 200;

    if (row >= 0 && row < 3 && col >= 0 && col < 3 && grid[row][col] == 0) {
        grid[row][col] = currentPlayer;
        winner = checkWin();
        if (winner) {
            gameOver = true;
            if (winner == 1) win1++;
            else win2++;
        } else if (isDraw()) {
            gameOver = true;
        } else {
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
        }
    }

    glutPostRedisplay();
}



void initXO() {
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 700, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void runTicTacToeGame() {
    glutDisplayFunc(displayXO);
    glutMouseFunc(mouseXO);
    initXO();
    PlaySoundA("music/xo.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
}