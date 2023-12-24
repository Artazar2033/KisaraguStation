#ifndef ENTITY
#define ENTITY

#include "map.h"

////////////////////////////КЛАСС СУЩНОСТЬ////////////////////////
class Entity {
public:
    enum { left, right, up, down, stay} state;// тип перечисления - состояние объекта
    float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
    int w, h, health; //переменная “health”, хранящая жизни
    bool life; //переменная “life” жизнь, логическая
    Texture texture;//сфмл текстура
    Sprite sprite;//сфмл спрайт
    float CurrentFrame;//хранит текущий кадр
    string name;//враги могут быть разные, врагов можно различать по именам
    //каждому можно дать свое действие в update() в зависимости от имени

    string* TileMap;

    Entity(Image &image, float X, float Y, int W, int H, string Name, string* MapMap);
    FloatRect getRect();//метод получения прямоугольника
    virtual void update(float time) = 0;
    //virtual void SpawnCoin() = 0;
};

#endif // ENTITY

