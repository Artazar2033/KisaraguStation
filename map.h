#ifndef MAP
#define MAP

#include "libs.h"

const int HEIGHT_MAP = 20;//������ ����� ������
const int WIDTH_MAP = 25;//������ ����� ������

class Map {
public:
    int number;
    string TileMap[HEIGHT_MAP]; // = ������ � ������ ����� (����� ������������ ����������� ��� ����������)
    bool isPassed;
    Map(string abs[HEIGHT_MAP], int numb);
    Map(int numb);
    void randomMapGenerate();
    string* GetTileMap();
    void draw(Sprite* s_map, RenderWindow* window);
};

#endif // MAP
