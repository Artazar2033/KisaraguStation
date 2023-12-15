#ifndef MAP
#define MAP

#include "libs.h"

const int HEIGHT_MAP = 20;//размер карты высота
const int WIDTH_MAP = 25;//размер карты ширина

class Map{
public:
    string TileMap[HEIGHT_MAP]; // = массив с маской карты (тогда использовать конструктор без параметров)

    Map(string abs[HEIGHT_MAP]);
    Map();
    void randomMapGenerate();
    string* GetTileMap();
    void draw(Sprite* s_map, RenderWindow* window);
};

#endif // MAP

