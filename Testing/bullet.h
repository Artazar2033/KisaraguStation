#ifndef BULLET
#define BULLET

# include "entity.h"

////////////////////////////КЛАСС ПУЛИ////////////////////////
class Bullet :public Entity{//класс пули
public:
    int direction;//направление пули
    //всё так же, только взяли в конце состояние игрока (int dir)
    //для задания направления полёта пули
    Bullet(Image &image, float X, float Y, int W, int H, string Name, int dir, string* MapMap);
    void update(float time);
    //void SpawnCoin(); //метод спавна монет (не используется)
};


#endif // BULLET

