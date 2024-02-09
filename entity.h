#ifndef ENTITY
#define ENTITY

#include "map.h"

////////////////////////////����� ��������////////////////////////
class Entity {
public:
    // ��� ������������ - ��������� �������
    enum { left, right, up, down, upLeft, upRight, downLeft, downRight, stay } state;
    float dx, dy, x, y, speed, moveTimer;//�������� ���������� ������ ��� ������� �����
    int w, h, health; //���������� �health�, �������� �����
    bool life; //���������� �life� �����, ����������
    Texture texture;//���� ��������
    Sprite sprite;//���� ������
    float CurrentFrame;//������ ������� ����
    string name;//����� ����� ���� ������, ������ ����� ��������� �� ������
    //������� ����� ���� ���� �������� � update() � ����������� �� �����

    string* TileMap;

    Entity(Image& image, float X, float Y, int W, int H, string Name, string* MapMap);
    FloatRect getRect();//����� ��������� ��������������
    virtual void update(float time) = 0;
    //virtual void SpawnCoin() = 0;
};

#endif // ENTITY
