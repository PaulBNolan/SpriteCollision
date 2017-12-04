#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\Main.hpp>
#include <SFML\Audio.hpp>
#include <SFML/System/Time.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <Player.h>
#include <Input.h>
#include <Debug.h>
using namespace std;

sf::VertexArray m_targetLine(sf::Lines);
sf::VertexArray m_playerLine(sf::Lines);


int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
	
	// Load a sprite to display
	sf::Texture sprite_sheet;
	if (!sprite_sheet.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture mouse_texture;
	if (!mouse_texture.loadFromFile("assets\\mouse.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	sf::Texture bounding_texture;
	if (!bounding_texture.loadFromFile("assets\\Bounding.png"))
	{

	}
	// Setup a mouse Sprite
	sf::Sprite mouse;
	mouse.setTexture(mouse_texture);

	sf::Sprite bounding;
	bounding.setTexture(bounding_texture);
	//Setup mouse AABB
	c2AABB aabb_mouse;
	aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
	aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().height);

	c2Circle circle;
	circle.p = c2V(mouse.getPosition().x + mouse.getGlobalBounds().width / 2, mouse.getPosition().y + mouse.getGlobalBounds().height / 2);
	circle.r = mouse.getGlobalBounds().width / 2;
	sf::CircleShape m_circle;
	m_circle.setRadius(circle.r);
	m_circle.setFillColor(sf::Color::Green);

	c2Circle target;
	target.r = circle.r;
	sf::Vector2f postion(600, 200);
	target.p = c2V(postion.x + target.r, postion.y + target.r);

	sf::CircleShape m_target;
	m_target.setRadius(target.r);
	m_target.setFillColor(sf::Color::Green);
	m_target.setPosition(postion);

	c2Capsule capsule;
	sf::Vector2f capsulePosition(400, 0);
	capsule.r = circle.r;
	capsule.a = c2V(capsulePosition.x + capsule.r,capsulePosition.y + capsule.r);
	capsule.b = c2V(capsulePosition.x + capsule.r + mouse.getGlobalBounds().width, capsulePosition.y + capsule.r);

	sf::CircleShape circles[2];
	circles[0].setPosition(capsulePosition);
	circles[1].setPosition(capsulePosition.x + mouse.getGlobalBounds().width, capsulePosition.y);
	for (int index = 0; index < 2; index++)
	{
		circles[index].setRadius(capsule.r);
		circles[index].setFillColor(sf::Color::Yellow);
	}

	sf::RectangleShape rectangle;
	rectangle.setPosition(capsulePosition.x + capsule.r, capsulePosition.y);
	sf::Vector2f capsuleSize(mouse.getGlobalBounds().width, 2 * capsule.r);
	rectangle.setSize(capsuleSize);
	rectangle.setFillColor(sf::Color::Yellow);

	// Setup Players Default Animated Sprite
	AnimatedSprite animated_sprite(sprite_sheet);
	animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));
	animated_sprite.setPosition(40, 100);
	// Setup Players AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(animated_sprite.getPosition().x, animated_sprite.getPosition().y);
	aabb_player.max = c2V(animated_sprite.getPosition().x + animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(),
		animated_sprite.getPosition().y + animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size());

	bounding.setPosition(0, 500);
	c2AABB aabb_bounding;
	aabb_bounding.min = c2V(0, 500);
	aabb_bounding.max = c2V(0 + animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(),
		500 + animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size());

	sf::RectangleShape background;

	sf::Vector2f size(animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(), animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size());
	background.setSize(size);
	background.setFillColor(sf::Color::Blue);
	background.setPosition(animated_sprite.getPosition());
	// Setup the Player
	Player player(animated_sprite);
	Input input;

	
	
	
	sf::Vertex startPoint;
	sf::Vertex endPoint;
	sf::Vector2f m_vertexStart = { 0,0 };
	sf::Vector2f m_vertexEnd = { 800, 600 };

	float distance = sqrt((m_vertexEnd.x - m_vertexStart.x) * (m_vertexEnd.x - m_vertexStart.x) + (m_vertexEnd.y - m_vertexStart.y) * (m_vertexEnd.y - m_vertexStart.y));
	sf::Vector2f m_direction = { (m_vertexEnd.x - m_vertexStart.x)/ distance,(m_vertexEnd.y - m_vertexStart.y)/distance };
	c2Ray targetRay;
	targetRay.p = c2V(m_vertexStart.x, m_vertexStart.y);
	targetRay.d = c2V(m_direction.x, m_direction.y);
	targetRay.t = distance;

	c2Raycast targetRayCast;
	targetRayCast.t = distance;
	targetRayCast.n = {m_vertexEnd.x - m_vertexStart.x,m_vertexEnd.y - m_vertexStart.y };

	startPoint = sf::Vertex(static_cast<sf::Vector2f>(m_vertexStart), sf::Color::Red);
	endPoint = sf::Vertex(static_cast<sf::Vector2f>(m_vertexEnd), sf::Color::Red);

	m_targetLine.append(startPoint);
	m_targetLine.append(endPoint);




	sf::Vertex playerStartPoint;
	sf::Vertex playerEndPoint;

	sf::Vector2f m_playerVertexStart = { mouse.getPosition().x, mouse.getPosition().y };
	sf::Vector2f m_playerVertexEnd = { mouse.getPosition().x + mouse.getGlobalBounds().width, mouse.getPosition().y + mouse.getGlobalBounds().width };

	playerStartPoint = sf::Vertex(static_cast<sf::Vector2f>(m_playerVertexStart), sf::Color::Blue);
	playerEndPoint = sf::Vertex(static_cast<sf::Vector2f>(m_playerVertexEnd), sf::Color::Blue);

	m_playerLine.append(playerStartPoint);
	m_playerLine.append(playerEndPoint);

	distance = sqrt((m_playerVertexEnd.x - m_playerVertexStart.x) * (m_playerVertexEnd.x - m_playerVertexStart.x) + (m_playerVertexEnd.y - m_playerVertexStart.y) * (m_playerVertexEnd.y - m_playerVertexStart.y));
	sf::Vector2f m_playerDirection = { (m_playerVertexEnd.x - m_playerVertexStart.x) / distance,(m_playerVertexEnd.y - m_playerVertexStart.y) / distance };

	c2Ray playerRay;
	playerRay.p = c2V(m_playerVertexStart.x, m_playerVertexStart.y);
	playerRay.d = c2V(m_playerDirection.x, m_playerDirection.y);
	playerRay.t = distance;

	c2Raycast playerRayCast;
	playerRayCast.t = distance;
	playerRayCast.n = { c2Norm(c2V(m_playerVertexEnd.x - m_playerVertexStart.x,m_playerVertexEnd.y - m_playerVertexStart.y)) };
	// Collision result
	int result = 0;
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		mouse.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		// Update mouse AABB
		aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
		aabb_mouse.max = c2V(mouse.getPosition().x + mouse.getGlobalBounds().width, mouse.getPosition().y + mouse.getGlobalBounds().width);


		circle.p = c2V(mouse.getPosition().x + mouse.getGlobalBounds().width / 2, mouse.getPosition().y + mouse.getGlobalBounds().height / 2);
		circle.r = mouse.getGlobalBounds().width / 2;

	
		 m_playerVertexStart = { mouse.getPosition().x, mouse.getPosition().y };
		 m_playerVertexEnd = { mouse.getPosition().x + mouse.getGlobalBounds().width, mouse.getPosition().y + mouse.getGlobalBounds().width };

		playerStartPoint = sf::Vertex(static_cast<sf::Vector2f>(m_playerVertexStart), sf::Color::Blue);
		playerEndPoint = sf::Vertex(static_cast<sf::Vector2f>(m_playerVertexEnd), sf::Color::Blue);

		playerRay.p = c2V(m_playerVertexStart.x, m_playerVertexStart.y);

		playerRayCast.n = { c2Norm(c2V(m_playerVertexEnd.x - m_playerVertexStart.x,m_playerVertexEnd.y - m_playerVertexStart.y)) };
		m_playerLine.clear();
		m_playerLine.append(playerStartPoint);
		m_playerLine.append(playerEndPoint);
		
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					input.setCurrent(Input::Action::LEFT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					input.setCurrent(Input::Action::RIGHT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					input.setCurrent(Input::Action::UP);
				}
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
		}

		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();

		// Check for collisions
		result = c2AABBtoAABB(aabb_mouse, aabb_player);
		std::cout << ((result != 0) ? ("Collision") : "") << endl;
		if (result){
			player.getAnimatedSprite().setColor(sf::Color(255,0,0));
		}
		else {
			player.getAnimatedSprite().setColor(sf::Color(255, 255, 0));
		}


		result = c2AABBtoAABB(aabb_mouse, aabb_bounding);
		if (result)
		{
			bounding.setColor(sf::Color(255, 0, 0));
		}
		else
		{
			bounding.setColor(sf::Color(0, 255, 0));
		}

		m_circle.setPosition(mouse.getPosition().x, mouse.getPosition().y);
		result = c2CircletoAABB(circle,aabb_player);
		std::cout << ((result != 0) ? ("Circle Collision") : "") << endl;

		result = c2CircletoCircle(circle, target);
		std::cout << ((result != 0) ? ("Circle Collision") : "") << endl;




		result = c2AABBtoCapsule(aabb_mouse, capsule);
		std::cout << ((result != 0) ? ("Capsule Collision") : "") << endl;
		// Clear screen

		result = c2CircletoCapsule(circle, capsule);
		std::cout << ((result != 0) ? ("Capsule/Circle Collision") : "") << std::endl;

		result = c2RaytoAABB(targetRay, aabb_mouse, &targetRayCast);
		std::cout << ((result != 0) ? ("Ray Collision") : "") << std::endl;

		result = c2RaytoCircle(targetRay, circle, &targetRayCast);
		cout << ((result != 0) ? ("Circle/Ray Collision") : "") << std::endl;

		result = c2RaytoAABB(playerRay, aabb_player, &playerRayCast);
		std::cout << ((result != 0) ? ("Ray Collision") : "") << std::endl;

		result = c2RaytoCircle(playerRay, target, &playerRayCast);
		cout << ((result != 0) ? ("Ray/Circle Collision") : "") << std::endl;

		result = c2RaytoCapsule(playerRay, capsule, &playerRayCast);
		std::cout << ((result != 0) ? ("Ray Capsule Collision") : "") << std::endl;


		window.clear();

		// Draw the Players Current Animated Sprite
		window.draw(background);
		window.draw(player.getAnimatedSprite());
		window.draw(bounding);

		window.draw(m_target);

		for (int index = 0; index < 2; index++)
		{
			window.draw(circles[index]);
		}
		window.draw(rectangle);
		window.draw(mouse);
		//window.draw(m_circle);
		//window.draw(m_playerLine);

		window.draw(m_targetLine);
		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};