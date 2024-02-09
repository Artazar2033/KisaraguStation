#ifndef ENEMY
#define ENEMY

#include "entity.h"

////////////////////////////����� �����////////////////////////
class Enemy :public Entity {
public:
    int direction;//����������� �������� �����
    int damage;

    Enemy(Image& image, float X, float Y, int W, int H, string Name, string* MapMap);
    void checkCollisionWithMap(float Dx, float Dy);//�-��� �������� ������������ � ������
    void update(float time);
    void SpawnCoin(); //����� ������� ����� ������
};

#endif
