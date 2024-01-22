#include "game.h"

int main() {
    Image GGimage, vmImage;
    GGimage.loadFromFile("images/hero.png"); // загружаем изображение игрока
    GGimage.createMaskFromColor(Color(255, 255, 255));

    vmImage.loadFromFile("images/vending.png"); // загружаем изображение автомата с едой
    vmImage.createMaskFromColor(Color(255, 255, 255));

    Game game(GGimage, vmImage);
    game.run();
    return 0;
}
