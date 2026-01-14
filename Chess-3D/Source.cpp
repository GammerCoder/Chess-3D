
#include "SFML/Graphics.hpp"
int main() {

    sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "Chess");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear();
            sf::Vector2u winsiz = window.getSize();
            float boardw = winsiz.x * 0.7;
            float boardz = std::min(boardw, (float)winsiz.y);
            float tilesize = boardz / 8.0f;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    sf::RectangleShape Tile({ tilesize,tilesize });
                    Tile.setPosition({ i * tilesize, j * tilesize });
                    if ((i + j) % 2 == 0) {
                        Tile.setFillColor(sf::Color(238, 238, 210));
                    }
                    else {
                        Tile.setFillColor(sf::Color(118,150,56));
                    }
                    window.draw(Tile);
                }
            }


        
        window.display();
    }

	return 0;
}