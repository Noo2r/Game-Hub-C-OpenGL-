#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "pong.h"
#include "menu.h"
#include <iostream>
#include <GL/glut.h>
#include <string>
#include <cmath>

namespace Pong
{
    // Game variables
    float player1PositionY = 0.0f;
    float player2PositionY = 0.0f;
    float ballPositionX = 0.0f;
    float ballPositionY = 0.0f;
    float ballSpeedX = 0.005f;
    float ballSpeedY = 0.005f;
    float playerSpeed = 0.05f;
    int scorePlayer1 = 0;
    int scorePlayer2 = 0;

    // Texture IDs
    GLuint luffyTexture;
    GLuint zoroTexture;
    GLuint buggyTexture;
    GLuint backgroundTexture;

    // Window dimensions
    const int PONG_WIDTH = 800;
    const int PONG_HEIGHT = 600;

    GLuint loadTexture(const char *filename)
    {
        int width, height, channels;
        unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);

        if (!data)
        {
            std::cerr << "Failed to load image: " << filename << " - " << stbi_failure_reason() << std::endl;
            return 0;
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
        return textureID;
    }

    void loadTextures()
    {
        stbi_set_flip_vertically_on_load(true);

        zoroTexture = loadTexture("images/Zoro.png");
        luffyTexture = loadTexture("images/Luffy.png");
        buggyTexture = loadTexture("images/Ball.png");
        backgroundTexture = loadTexture("images/background.jpg");

        if (zoroTexture == 0)
            std::cerr << "Zoro texture failed to load" << std::endl;
        if (luffyTexture == 0)
            std::cerr << "Luffy texture failed to load" << std::endl;
        if (buggyTexture == 0)
            std::cerr << "Ball texture failed to load" << std::endl;
        if (backgroundTexture == 0)
            std::cerr << "Background texture failed to load" << std::endl;
    }

    void drawTexturedRect(float x, float y, float width, float height, GLuint textureID)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(x + width, y);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(x + width, y + height);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(x, y + height);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    void drawScore()
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        std::string scoreText = std::to_string(scorePlayer1) + "  x  " + std::to_string(scorePlayer2);

        float textWidth = scoreText.length() * 12;
        float x = (PONG_WIDTH - textWidth) / 2.0f;
        float y = PONG_HEIGHT - 50;

        float xNormalized = x / (float)PONG_WIDTH;
        float yNormalized = y / (float)PONG_HEIGHT;

