#include <GL/glut.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <mmsystem.h>


using namespace std;

namespace Memory {

    int level = 1;
    int ROWS = 4, COLS = 4;
    const int CARD_SIZE = 100;
    const int PADDING = 10;
    int currentPlayer = 1;
    int score1 = 0, score2 = 0;

    struct Card {
        int value;
        bool revealed;
        bool matched;
        int owner = 0;
    };

    vector<vector<Card>> grid;
    int winWidth = COLS * (CARD_SIZE + PADDING);
    int winHeight = ROWS * (CARD_SIZE + PADDING) + 100;

    Card* firstCard = nullptr;
    Card* secondCard = nullptr;
    int firstX = -1, firstY = -1, secondX = -1, secondY = -1;
    bool lockInput = false;


    void initMemory() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, winWidth, winHeight, 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void resetGame() {
        if (level == 1) {
            ROWS = 4; COLS = 4;
        } else if (level == 2) {
            ROWS = 6; COLS = 6;
        }

        int pairs = (ROWS * COLS) / 2;
        vector<int> values;
        for (int i = 1; i <= pairs; ++i) {
            values.push_back(i);
            values.push_back(i);
        }

        random_shuffle(values.begin(), values.end());

        grid.clear();
        int index = 0;
        for (int i = 0; i < ROWS; ++i) {
            vector<Card> row;
            for (int j = 0; j < COLS; ++j) {
                row.push_back({ values[index++], false, false, 0 });
            }
            grid.push_back(row);
        }

        firstCard = secondCard = nullptr;
        score1 = score2 = 0;
        currentPlayer = 1;
        lockInput = false;

        winWidth = COLS * (CARD_SIZE + PADDING);
        winHeight = ROWS * (CARD_SIZE + PADDING) + 150; 

        glutReshapeWindow(winWidth, winHeight);
        initMemory(); 

        glutPostRedisplay();
    }

    void drawText(float x, float y, string text) {
        glRasterPos2f(x, y);
        for (char c : text)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    void display() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                Card& card = grid[i][j];
                float x = j * (CARD_SIZE + PADDING);
                float y = i * (CARD_SIZE + PADDING);
                if (card.matched) {
                    glColor3f(card.owner == 1 ? 1.0f : 0.9f, card.owner == 1 ? 1.0f : 0.4f,card.owner == 1 ? 0.0f : 0.4f);
                } else if (card.revealed) glColor3f(1.0f, 1.0f, 1.0f);
                else glColor3f(0.2f, 0.2f, 0.8f);

                glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + CARD_SIZE, y);
                glVertex2f(x + CARD_SIZE, y + CARD_SIZE);
                glVertex2f(x, y + CARD_SIZE);
                glEnd();


                if (card.revealed || card.matched) {
                    glColor3f(0, 0, 0);
                    drawText(x + CARD_SIZE / 2 - 5, y + CARD_SIZE / 2, to_string(card.value));
                }
            }

        }

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(20, winHeight - 120);
        glVertex2f(150, winHeight - 120);
        glVertex2f(150, winHeight - 85);
        glVertex2f(20, winHeight - 85);
        glEnd();

        glColor3f(0.9f, 0.4f, 0.4f);
        glBegin(GL_QUADS);
        glVertex2f(250, winHeight - 120);
        glVertex2f(380, winHeight - 120);
        glVertex2f(380, winHeight - 85);
        glVertex2f(250, winHeight - 85);
        glEnd();

        glColor3f(0, 0, 0);
        drawText(150, winHeight - 130, "Turn: Player " + to_string(currentPlayer));
        drawText(40, winHeight - 95, "Player 1: " + to_string(score1));
        drawText(270, winHeight - 95, "Player 2: " + to_string(score2));
        


        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(0, winHeight - 70);
        glVertex2f(winWidth, winHeight - 70);
        glEnd();

        drawText(20, winHeight - 30, "Choose:");

        glColor3f(0.7f, 0.9f, 0.7f);
        glBegin(GL_QUADS);
        glVertex2f(100, winHeight - 50);
        glVertex2f(180, winHeight - 50);
        glVertex2f(180, winHeight - 20);
        glVertex2f(100, winHeight - 20);
        glEnd();

        glColor3f(0, 0, 0);
        drawText(120, winHeight - 30, "Easy");

        glColor3f(0.8f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(230, winHeight - 50);
        glVertex2f(310, winHeight - 50);
        glVertex2f(310, winHeight - 20);
        glVertex2f(230, winHeight - 20);
        glEnd();

        glColor3f(0, 0, 0);
        drawText(250, winHeight - 30, "Hard");

        glutSwapBuffers();
    }

    void hideUnmatched() {
        this_thread::sleep_for(chrono::milliseconds(1000));
        if (firstCard && secondCard && firstCard->value != secondCard->value) {
            firstCard->revealed = false;
            secondCard->revealed = false;
        }
        firstCard = nullptr;
        secondCard = nullptr;
        lockInput = false;
        glutPostRedisplay();
    }

    void mouse(int button, int state, int x, int y) {
        if (lockInput || button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

        if (x >= 100 && x <= 180 && y >= winHeight - 50 && y <= winHeight - 20) {
            level = 1;
            resetGame();
            return;
        }

        if (x >= 200 && x <= 280 && y >= winHeight - 50 && y <= winHeight - 20) {
            level = 2;
            resetGame();
            return;
        }


        int col = x / (CARD_SIZE + PADDING);
        int row = y / (CARD_SIZE + PADDING);

        if (row >= ROWS || col >= COLS) return;
        Card& clicked = grid[row][col];
        if (clicked.revealed || clicked.matched) return;

        clicked.revealed = true;

        if (!firstCard) {
            firstCard = &clicked;
            firstX = col; firstY = row;
        } else if (!secondCard && !(firstX == col && firstY == row)) {
            secondCard = &clicked;
            secondX = col; secondY = row;

            if (firstCard->value == secondCard->value) {
                firstCard->matched = secondCard->matched = true;
                firstCard->owner = secondCard->owner = currentPlayer;
                (currentPlayer == 1 ? score1 : score2)++;
                firstCard = secondCard = nullptr;
            } else {
                lockInput = true;
                thread(hideUnmatched).detach();
                currentPlayer = (currentPlayer == 1 ? 2 : 1);
            }
        }

        glutPostRedisplay();
    }

    void runMemoryGame() {
        resetGame();             
        initMemory();            
        glutDisplayFunc(display);
        glutMouseFunc(mouse);
        glutPostRedisplay();     
        PlaySoundA("music/memory.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }

}