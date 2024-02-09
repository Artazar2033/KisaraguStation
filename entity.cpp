#include "entity.h"

Entity::Entity(Image& image, float X, float Y, int W, int H, string Name, string* MapMap) {
    x = X; y = Y; //���������� ��������� �������
    w = W; h = H;
    name = Name;
    moveTimer = 0;
    dx = 0; dy = 0;
    speed = 0;
    CurrentFrame = 0;
    health = 100;
    life = true; //���������������� ���������� ���������� �����, ����� ���
    texture.loadFromImage(image); //������� ���� ����������� � ��������
    sprite.setTexture(texture); //�������� ������ ���������

    TileMap = MapMap;
}

FloatRect Entity::getRect() {//����� ��������� ��������������. ��� �����, ������� (���,�����).
    FloatRect FR(x, y, w, h); // ���������� FR ���� FloatRect
    return FR;
    //��� ������ (�����) "sf::FloatRect" ��������� ������� ������ ���������� ��������������
    //� ����� ���� ��� ���������� �������� ������������ ����� �� �����
    //����� ��� �������� ��������, ���� �� ��� �����-���� ���� �� ���� �����
    //��� �-��� ����� ��� �������� �����������
}
