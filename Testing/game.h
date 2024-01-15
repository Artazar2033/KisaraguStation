#ifndef GAME
#define GAME

#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "vendingmachine.h"
#include "deathanimation.h"
#include "ending.h"

const int PLAYER_DAMAGE = 40; //урон от пули по врагу
const int FIRE_SPEED = 500; //скорострельность в мс
const int ENEMY_COUNT = 0; //максимальное количество врагов в игре

class Game {
public:
    Game(Image& im, Image& vmIm);

    ~Game(); //на всякий случай

    void run();

private:
    RenderWindow window;
    Clock clock;
    Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры
    Font font;
    Text text, floatingText;//объект текста для постоянных надписей и для всплывающих
    Image heroImage;
    Image vendingMachineImage;
    Image easyEnemyImage;
    Image BulletImage;
    Image map_image, safeRoom_image;//объект изображения для карты
    Texture map, safeRoomMap; //и второй для изменения стиля сейфрума без доп вырезания
    Sprite s_map, s_SafeRoomMap;
    list<Enemy*> enemies;
    list<Entity*> Bullets;
    list<Entity*>::iterator it;//итератор класса Entity
    list<Enemy*>::iterator eit;//итератор по классу Enemy
    Player p;
    VendingMachine VMachine;

    Music backgroundMusic; //3:58
    SoundBuffer gunshotBuffer, gostBuffer, VMTakeDamageBuf, VMDestroyBuf;
    Sound gunshotSound, gostSound, VMTakeDamage, VMDestroy;

    int roomNumber = 1;
    bool enemyAlsoCreatedOnMap = true;
    int enemiesCount = 0;
    int gameTime;
    int hpDownPlayerTimer;//Переменная под время для неуязвимости игрока после получения урона
    int createBulletsTimer;//Переменная под время для задержки выстрела
    int displayTimer;//Переменная под время для появления текста
    float displayTime, fadeTime;// таймеры для для появления и исчезновения всплывающего текста
    Map map1;
    Map map2;
    Map map3;
    Map map4;
    Map SafeRoom;

    void loadTextures();

    void handleEvents();

    void update();

    void draw();

    void checkOptionForWindow();

    void changeMapLogic();
};

#endif // GAME

