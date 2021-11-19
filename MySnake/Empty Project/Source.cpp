#include "GameLibrary.h"

Snake initSnake();
int collDetected(Snake* snakeStruct, Apple* appleStruct);
void moveSnake(Snake* snakeStruct);
void drawSnake(Snake* snakeStruct);

void generateApple(Apple* appleStruct, Snake* sankeStruct);
void drawApple(Apple* appleSturct);
void drawUI();
void drawBackground(Color first_color, Color second_color, Color bground_color);

void getScoreString(scoreType scoreType)
{
    int tempScore = (scoreType == SCORE_TYPE)? score : highscore,
        end_index = 1;
    char* stringedScore = (char*)malloc(sizeof(char));
    stringedScore[0] = '\0';
    while (tempScore)
    {
        char curChar = (tempScore % 10) + 48;
        tempScore /= 10;
        stringedScore = (char*)realloc(stringedScore, sizeof(stringedScore) / sizeof(char) + 1);
        end_index++;
        for (int i = end_index; i > 0; i--)
            stringedScore[i] = stringedScore[i - 1];
        stringedScore[0] = curChar;
    }
    if (scoreType == SCORE_TYPE)
    {
        strcpy(scoreString, "Score: ");
        strcat(scoreString, stringedScore);
    }
    else if (scoreType == HIGHSCORE_TYPE)
    {
        strcpy(highscoreString, "Highscore: ");
        strcat(highscoreString, stringedScore);
    }
}
void rotateSprite(Sprite* sprite, int rotation, int newOriginX, int newOriginY)
{
    sprite->setRotation((float)rotation);
    sprite->setOrigin(newOriginX, newOriginY);
}

int main()
{
    font.loadFromFile("PIXEL_FONT.ttf");
    srand((unsigned int)time(NULL));
// -------------------------------------- Setting window icon -------------------------------------
    Image icon;
    icon.loadFromFile("snk_bod.png");
    window.setIcon(SPRITE_DIM, SPRITE_DIM, icon.getPixelsPtr());
// ------------------------------------------------------------------------------------------------
// --------------------------------- Setting SCORE and TITLE text ---------------------------------
    // Reading last highscore
    //fwrite(&highscore, sizeof(int), 1, HIGHSCORE);
    fread(&highscore, sizeof(int), 1, HIGHSCORE);
    fclose(HIGHSCORE);
    // Starting score
    strcpy(scoreString, "Score: 0");
    getScoreString(HIGHSCORE_TYPE);
    // Setting text fonts
    scoreText.setFont(font);
    highscoreText.setFont(font);
    titleText.setFont(font);

    // Setting text sizes
    scoreText.setCharacterSize(18.0f);
    highscoreText.setCharacterSize(18.0f);
    titleText.setCharacterSize(30.0f);
    // Setting title text style
    titleText.setStyle(Text::Bold);
    // Setting text strings
    titleText.setString("Snake");
    scoreText.setString(scoreString);
    highscoreText.setString(highscoreString);
    // Setting text colors
    scoreText.setFillColor(Color::White);
    highscoreText.setFillColor(Color::White);
    titleText.setFillColor(Color::White);
    // Setting text positions
    scoreText.setPosition(Vector2f(1.0f, 1.0f));
    highscoreText.setPosition(Vector2f(1.0f, 20.0f));
    titleText.setPosition(Vector2f((float)WIDTH / 2, titleText.getLocalBounds().height));
    // Setting title drawing origin
    titleText.setOrigin(titleText.getLocalBounds().left + titleText.getLocalBounds().width / 2.0f,
        titleText.getLocalBounds().top + titleText.getLocalBounds().height / 2.0f);
// ------------------------------------------------------------------------------------------------
// ------------------------------------- Initializing structs -------------------------------------
    // Initializing apple and setting sprite
    Snake snakeStruct = initSnake();
    Texture bodyTex, headTex;
    bodyTex.loadFromFile("snk_bod.png");
    headTex.loadFromFile("snk_head.png");
    snakeStruct.bodySprite.setTexture(bodyTex);
    snakeStruct.headSprite.setTexture(headTex);
    // Initializing apple, setting sprite and generating first location
    Apple appleStruct;
    generateApple(&appleStruct, &snakeStruct);
    Texture appleTex;
    appleTex.loadFromFile("apple.png");
    appleStruct.sprite.setTexture(appleTex);
// ------------------------------------------------------------------------------------------------
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) 
            {
                if ((event.key.code == Keyboard::W || event.key.code == Keyboard::Up) && snakeStruct.dir != 2)
                    snakeStruct.dir = 3;
                else if ((event.key.code == Keyboard::S || event.key.code == Keyboard::Down) && snakeStruct.dir != 3)
                    snakeStruct.dir = 2;
                else if ((event.key.code == Keyboard::D || event.key.code == Keyboard::Right) && snakeStruct.dir != 1)
                    snakeStruct.dir = 0;
                else if ((event.key.code == Keyboard::A || event.key.code == Keyboard::Left) && snakeStruct.dir != 0)
                    snakeStruct.dir = 1;
                break;
            }
        }
    // ------------------------------------- Gameplay Section -----------------------------------------
        window.clear();
        drawBackground(first_color, second_color, bground_color);
        drawUI();
        moveSnake(&snakeStruct);
        drawSnake(&snakeStruct);
        if (!collDetected(&snakeStruct, &appleStruct))
        {
            window.close();
            fclose(HIGHSCORE);
        }
        drawApple(&appleStruct);
        window.display();
        sleep(milliseconds(100));
    // ------------------------------------------------------------------------------------------------
    }
    return 0;
}

