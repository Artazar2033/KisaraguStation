#ifndef VENDINGMACHINE
#define VENDINGMACHINE

/*class Vending Machine{
public:
    Game() {
        // Создание окна игры
        window.create(sf::VideoMode(800, 600), "Coin Exchange");

        // Загрузка текстуры для объекта монеты
        coinTexture.loadFromFile("coin.png");
        coinSprite.setTexture(coinTexture);
        coinSprite.setPosition(100, 100);

        // Загрузка текстуры для объекта жизни
        lifeTexture.loadFromFile("life.png");
        lifeSprite.setTexture(lifeTexture);
        lifeSprite.setPosition(400, 300);

        // Переменные для отслеживания количества монет и жизней
        coinCount = 100;
        lifeCount = 100;
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                // Проверка на нажатие на монету
                if (coinSprite.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                    // Если есть монеты, обменять монету на жизнь
                    if (coinCount > 0) {
                        coinCount--;
                        lifeCount++;
                    }
                }
            }
        }
    }

    void update() {
        // Обновление состояния игры (можно добавить логику)
    }

    void render() {
        window.clear();

        // Отрисовка объектов
        window.draw(coinSprite);
        window.draw(lifeSprite);

        // Отрисовка количества монет и жизней
        sf::Font font;
        font.loadFromFile("arial.ttf");
        sf::Text coinText("Coins: " + std::to_string(coinCount), font);
        coinText.setCharacterSize(24);
        coinText.setFillColor(sf::Color::White);
        coinText.setPosition(10, 10);
        window.draw(coinText);

        sf::Text lifeText("Lives: " + std::to_string(lifeCount), font);
        lifeText.setCharacterSize(24);
        lifeText.setFillColor(sf::Color::White);
        lifeText.setPosition(10, 40);
        window.draw(lifeText);

        window.display();
    }

    sf::RenderWindow window;
    sf::Texture coinTexture;
    sf::Sprite coinSprite;
    sf::Texture lifeTexture;
    sf::Sprite lifeSprite;
    int coinCount;
    int lifeCount;
};

int main() {
    Game game;
    game.run();

    return 0;
}
*/

class VendingMachine:public Entity {
public:
    int price;

    VendinMachine(Image &image, float X, float Y, int W, int H, string Name, int dir, string* MapMap);

    bool canAfford(Player& Player);

    void exchangeCoins(Player& Player);
};


#endif // VENDINGMACHINE

