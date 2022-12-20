#include "application/Application.h"

int main(int argc, char** argv) {
	const char* filePath = "/Users/abdoulayedia/Projects/Dev/C++/sprite_editor/"
	                       "assets/images/spritesheets/vegito/ssj_blue.png";
	sf::Texture&      t  = se::AssetManager::Get().GetTexture(filePath);
	sf::Sprite        s(t);
	se::SpriteManager sm(s);

	auto slices = sm.SliceSprite(filePath);
	std::cout << "count: " << slices.size() << "\n";

	sf::RenderWindow window(
	    sf::VideoMode(s.getGlobalBounds().width, s.getGlobalBounds().height),
	    "Test");
	window.setFramerateLimit(60);
	sf::Clock clock;

	sf::RectangleShape r;
	r.setFillColor(sf::Color::Transparent);
	r.setOutlineColor(sf::Color::Yellow);
	r.setOutlineThickness(1.f);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					switch (event.key.code) {
						case sf::Keyboard::Escape:
							window.close();
							break;

						default:
							break;
					}
					break;

				default:
					break;
			}
		}

		window.clear();

		window.draw(s);
		for (auto& frame : slices) {
			r.setPosition(frame.left, frame.top);
			r.setSize(sf::Vector2f(frame.width, frame.height));

			window.draw(r);
		}

		window.display();
	}

	// return se::Application::Get().Run();
	return 0;
}
