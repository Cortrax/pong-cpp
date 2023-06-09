#include <SFML/Graphics.hpp>

#include <ball.hpp>
#include <gamelogic.hpp>
#include <net.hpp>
#include <paddles.hpp>
#include <string>

int main()
{
	// Make the window
	sf::RenderWindow window(sf::VideoMode(1000, 600), "PONG");
	window.setFramerateLimit(60);

	// Create the top and bottom window borders
	sf::RectangleShape topBorder;
	sf::RectangleShape bottomBorder;

	topBorder.setSize(sf::Vector2f(1000, 10));
	topBorder.setFillColor(sf::Color(200, 200, 200));
	topBorder.setPosition(0, 0);

	bottomBorder.setSize(sf::Vector2f(1000, 10));
	bottomBorder.setFillColor(sf::Color(200, 200, 200));
	bottomBorder.setPosition(0, 590);

	// Create player paddles
	Paddles leftPaddle(10, 40, 20, 280);
	Paddles rightPaddle(10, 40, 960, 280);

	sf::Texture secretTexture;
	if (!secretTexture.loadFromFile("content/textures/default.png"))
	{
		std::cout << "Could not load secret ball texture";
	}

	// Create ball
	// Change width and height to 0.03 if image is Bop
	Ball ball(0.5, 0.5, 595, 295, secretTexture);

	int leftScore = 0;
	int rightScore = 0;

	// Draw dotted line
	Net net(3, 15);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			// Check if s or w has been pressed
			if (event.type == sf::Event::KeyPressed)
			{
				leftPaddle.processLeftEvents(event.key.code, true);
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				leftPaddle.processLeftEvents(event.key.code, false);
			}

			// Check if up or down arrow has been pressed
			if (event.type == sf::Event::KeyPressed)
			{
				rightPaddle.processRightEvents(event.key.code, true);
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				rightPaddle.processRightEvents(event.key.code, false);
			}
		}

		ball.paddleCollision(leftPaddle, rightPaddle);
		window.clear(sf::Color(6, 6, 6));
		ball.update();
		ball.resetBall();
		ball.drawTo(window);
		leftPaddle.updateLeft();
		leftPaddle.leftDrawTo(window);
		rightPaddle.updateRight();
		rightPaddle.rightDrawTo(window);
		window.draw(topBorder);
		window.draw(bottomBorder);
		net.drawTo(window);
		Logic gameLogic(leftScore, rightScore);
		gameLogic.updateScore(ball, leftScore, rightScore);
		gameLogic.drawScore(window);
		gameLogic.endGame(leftScore, rightScore, window);

		if (leftScore >= 11 || rightScore >= 11)
		{
			ball.endGame();
		}

		window.display();
	}
	return 0;
}