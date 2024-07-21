#include "Particle.h"

#include <time.h>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

std::vector<Particle> setup() {
	std::vector<Particle> particles;
	Particle p = Particle(0,0,0);
	particles.push_back(p);
	return particles;
}

int main() {
	std::vector<Particle> particles = setup();
	std::printf("setup works!");

	sf::RenderWindow window;
	window.create(sf::VideoMode(1000, 1000, 32), "WINDOW", sf::Style::Fullscreen | sf::Style::Close | sf::Style::Resize);

	sf::Texture texture;
	std::vector<sf::Uint8> pixels(1000 * 1000 * 4, 0);
	texture.update(pixels.data());

	sf::Sprite sprite;
	sprite.setTexture(texture);



	

	while (window.isOpen()) {

		sf::Event event;
		window.pollEvent(event);
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.scancode == sf::Keyboard::Scan::Escape) {
				window.close();
			}
		}
		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}