Snake initSnake()
{
    Sprite bodySprite, headSprite;
    Point* snakeBody = (Point*)calloc(START_SIZE, sizeof(Point));
    Snake snakeStruct = { snakeBody, START_SIZE, bodySprite, headSprite, 200, 0 };
    for (int i = 0; i < snakeStruct.cur_len; i++)
    {
        snakeStruct.snakeBody[i].x = (WIDTH / SPRITE_DIM / 2) * SPRITE_DIM;
        snakeStruct.snakeBody[i].y = (HEIGHT / SPRITE_DIM / 2) * SPRITE_DIM;
    }
    return snakeStruct;
}

void checkHighscore()
{
    if (score > highscore)
    {
        highscore = score;
        HIGHSCORE = fopen("highscore", "wb");
        fwrite(&highscore, sizeof(int), 1, HIGHSCORE);
        fclose(HIGHSCORE);
        getScoreString(HIGHSCORE_TYPE);
    }
}

int collDetected(Snake* snakeStruct, Apple* appleStruct)
{
    // If apple eaten
    if (snakeStruct->snakeBody[0].x == appleStruct->location.x &&
        snakeStruct->snakeBody[0].y == appleStruct->location.y)
    {
        snakeStruct->snakeBody = (Point*)realloc(snakeStruct->snakeBody, sizeof(Point) * (snakeStruct->cur_len + 1));
        snakeStruct->cur_len += 1;
        score++;
        getScoreString(SCORE_TYPE);
        checkHighscore();
        generateApple(appleStruct, snakeStruct);
    }
    // If out of bounds
    if (snakeStruct->snakeBody[0].x > WIDTH - SPRITE_DIM || snakeStruct->snakeBody[0].x < 0 ||
        snakeStruct->snakeBody[0].y < SPRITE_DIM || snakeStruct->snakeBody[0].y > HEIGHT - SPRITE_DIM)
        return 0;
    // If hit itself
    for (int i = 1; i < snakeStruct->cur_len; i++)
    {
        if (snakeStruct->snakeBody[0].x == snakeStruct->snakeBody[i].x &&
            snakeStruct->snakeBody[0].y == snakeStruct->snakeBody[i].y)
            return 0;
    }
}

