#include "game.h"

int main() {
    Image image;
    image.loadFromFile("images/hero.png"); // загружаем изображение игрока
    image.createMaskFromColor(Color(255, 255, 255));

    Game game(image);
    game.run();
    return 0;
}
