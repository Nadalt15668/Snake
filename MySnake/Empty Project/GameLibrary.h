#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <time.h>
#include <string.h>
using namespace sf;
typedef enum { SCORE_TYPE, HIGHSCORE_TYPE} scoreType;

// Game dimensions
#define SPRITE_DIM 40
#define WIDTH 17 * SPRITE_DIM
#define HEIGHT 17 * SPRITE_DIM
#define START_SIZE 3
// Game Colors
sf::Color first_color(100, 149, 237, 255);
sf::Color second_color(131, 170, 241, 255);
sf::Color bground_color(41, 61, 98, 255);
// Score System
int score = 0, highscore = 1;
FILE* HIGHSCORE = fopen("highscore", "rb");
char* scoreString = (char*)calloc(99, sizeof(char));
char* highscoreString = (char*)calloc(99, sizeof(char));
// UI and window
Text scoreText, titleText, highscoreText;
RenderWindow window(VideoMode(WIDTH, HEIGHT), "Snake Game");
Font font;
// Structs
typedef struct Point
{
    int x;
    int y;
}Point;
typedef struct Snake
{
    Point* snakeBody;
    int cur_len;
    Sprite bodySprite;
    Sprite headSprite;
    int speed;
    int dir;
};
typedef struct Apple
{
    Point location;
    Sprite sprite;
};

