#include "application/Application.h"

int main(int argc, char** argv) {
	const char* filePath = "/Users/abdoulayedia/Projects/Dev/C++/sprite_editor/"
	                       "assets/images/spritesheets/goku/1.png";
	sf::Texture&      t  = se::AssetManager::Get().GetTexture(filePath);
	sf::Sprite        s(t);
	se::SpriteManager sm(s);

	std::vector<sf::IntRect> slices;

	sf::RenderWindow window(
	    sf::VideoMode(s.getGlobalBounds().width, s.getGlobalBounds().height),
	    "Test");
	window.setFramerateLimit(60);
	sf::Clock clock;

	sf::Texture removeBackgroundTexture = *sm.GetSprite().getTexture();

	sf::Vector2i mousePos;
	sf::Vector2f start, end;
	bool         isLeftMouseButtonPressed = false;

	sf::RectangleShape r;
	r.setFillColor(sf::Color::Transparent);
	r.setOutlineColor(sf::Color::Yellow);
	r.setOutlineThickness(1.f);

	sf::RectangleShape roiRect;
	roiRect.setFillColor(sf::Color::Transparent);
	roiRect.setOutlineColor(sf::Color::Red);
	roiRect.setOutlineThickness(1.f);

	while (window.isOpen()) {
		mousePos = sf::Mouse::getPosition(window);

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
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left) {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
							se::SpriteManager::removeTextureBackground(
							    removeBackgroundTexture,
							    mousePos.x,
							    mousePos.y);
							s.setTexture(removeBackgroundTexture);
						}
						start                    = sf::Vector2f(mousePos);
						isLeftMouseButtonPressed = true;
					}
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left) {
						isLeftMouseButtonPressed = false;
						end                      = sf::Vector2f(mousePos);

						if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
							slices =
							    sm.SliceSprite(removeBackgroundTexture,
							                   sf::IntRect(start.x,
							                               start.y,
							                               end.x - start.x,
							                               end.y - start.y));
						}
					}
					break;
				default:
					break;
			}
		}

		if (isLeftMouseButtonPressed) {
			end = sf::Vector2f(mousePos);
		}

		window.clear();

		window.draw(s);

		if (!isLeftMouseButtonPressed) {
			for (auto& frame : slices) {
				r.setPosition(start.x + frame.left, start.y + frame.top);
				r.setSize(sf::Vector2f(frame.width, frame.height));

				window.draw(r);
			}
		}

		if (isLeftMouseButtonPressed) {
			roiRect.setPosition(start);
			roiRect.setSize(sf::Vector2f(end.x - start.x, end.y - start.y));
			window.draw(roiRect);
		}

		window.display();
	}

	// return se::Application::Get().Run();
	return 0;
}
