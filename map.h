#ifndef MAP
#define MAP

#include "libs.h"

const int HEIGHT_MAP = 20;//размер карты высота
const int WIDTH_MAP = 25;//размер карты ширина

class Map {
public:
    int number;
    string TileMap[HEIGHT_MAP]; // = массив с маской карты (тогда использовать конструктор без параметров)
    bool isPassed;
    Map(string abs[HEIGHT_MAP], int numb);
    Map(int numb);
    void randomMapGenerate();
    string* GetTileMap();
    void draw(Sprite* s_map, RenderWindow* window);
};

#endif // MAP
