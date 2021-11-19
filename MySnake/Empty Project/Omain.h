#include "Omsnake.h"

void main()
{
	sf::RenderWindow win(sf::VideoMode(1000, 1000), "Snake Game");
	Snake* snake = new Snake(100, 100);
	while (win.isOpen())
	{
		sf::Event event;
		while (win.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				win.close();
		}
		snake->Move();
		snake->PrintSnake(win);
	}
}