        glRasterPos2f(xNormalized, yNormalized);
        for (const char &c : scoreText)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }

    void updateBall(int value)
    {
        ballPositionX += ballSpeedX;
        ballPositionY += ballSpeedY;

        if (ballPositionY <= 0.05f)
        {
            ballPositionY = 0.05f;
            ballSpeedY = std::abs(ballSpeedY);
        }
        else if (ballPositionY >= 0.95f)
        {
            ballPositionY = 0.95f;
            ballSpeedY = -std::abs(ballSpeedY);
        }

        float paddleWidth = 0.1f;
        float paddleHeight = 0.2f;
        float ballRadius = 0.05f;

        float leftPaddleRight = 0.1f + paddleWidth;
        float leftPaddleTop = player1PositionY + paddleHeight / 2;
        float leftPaddleBottom = player1PositionY - paddleHeight / 2;

        float rightPaddleLeft = 0.9f - paddleWidth;
        float rightPaddleTop = player2PositionY + paddleHeight / 2;
        float rightPaddleBottom = player2PositionY - paddleHeight / 2;

        float ballLeft = ballPositionX - ballRadius;
        float ballRight = ballPositionX + ballRadius;
        float ballTop = ballPositionY + ballRadius;
        float ballBottom = ballPositionY - ballRadius;

        if (ballLeft <= leftPaddleRight && ballRight > leftPaddleRight - paddleWidth &&
            ballBottom <= leftPaddleTop && ballTop >= leftPaddleBottom)
        {
            float relativeIntersectY = (player1PositionY - ballPositionY) / (paddleHeight / 2);
            float bounceAngle = relativeIntersectY * (M_PI / 4);

            ballPositionX = leftPaddleRight + ballRadius;

            float speed = sqrt(ballSpeedX * ballSpeedX + ballSpeedY * ballSpeedY) * 1.05f;
            if (speed > 0.015f)
                speed = 0.015f;

            ballSpeedX = speed * cos(bounceAngle);
            ballSpeedY = -speed * sin(bounceAngle);
        }

        if (ballRight >= rightPaddleLeft && ballLeft < rightPaddleLeft + paddleWidth &&
            ballBottom <= rightPaddleTop && ballTop >= rightPaddleBottom)
        {
            float relativeIntersectY = (player2PositionY - ballPositionY) / (paddleHeight / 2);
            float bounceAngle = relativeIntersectY * (M_PI / 4);

            ballPositionX = rightPaddleLeft - ballRadius;

            float speed = sqrt(ballSpeedX * ballSpeedX + ballSpeedY * ballSpeedY) * 1.05f;
            if (speed > 0.015f)
                speed = 0.015f;

            ballSpeedX = -speed * cos(bounceAngle);
            ballSpeedY = -speed * sin(bounceAngle);
        }

        if (ballRight < 0.0f)
        {
            scorePlayer2++;
            ballPositionX = ballPositionY = 0.5f;
            ballSpeedX = 0.005f;
            ballSpeedY = 0.005f * (((float)rand() / RAND_MAX) * 2.0f - 1.0f);
        }
        else if (ballLeft > 1.0f)
        {
            scorePlayer1++;
            ballPositionX = ballPositionY = 0.5f;
            ballSpeedX = -0.005f;
            ballSpeedY = 0.005f * (((float)rand() / RAND_MAX) * 2.0f - 1.0f);
        }

        glutTimerFunc(16, updateBall, 0);
        glutPostRedisplay();
    }

    void displayPong()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Draw background
        drawTexturedRect(0.0f, 0.0f, 1.0f, 1.0f, backgroundTexture);

        // Draw center dashed line
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        for (float y = 0.0f; y < 1.0f; y += 0.05f)
        {
            glVertex2f(0.5f, y);
            glVertex2f(0.5f, y + 0.025f);
        }
        glEnd();

        // Draw game objects
        drawTexturedRect(0.1f, player1PositionY - 0.1f, 0.2f, 0.2f, zoroTexture);
        drawTexturedRect(0.8f, player2PositionY - 0.1f, 0.2f, 0.2f, luffyTexture);
        drawTexturedRect(ballPositionX - 0.05f, ballPositionY - 0.05f, 0.1f, 0.1f, buggyTexture);

        drawScore();

        glutSwapBuffers();
    }

    void pongKeyboard(unsigned char key, int x, int y)
    {
        switch (key)
        {
        case 27:
            cleanupPong();
            currentScreen = GAME_MENU;
            resize(700, 610);
            glutDisplayFunc(display);
            glutKeyboardFunc(keyboard);
            glutSpecialFunc(NULL);
            PlaySoundW(NULL, NULL, 0);
            PlaySoundW(L"music/menu.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            break;
        case 'w':
            if (player1PositionY < 0.9f)
                player1PositionY += playerSpeed;
            break;
        case 's':
            if (player1PositionY > 0.1f)
                player1PositionY -= playerSpeed;
            break;
        }
        glutPostRedisplay();
    }

    void pongSpecialKeys(int key, int x, int y)
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            if (player2PositionY < 0.9f)
                player2PositionY += playerSpeed;
            break;
        case GLUT_KEY_DOWN:
            if (player2PositionY > 0.1f)
                player2PositionY -= playerSpeed;
            break;
        }
        glutPostRedisplay();
    }

    void initPong()
    {
        player1PositionY = player2PositionY = 0.5f;
        ballPositionX = ballPositionY = 0.5f;
        ballSpeedX = 0.005f;
        ballSpeedY = 0.005f;
        scorePlayer1 = scorePlayer2 = 0;

        loadTextures();

        glutTimerFunc(16, updateBall, 0);

        PlaySoundW(NULL, NULL, 0);
        PlaySoundW(L"music/joyboy.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }

    void cleanupPong()
    {
        glDeleteTextures(1, &zoroTexture);
        glDeleteTextures(1, &luffyTexture);
        glDeleteTextures(1, &buggyTexture);
        glDeleteTextures(1, &backgroundTexture);
    }

    void runPongGame()
    {
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, 1.0, 0.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        initPong();

        glutDisplayFunc(displayPong);
        glutKeyboardFunc(pongKeyboard);
        glutSpecialFunc(pongSpecialKeys);
    }
}
