#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <windows.h>
#include <mmsystem.h>

#include "menu.h"

using namespace std;



namespace Flappy {

    struct Pipe {
        float x;
        float gapY;
        bool passed = false;
    };

    const int WIDTH = 800;
    const int HEIGHT = 600;
    const int PIPE_WIDTH = 80;
    const float GRAVITY = 0.5f;
    const float JUMP_STRENGTH = -8.0f;
    const int GAP_HEIGHT = 200;

    int score = 0;
    int highScore = 0;
    float birdY = HEIGHT / 2;
    float birdVelocity = 0;
    bool gameOver = false;

    vector<Pipe> pipes;

    void drawText(float x, float y, string text) {
        glRasterPos2f(x, y);
        for (char c : text)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    void drawCircle(float x, float y, float radius) {
        glColor3f(1, 1, 0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 100; i++) {
            float angle = i * 2.0f * 3.14159f / 100;
            glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
        }
        glEnd();
    }

    void drawRect(float x, float y, float width, float height) {
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }

    void resetGame() {
        birdY = HEIGHT / 2;
        birdVelocity = 0;
        pipes.clear();
        gameOver = false;
        score = 0;
    }

    void loadHighScore() {
        FILE* file = fopen("highscore.txt", "r");
        if (file) {
            fscanf(file, "%d", &highScore);
            fclose(file);
        }
    }

    void saveHighScore() {
        if (score > highScore) {
            FILE* file = fopen("highscore.txt", "w");
            if (file) {
                fprintf(file, "%d", score);
                fclose(file);
            }
            highScore = score;
        }
    }

    void displayflappy() {
        glClear(GL_COLOR_BUFFER_BIT);

        drawCircle(200, birdY, 20);

        glColor3f(0.0f, 0.7f, 0.0f);
        for (const auto& pipe : pipes) {
            drawRect(pipe.x, 0, PIPE_WIDTH, pipe.gapY - GAP_HEIGHT / 2);
            drawRect(pipe.x, pipe.gapY + GAP_HEIGHT / 2, PIPE_WIDTH, HEIGHT - pipe.gapY);
        }

        glColor3f(0, 0, 0);
        drawText(20, 30, "Score: " + to_string(score));
        drawText(650, 30, "High: " + to_string(highScore));

        if (gameOver) {
            glColor3f(1, 0, 0);
            drawText(WIDTH / 2 - 60, HEIGHT / 2 - 50, "GAME OVER!");

            glColor3f(0.1f, 0.8f, 0.2f);
            glBegin(GL_QUADS);
            glVertex2f(WIDTH / 2 - 50, HEIGHT / 2 - 30);
            glVertex2f(WIDTH / 2 + 50 , HEIGHT / 2 - 30);
            glVertex2f(WIDTH / 2 + 50 , HEIGHT / 2 );
            glVertex2f(WIDTH / 2 - 50,HEIGHT / 2 );
            glEnd();

            glColor3f(0, 0, 0);
            drawText(WIDTH / 2 - 30, HEIGHT / 2 - 10 , "Restart");

            glColor3f(0.1f, 0.8f, 0.2f);
            glBegin(GL_QUADS);
            glVertex2f(WIDTH / 2 - 50, HEIGHT / 2 + 15);
            glVertex2f(WIDTH / 2 + 50 , HEIGHT / 2 + 15);
            glVertex2f(WIDTH / 2 + 50 , HEIGHT / 2 + 45);
            glVertex2f(WIDTH / 2 - 50,HEIGHT / 2 + 45);
            glEnd();

            glColor3f(0, 0, 0);
            drawText(WIDTH / 2 - 20, HEIGHT / 2 + 35 , "Back");
        }

        glutSwapBuffers();
    }

    void update(int value) {
        if (gameOver) {
            saveHighScore();
            return;
        }

        birdVelocity += GRAVITY;
        birdY += birdVelocity;

        static int frameCount = 0;
        frameCount++;
        if (frameCount % 90 == 0) {
            Pipe newPipe;
            newPipe.x = WIDTH;
            newPipe.gapY = 150 + rand() % (HEIGHT - 300);
            pipes.push_back(newPipe);
        }

        for (auto& pipe : pipes)
            pipe.x -= 5;

        while (!pipes.empty() && pipes.front().x + PIPE_WIDTH < 0)
            pipes.erase(pipes.begin());

        for (auto& pipe : pipes) {
            if (200 + 20 > pipe.x && 200 - 20 < pipe.x + PIPE_WIDTH) {
                if (birdY - 20 < pipe.gapY - GAP_HEIGHT / 2 || birdY + 20 > pipe.gapY + GAP_HEIGHT / 2)
                    gameOver = true;
            }

            if (!pipe.passed && pipe.x + PIPE_WIDTH < 200 - 20) {
                score++;
                pipe.passed = true;
            }
        }

        if (birdY < 0 || birdY > HEIGHT)
            gameOver = true;

        glutPostRedisplay();
        glutTimerFunc(16, update, 0);
    }

    void keyboardflappy(unsigned char key, int x, int y) {
        if (key == ' ') {
            if (!gameOver)
                birdVelocity = JUMP_STRENGTH;
        }
    }

    void mouseflappy(int button, int state, int x, int y) {
        if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

        if (gameOver) {
            if (x >= WIDTH / 2 - 50 && x <= WIDTH / 2 + 50 && y >= HEIGHT / 2 - 30 && y <= HEIGHT / 2) {
                resetGame();
                glutTimerFunc(0, update, 0);
            }
            
            else if (x >= WIDTH / 2 - 50 && x <= WIDTH / 2 + 50 && y >= HEIGHT / 2 + 15 && y <= HEIGHT / 2 + 45) {
                    saveHighScore();
                    currentScreen = GAME_MENU;
                    resize(WIDTH, HEIGHT);
                    glutDisplayFunc(display);
                    glutMouseFunc(mouse);
                    glutKeyboardFunc(nullptr);
                    PlaySoundA("music/menu.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                    glutPostRedisplay();
            }
        }
    }



    void initFlappy() {
        glClearColor(0.5f, 0.8f, 0.9f, 1.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, WIDTH, HEIGHT, 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        srand(time(0));
    }

    void runFlappyGame() {
        resetGame();
        initFlappy();
        loadHighScore();
        glutDisplayFunc(displayflappy);
        glutKeyboardFunc(keyboardflappy);
        glutMouseFunc(mouseflappy);
        glutTimerFunc(0, update, 0);
        PlaySoundA("music/flappybird.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }

}
