 /* class VendingMachine {
private:
    int coins; // количество монет
    int lives; // количество жизней

    sf::Font font;
    sf::Text coinsText;
    sf::Text livesText;
    sf::Text instructionText;

public:
    VendingMachine() : coins(0), lives(0) {
        // Загружаем шрифт
        font.loadFromFile("arial.ttf");

        // Задаем начальные значения для текста
        coinsText.setFont(font);
        coinsText.setCharacterSize(24);
        coinsText.setPosition(50, 50);
        coinsText.setFillColor(sf::Color::White);

        livesText.setFont(font);
        livesText.setCharacterSize(24);
        livesText.setPosition(50, 100);
        livesText.setFillColor(sf::Color::White);

        instructionText.setFont(font);
        instructionText.setCharacterSize(24);
        instructionText.setPosition(50, 150);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setString("Нажмите пробел, чтобы обменять монеты на жизни.");

        // Создаем окно
        sf::RenderWindow window(sf::VideoMode(800, 600), "Vending Machine");

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                // При нажатии клавиши пробел обмениваем монеты на жизни
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                    if (coins >= 10) { // Проверяем, достаточно ли монет для обмена
                        coins -= 10;
                        lives++;
                        updateText(); // Обновляем текст с количеством монет и жизней
                    }
                }
            }

            window.clear();
            window.draw(coinsText);
            window.draw(livesText);
            window.draw(instructionText);
            window.display();
        }
    }

private:
    void updateText() {
        // Обновляем текст с количеством монет и жизней
        coinsText.setString("Монеты: " + std::to_string(coins));
        livesText.setString("Жизни: " + std::to_string(lives));
    }
};

int main() {
    VendingMachine game;
    return 0;
}
*/

#include "vendingmachine.h"

VendingMachine::VendingMachine(Image &image, float X, float Y, int W, int H, string Name, string* MapMap)
                :Entity(image, X, Y, W, H, Name, MapMap){

        price = 5;

        if (name == "vm"){
            //Задаем спрайту один прямоугольник для
            sprite.setPosition(0, 0, w, h);
        }
    }
}

bool canAfford(Player& player) {
    return player.playerScore >= price;
}

void exchangeCoins(Player& player) {
    if (canAfford(player)) {
        player.playerScore -= price;
        player.gainLife();
    }
}

/*
void draw(sf::RenderWindow& window) {
    // Отрисовка вендингового аппарата
    // ...
}
*/

