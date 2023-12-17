#ifndef PLAYER
#define PLAYER

#include "entity.h"

////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
    int playerScore;//эта переменная может быть только у игрока
    int numberOfRoom;
    Player(Image &image, float X, float Y, int W, int H, string Name, string* MapMap);
    void control();
    //Метод проверки столкновений с элементами карты
    void checkCollisionWithMap(float Dx, float Dy);//////////////////////////////////////////////

    //void teleport(int* i, int* j, int* num); //метод проверки столкновения с комнатой
    void checkCollisionWithDoor();

    void update(float time); //метод "оживления/обновления" объекта класса.

   // void gainCoin();
};

#endif // PLAYER

