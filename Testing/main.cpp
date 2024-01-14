#include "game.h"

int main() {
    Image image, vmImage;
    image.loadFromFile("images/hero.png"); // загружаем изображение игрока
    image.createMaskFromColor(Color(255, 255, 255));

    vmImage.loadFromFile("images/vending.png"); // загружаем изображение автомата с едой
    vmImage.createMaskFromColor(Color(255, 255, 255));

    Game game(image, vmImage);
    game.run();
    return 0;
}
