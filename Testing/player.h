#ifndef PLAYER
#define PLAYER

#include "entity.h"

////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
    int playerScore;//эта переменная может быть только у игрока
    Player(Image &image, float X, float Y, int W, int H, string Name, string* MapMap);
    void control();
    //Метод проверки столкновений с элементами карты

    void checkCollisionWithMap(float Dx, float Dy);//////////////////////////////////////////////

    void update(float time); //метод "оживления/обновления" объекта класса.
};

#endif // PLAYER

