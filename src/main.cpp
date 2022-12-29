#include "application/Application.h"

float lerp(float a, float b, float f) {
	return a + f * (b - a);
}

sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float f) {
	return a + f * (b - a);
}

int main(int argc, char** argv) {
	return se::Application::Get().Run();

	const char*  filePath = "../resources/icons/icons_pack_bg.png";
	sf::Texture& t        = se::AssetManager::Get().GetTexture(filePath);
	// t.setSmooth(true);
	// anti-aliasing
	sf::Sprite        s(t);
	se::SpriteManager sm(s);

	std::vector<sf::IntRect> slices;

	sf::RenderWindow window(sf::VideoMode(s.getGlobalBounds().width, s.getGlobalBounds().height), "Test");
	window.setFramerateLimit(60);
	sf::Clock clock;
	sf::Time  elapsedTime;
	sf::View  view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));

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

	float mouseDelta = 0.f, previousMouseDelta = 0.f;
	float zoomSpeed   = 3.f;
	float minZoom     = 0.1f;
	float maxZoom     = 10.f;
	float currentZoom = 1.f, targetZoom = 1.f;

	sf::Vector2f viewPos      = view.getCenter();
	sf::Vector2f viewPosStart = viewPos;
	sf::Vector2f viewPosDelta = sf::Vector2f(0.f, 0.f);

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
							se::SpriteManager::removeTextureBackground(removeBackgroundTexture,
							                                           mousePos.x,
							                                           mousePos.y,
							                                           sf::Color::Transparent,
							                                           0.1f);
							s.setTexture(removeBackgroundTexture);
						}
						// set the start position considering the view
						start = sf::Vector2f(mousePos) - sf::Vector2f(viewPosDelta.x, -viewPosDelta.y);
						isLeftMouseButtonPressed = true;
					}
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left) {
						isLeftMouseButtonPressed = false;
						end = sf::Vector2f(mousePos) - sf::Vector2f(viewPosDelta.x, -viewPosDelta.y);

						if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
							slices = se::SpriteManager::SliceSprite(
							    removeBackgroundTexture,
							    sf::IntRect(start.x, start.y, end.x - start.x, end.y - start.y));
						}
					}
					break;
				case sf::Event::MouseWheelScrolled:
					previousMouseDelta = mouseDelta;
					mouseDelta         = event.mouseWheelScroll.delta;

					if (previousMouseDelta * mouseDelta < 0.f) {
						targetZoom = currentZoom;
						mouseDelta = 0.f;
					}

					if (mouseDelta > 0.f) {
						targetZoom *= 1.1f;
					} else if (mouseDelta < 0.f) {
						targetZoom /= 1.1f;
					}
					break;
				default:
					break;
			}
		}

		elapsedTime = clock.restart();

		if (targetZoom < minZoom) {
			targetZoom = minZoom;
		} else if (targetZoom > maxZoom) {
			targetZoom = maxZoom;
		}

		if (currentZoom != targetZoom) {
			currentZoom = lerp(currentZoom, targetZoom, elapsedTime.asSeconds() * zoomSpeed);
			view.setSize(window.getSize().x * currentZoom, window.getSize().y * currentZoom);
			// move the view to the mouse position
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem)) {
			viewPos = lerp(viewPos, sf::Vector2f(mousePos), elapsedTime.asSeconds() * zoomSpeed);
			view.setCenter(viewPos);
		}

		viewPosDelta = viewPosStart - viewPos;

		// dampen the zoom
		targetZoom = lerp(targetZoom, currentZoom, elapsedTime.asSeconds() * zoomSpeed);

		if (isLeftMouseButtonPressed) {
			end = sf::Vector2f(mousePos);
		}

		// 		// if we are on a WIN32 system, system("cls") will clear the
		// console
		// 		// window,
		// // else we will use the clear() function
		// #ifdef _WIN32
		// 		system("cls");
		// #else
		// 		std::cout << "\033[2J\033[1;1H";
		// #endif

		// 		std::cout << "FPS: " << 1.f / elapsedTime.asSeconds() <<
		// std::endl; 		std::cout << "Zoom: " << currentZoom << std::endl;
		// 		std::cout << "Mouse Position: " << mousePos.x << ", " <<
		// mousePos.y
		// 		          << std::endl;
		// 		std::cout << "Mouse Position (View): " << mousePos.x -
		// viewPosDelta.x
		// 		          << ", " << mousePos.y + viewPosDelta.y << std::endl;
		// 		std::cout << "View Position Start: " << viewPosStart.x << ", "
		// 		          << viewPosStart.y << std::endl;
		// 		std::cout << "View Position: " << viewPos.x << ", " << viewPos.y
		// 		          << std::endl;
		// 		std::cout << "View Position Delta: " << viewPosDelta.x << ", "
		// 		          << viewPosDelta.y << std::endl;
		// 		std::cout << "View Size: " << view.getSize().x << ", "
		// 		          << view.getSize().y << std::endl;
		// 		std::cout << "Start: " << start.x << ", " << start.y <<
		// std::endl; 		std::cout << "End: " << end.x << ", " << end.y <<
		// std::endl; 		std::cout << "Slices: " << slices.size() <<
		// std::endl;

		window.clear();
		window.setView(view);

		window.draw(s);

		if (!isLeftMouseButtonPressed) {
			for (auto& frame : slices) {
				std::cout << "Slice: " << start.x + frame.left << ", " << start.y + frame.top << ", " << frame.width
				          << ", " << frame.height << std::endl;
				r.setPosition(start.x + frame.left, start.y + frame.top);
				r.setSize(sf::Vector2f(frame.width, frame.height));

				window.draw(r);
			}
		}

		// width: 200
		// height: 226 - 114 = 112
		// 94 / 2 = 47
		// draw a grid of these dimensions across the screen
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 11; j++) {
				r.setPosition(200 * i + 100, 200 * j + 94);
				r.setSize(sf::Vector2f(200, 200));
				window.draw(r);
			}
		}

		if (isLeftMouseButtonPressed) {
			roiRect.setPosition(start);
			roiRect.setSize(sf::Vector2f(end.x - start.x, end.y - start.y));
			window.draw(roiRect);
		}

		window.setView(window.getDefaultView());
		window.display();
	}

	// return se::Application::Get().Run();
	return 0;
}
