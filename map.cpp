#include "map.h"

Map::Map(string abs[HEIGHT_MAP], int numb) {
    for (int i = 0; i < HEIGHT_MAP; i++) {
        TileMap[i] = abs[i];
    }
    isPassed = false;
    number = numb;
}

Map::Map(int numb) { isPassed = false; number = numb; }

void Map::randomMapGenerate() {//�������� ����������� �����
    int randomElementX = 0;//���������� ��� �������� ���������� �������� �� �����������
    int randomElementY = 0;//���������� ��� �������� ���������� �������� �� ���������
    int countStone = 5;//���������� ������
    while (countStone > 0) {
        randomElementX = 1 + rand() % (WIDTH_MAP - 1);//��������������� �������� �� �x� �� 1 ��
        //������ �����-1. ����������� ������� ����� �� �������� ����� ������� �����
        randomElementY = 1 + rand() % (HEIGHT_MAP - 1);//�� �y�
        if (TileMap[randomElementY][randomElementX] == ' ') {//���� ��������� ������ ������,
            TileMap[randomElementY][randomElementX] = 's'; //�� ������ ���� ������.
            countStone--;
        }
    }
}

string* Map::GetTileMap() {
    return TileMap;
}

void Map::draw(Sprite* s_map, RenderWindow* window) { //���� ����� ������ ��������� �� ������ ������,
    //����� ����� ����� ��������� ��-�������
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++)
        {
            if (TileMap[i][j] == ' ') s_map->setTextureRect(IntRect(0, 0, 32, 32));//���
            if (TileMap[i][j] == 's') s_map->setTextureRect(IntRect(32, 0, 32, 32));//�������
            if (TileMap[i][j] == '0') s_map->setTextureRect(IntRect(64, 0, 32, 32));//������
            if (TileMap[i][j] == 'f') s_map->setTextureRect(IntRect(96, 0, 32, 32));//�������� ����
            if (TileMap[i][j] == 'h') s_map->setTextureRect(IntRect(128, 0, 32, 32));//���

            if (TileMap[i][j] == 'k') s_map->setTextureRect(IntRect(291, 0, 32, 32));//���� !!!!!!!!!!!!!!!!!!!

            if ((TileMap[i][j] == '?') || (TileMap[i][j] == '/')) s_map->setTextureRect(IntRect(162, 0, 32, 32));
            //������������ �����, ����
            if ((TileMap[i][j] == '!') || (TileMap[i][j] == '1')) s_map->setTextureRect(IntRect(193, 0, 32, 32));
            //������������ �����, ���
            if ((TileMap[i][j] == '(') || (TileMap[i][j] == '9')) s_map->setTextureRect(IntRect(225, 0, 32, 32));
            //�������������� �����, ����
            if ((TileMap[i][j] == ')') || (TileMap[i][j] == '-')) s_map->setTextureRect(IntRect(257, 0, 32, 32));
            //�������������� �����, �����
            if (TileMap[i][j] == '_') s_map->setTextureRect(IntRect(322, 0, 32, 32));
            //����� � ������� (!!!�������� ������!!!)
            if (TileMap[i][j] == 'd') s_map->setTextureRect(IntRect(257, 0, 32, 32));//����� � ����� (???)
            s_map->setPosition(j * 32, i * 32);
            (*window).draw(*s_map);
        }
}
