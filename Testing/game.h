#ifndef GAME
#define GAME
#ifndef GAME

#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "vendingmachine.h"

const int PLAYER_DAMAGE = 40; //урон от пули по врагу
const int FIRE_SPEED = 500; //скорострельность в мс
const int ENEMY_COUNT = 3; //максимальное количество врагов в игре

class Game {
    void run();
    RenderWindow window;
    Clock clock;
    Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры
    Font font;
    Text text;
    Image heroImage;
    Image vendingMachineImage;
    Image easyEnemyImage;
    Image BulletImage;
    Image map_image;//объект изображения для карты
    Texture map;
    Sprite s_map;
    list<Enemy*> enemies;
    list<Entity*> Bullets;
    list<Entity*>::iterator it;//итератор класса Entity
    list<Enemy*>::iterator eit;//итератор по классу Enemy
    Player p;
    int roomNumber = 1;
    bool enemyAlsoCreatedOnMap = true;
    int enemiesCount = 0;
    int gameTime;
    int hpDownPlayerTimer;//Переменная под время для неуязвимости игрока после получения урона
    int createBulletsTimer;//Переменная под время для задержки выстрела
    Map map1;
    Map map2;
    Map map3;
    Map map4;

    void loadTextures();

    void handleEvents();

    void update();

    void draw();

    void changeMapLogic();
};

#endif // GAME

