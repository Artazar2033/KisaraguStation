#ifndef PLAYER
#define PLAYER

#include "entity.h"

////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
    int playerScore;//эта переменная может быть только у игрока
    int playerKey;
    int numberOfRoom;
    bool killAllEnemies;
    int savedState;

    SoundBuffer doorBuffer;
    Sound doorSound;

    Player(Image& image, float X, float Y, int W, int H, string Name, string* MapMap);
    void control();
    //Метод проверки столкновений с элементами карты и дверью
    void checkCollisionWithMap(float Dx, float Dy);
    void checkCollisionWithDoor();
    void checkCollisionWithVM(float Dx, float Dy);

    void update(float time);
};

#endif // PLAYER
