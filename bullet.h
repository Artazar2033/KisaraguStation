#ifndef BULLET
#define BULLET

# include "entity.h"

////////////////////////////����� ����////////////////////////
class Bullet :public Entity {//����� ����
public:
    int direction;//����������� ����
    //�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
    //��� ������� ����������� ����� ����
    Bullet(Image& image, float X, float Y, int W, int H, string Name, int dir, string* MapMap);
    void update(float time);
    //void SpawnCoin(); //����� ������ ����� (�� ������������)
};

#endif // BULLET
#pragma once
