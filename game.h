#ifndef GAME
#define GAME

#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "vendingmachine.h"
#include "deathanimation.h"
#include "ending.h"

const int PLAYER_DAMAGE = 40; //���� �� ���� �� �����
const int FIRE_SPEED = 500; //���������������� � ��
const int ENEMY_COUNT = 3; //������������ ���������� ������ � ����
const string PLAYER_TAKE_KEY = "You picked up the mysterious key...\n How did he get into this machine?";
const string TOO_MUCH_MONEY = "I have more than 50 coins...\n Maybe I should buy something?";
const string BROKEN_VENDING = "Vending machine is brocken.\nWhat are you trying to achieve?";

class Game {
public:
    Game(Image& im, Image& vmIm);

    ~Game(); //�� ������ ������

    void run();

private:
    RenderWindow window;
    Clock clock;
    Clock gameTimeClock;//���������� �������� �������, ����� ����� ������� ����� ����
    Font font;
    Text text, floatingText;//������ ������ ��� ���������� �������� � ��� �����������
    Image heroImage;
    Image vendingMachineImage;
    Image easyEnemyImage;
    Image miniBossImage;
    Image BulletImage;
    Image map_image, safeRoom_image;//������ ����������� ��� �����
    Texture map, safeRoomMap; //� ������ ��� ��������� ����� �������� ��� ��� ���������
    Sprite s_map, s_SafeRoomMap;
    list<Enemy*> enemies;
    list<Entity*> Bullets;
    list<Entity*>::iterator it;//�������� ������ Entity
    list<Enemy*>::iterator eit;//�������� �� ������ Enemy
    Player p;
    VendingMachine VMachine;

    Music backgroundMusic; //3:58
    SoundBuffer gunshotBuffer, gostBuffer, VMTakeDamageBuf, VMDestroyBuf;
    Sound gunshotSound, gostSound, VMTakeDamage, VMDestroy;

    int roomNumber = 1;
    bool enemyAlsoCreatedOnMap = true;
    int enemiesCount = 0;
    int gameTime;
    int hpDownPlayerTimer;//���������� ��� ����� ��� ������������ ������ ����� ��������� �����
    int createBulletsTimer;//���������� ��� ����� ��� �������� ��������
    int displayTextTimer;//���������� ��� ����� ��� ��������� ������
    const float displayTime = 3000, fadeTime = 1000;// ������� ��� ��� ��������� � ������������ ������������ ������
    bool text1Ready, text2Ready; //���������� ��� ������������ ���������� �������
    Map map1;
    Map map2;
    Map map3;
    Map map4;
    Map SafeRoom;

    void loadTextures();

    void handleEvents();

    void update();
    void updateFloatingText(float time);

    void draw();

    void checkOptionForWindow();

    void changeMapLogic();
};

#endif // GAME
