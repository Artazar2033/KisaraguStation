#ifndef PLAYER
#define PLAYER

#include "entity.h"

////////////////////////////����� ������////////////////////////
class Player :public Entity {
public:
    int playerScore;//��� ���������� ����� ���� ������ � ������
    int playerKey;
    int numberOfRoom;
    bool killAllEnemies;
    int savedState;

    SoundBuffer doorBuffer;
    Sound doorSound;

    Player(Image& image, float X, float Y, int W, int H, string Name, string* MapMap);
    void control();
    //����� �������� ������������ � ���������� ����� � ������
    void checkCollisionWithMap(float Dx, float Dy);
    void checkCollisionWithDoor();
    void checkCollisionWithVM(float Dx, float Dy);

    void update(float time);
};

#endif // PLAYER
