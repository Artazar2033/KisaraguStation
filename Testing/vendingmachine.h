#ifndef VENDINGMACHINE
#define VENDINGMACHINE
#include "entity.h"
//#include "player.h"


class VendingMachine:public Entity {
public:
    int price;

    VendingMachine(Image &image, float X, float Y, int W, int H, string Name, string* MapMap);

  //  bool canAfford(Player& Player);

  //  void exchangeCoins(Player& Player);

     void SpawnLives();

     void update(float time); //метод "оживления/обновления" объекта класса.

     //void GainCoin
};


#endif // VENDINGMACHINE

