#ifndef PONG_H
#define PONG_H

#include <GL/glut.h>
#include <cmath>
#include <string>
#include <windows.h>
#include <mmsystem.h>

namespace Pong
{
    // Game variables
    extern float player1PositionY;
    extern float player2PositionY;
    extern float ballPositionX;
    extern float ballPositionY;
    extern float ballSpeedX;
    extern float ballSpeedY;
    extern float playerSpeed;
    extern int scorePlayer1;
    extern int scorePlayer2;

    // Texture IDs
    extern GLuint luffyTexture;
    extern GLuint zoroTexture;
    extern GLuint buggyTexture;

    // Game functions
    void initPong();
    void displayPong();
    void updateBall(int value);
    void pongKeyboard(unsigned char key, int x, int y);
    void pongSpecialKeys(int key, int x, int y);
    void loadTextures();
    void drawTexturedRect(float x, float y, float width, float height, GLuint textureID);
    void drawScore();
    void runPongGame();
    void cleanupPong();
}

#endif