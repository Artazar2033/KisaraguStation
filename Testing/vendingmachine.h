#ifndef VENDINGMACHINE
#define VENDINGMACHINE
#include "entity.h"
#include "player.h"


class VendingMachine:public Entity {
public:
    int price;

    SoundBuffer dealBuffer;
    Sound dealSound;

    VendingMachine(Image &image, float X, float Y, int W, int H, string Name, string* MapMap);
    void exchangeCoins(Player& player);
    void spawnFood();
    void spawnKey();
    void update(float time); //метод "оживления/обновления" объекта класса.
};

#endif // VENDINGMACHINE