void moveSnake(Snake* snakeStruct)
{
    for (int i = snakeStruct->cur_len - 1; i > 0; i--)
    {
        snakeStruct->snakeBody[i].x = snakeStruct->snakeBody[i - 1].x;
        snakeStruct->snakeBody[i].y = snakeStruct->snakeBody[i - 1].y;
    }
    if (snakeStruct->dir == 0) snakeStruct->snakeBody[0].x += SPRITE_DIM;
    if (snakeStruct->dir == 1) snakeStruct->snakeBody[0].x -= SPRITE_DIM;
    if (snakeStruct->dir == 2) snakeStruct->snakeBody[0].y += SPRITE_DIM;
    if (snakeStruct->dir == 3) snakeStruct->snakeBody[0].y -= SPRITE_DIM;
}

void drawSnake(Snake* snakeStruct)
{
    snakeStruct->headSprite.setPosition(snakeStruct->snakeBody[0].x, snakeStruct->snakeBody[0].y);
    // Rotating sprites according to the snake's current direction
    switch (snakeStruct->dir)
    {
    case (0):
        rotateSprite(&snakeStruct->headSprite, 90, 0, SPRITE_DIM);
        break;
    case (1):
        rotateSprite(&snakeStruct->headSprite, -90, SPRITE_DIM, 0);
        break;
    case (2):
        rotateSprite(&snakeStruct->headSprite, 180, SPRITE_DIM, SPRITE_DIM);
        break;
    case (3):
        rotateSprite(&snakeStruct->headSprite, 0, 0, 0);
        break;
    default:
        break;
    }
    window.draw(snakeStruct->headSprite);
    for (int i = 1; i < snakeStruct->cur_len; i++)
    {
        snakeStruct->bodySprite.setPosition(snakeStruct->snakeBody[i].x, snakeStruct->snakeBody[i].y);
        window.draw(snakeStruct->bodySprite);
    }
}

void generateApple(Apple* appleStruct, Snake* snakeStruct)
{
    int new_x;
    int new_y;
    int index = 0;
    do
    {
        new_x = (rand() % (WIDTH / SPRITE_DIM)) * SPRITE_DIM;
        new_y = (rand() % (HEIGHT / SPRITE_DIM)) * SPRITE_DIM;
    } while (snakeStruct->snakeBody[index].x == new_x &&
        snakeStruct->snakeBody[index].y == new_y ||
        new_y == 0);
    appleStruct->location.x = new_x;
    appleStruct->location.y = new_y;
}

void drawApple(Apple* appleSturct)
{
    appleSturct->sprite.setPosition(appleSturct->location.x,
                                    appleSturct->location.y);
    window.draw(appleSturct->sprite);
}

void drawBackground(sf::Color first_color, sf::Color second_color, sf::Color bground_color)
{
    RectangleShape back_tile;
    back_tile.setSize(sf::Vector2f((float)WIDTH, (float)SPRITE_DIM));
    back_tile.setFillColor(bground_color);
    window.draw(back_tile);
    back_tile.setSize(sf::Vector2f((float)SPRITE_DIM, (float)SPRITE_DIM));
    back_tile.setFillColor(first_color);
    int rowFlag = 0;// Determains which color started the last row

    for (float i = SPRITE_DIM; i < HEIGHT; i += SPRITE_DIM)
    {
        back_tile.setFillColor((!rowFlag) ? second_color : first_color);
        for (float j = 0; j < WIDTH; j += SPRITE_DIM)
        {
            back_tile.setPosition(j, i);
            window.draw(back_tile);
            back_tile.setFillColor((back_tile.getFillColor() == first_color) ? second_color : first_color);
        }
        rowFlag = (rowFlag) ? 0 : 1;
    }
}

void drawUI()
{
    scoreText.setString(scoreString);
    highscoreText.setString(highscoreString);
    window.draw(scoreText);
    window.draw(highscoreText);
    window.draw(titleText);
}