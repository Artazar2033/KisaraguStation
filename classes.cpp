#include <iostream>
#include <sstream>
#include <list>
//#include <SFML/Graphics.hpp>
#include "map.h" //подключили код с картой

using namespace sf;
using namespace std;

////////////////////////////КЛАСС СУЩНОСТЬ////////////////////////
class Entity {
public:
    enum { left, right, up, down, stay} state;// тип перечисления - состояние объекта
    float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
    int w, h, health; //переменная “health”, хранящая жизни игрока
    bool life; //переменная “life” жизнь, логическая
    Texture texture;//сфмл текстура
    Sprite sprite;//сфмл спрайт
    float CurrentFrame;//хранит текущий кадр
    string name;//враги могут быть разные, врагов можно различать по именам
    //каждому можно дать свое действие в update() в зависимости от имени
    Entity(Image &image, float X, float Y, int W, int H, string Name){
        x = X; y = Y; //координата появления спрайта
        w = W; h = H;
        name = Name;
        moveTimer = 0;
        dx = 0; dy = 0;
        speed = 0;
        CurrentFrame = 0;
        health = 100;
        life = true; //инициализировали логическую переменную жизни, герой жив
        texture.loadFromImage(image); //заносим наше изображение в текстуру
        sprite.setTexture(texture); //заливаем спрайт текстурой
    }
    FloatRect getRect(){//метод получения прямоугольника. его коорд, размеры (шир,высот).
        FloatRect FR(x, y, w, h); // переменная FR типа FloatRect
        return FR;
        //Тип данных (класс) "sf::FloatRect" позволяет хранить четыре координаты прямоугольника
        //в нашей игре это координаты текущего расположения тайла на карте
        //далее это позволит спросить, есть ли ещё какой-либо тайл на этом месте
        //эта ф-ция нужна для проверки пересечений
    }
    virtual void update(float time) = 0;
};
////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
    int playerScore;//эта переменная может быть только у игрока
    Player(Image &image, float X, float Y, int W, int H, string Name) :Entity(image, X,
                                                                                   Y, W, H, Name){
        playerScore = 0;
        state = stay;
        if (name == "Player1"){
            //Задаем спрайту один прямоугольник для
            //вывода одного игрока. IntRect – для приведения типов
            sprite.setTextureRect(IntRect(0, 0, w, h));
        }
    }
    void control(){
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            state = left;
            speed = 0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            state = right;
            speed = 0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            state = up;
            speed = 0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            state = down;
            speed = 0.1;
        }
    }
    //Метод проверки столкновений с элементами карты
    void checkCollisionWithMap(float Dx, float Dy) {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j<(x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0')//если элемент тайлик земли
                {
                    if (Dy > 0) { y = i * 32 - h; dy = 0; }//по Y
                    if (Dy < 0) { y = i * 32 + 32; dy = 0; }//столкновение с верхними краями
                    if (Dx > 0) { x = j * 32 - w; dx = 0; }//с правым краем карты
                    if (Dx < 0) { x = j * 32 + 32; dx = 0; }// с левым краем карты
                }
                if (TileMap[i][j] == 's') {
                    playerScore++; //если взяли камень
                    TileMap[i][j] = ' ';
                }
                if (TileMap[i][j] == 'f') {
                    health -= 40;//если взяли ядовитый цветок
                    TileMap[i][j] = ' ';//убрали цветок
                }
                if (TileMap[i][j] == 'h') {
                    health += 20;//если взяли сердечко
                    TileMap[i][j] = ' ';//убрали сердечко
                }
            }
    }
    void update(float time) //метод "оживления/обновления" объекта класса.
    {
        if (life) {//проверяем, жив ли герой
            control();//функция управления персонажем
            switch (state)//делаются различные действия в зависимости от состояния
            {
            case right:{//состояние идти вправо
                dx = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 195, 96, 96));
                break;
            }
            case left:{//состояние идти влево
                dx = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 100, 96, 96));
                break;
            }
            case up:{//идти вверх
                dy = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 292, 96, 96));
                break;
            }
            case down:{//идти вниз
                dy = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                break;
            }
            case stay:{//стоим
                dy = speed;
                dx = speed;
                break;
            }
            }
            //Clock stay_time;
            x += dx*time; //движение по “X”
            checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
            y += dy*time; //движение по “Y”
            checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
            speed = 0; //обнуляем скорость, чтобы персонаж остановился.
            //state = stay;
            //stay_time.restart();

            sprite.setPosition(x, y); //спрайт в позиции (x, y).
            if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
        }
    }
};
////////////////////////////КЛАСС ВРАГА////////////////////////
class Enemy :public Entity{
public:
    int direction;//направление движения врага
    Enemy(Image &image, float X, float Y, int W, int H, string Name) :Entity(image, X,
                                                                                  Y, W, H, Name){
        if (name == "EasyEnemy"){
            //Задаем спрайту один прямоугольник для
            //вывода одного игрока. IntRect – для приведения типов
            sprite.setTextureRect(IntRect(0, 0, w, h));
            direction = rand() % (3); //Направление движения врага задаём случайным образом
            //через генератор случайных чисел
            speed = 0.1;//даем скорость.этот объект всегда двигается
            dx = speed;
        }
    }
    void checkCollisionWithMap(float Dx, float Dy)//ф-ция проверки столкновений с картой
    {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j < (x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0')//если элемент - тайлик земли
                {
                    if (Dy > 0) {
                        y = i * 32 - h; dy = -0.1;
                        direction = rand() % (3); //Направление движения врага
                    }//по Y
                    if (Dy < 0) {
                        y = i * 32 + 32; dy = 0.1;
                        direction = rand() % (3);//Направление движения врага
                    }//столкновение с верхними краями
                    if (Dx > 0) {
                        x = j * 32 - w; dx = -0.1;
                        direction = rand() % (3);//Направление движения врага
                    }//с правым краем карты
                    if (Dx < 0) {
                        x = j * 32 + 32; dx = 0.1;
                        direction = rand() % (3); //Направление движения врага
                    }// с левым краем карты
                }
            }
    }

    void update(float time)
    {
        if (name == "EasyEnemy"){//для персонажа с таким именем логика будет такой
            if (life) {//проверяем, жив ли герой
                switch (direction)//делаются различные действия в зависимости от состояния
                {
                case 0:{//состояние идти вправо
                    dx = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
                    break;
                }
                case 1:{//состояние идти влево
                    dx = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
                    break;
                }
                case 2:{//идти вверх
                    dy = -speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
                    break;
                }
                case 3:{//идти вниз
                    dy = speed;
                    CurrentFrame += 0.005*time;
                    if (CurrentFrame > 3) CurrentFrame -= 3;
                    sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                    break;
                }
                }
                x += dx*time; //движение по “X”
                checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
                y += dy*time; //движение по “Y”
                checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
                sprite.setPosition(x, y); //спрайт в позиции (x, y).
                if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
            }
        }
    }
};//класс Enemy закрыт
////////////////////////////КЛАСС ПУЛИ////////////////////////
class Bullet :public Entity{//класс пули
public:
    int direction;//направление пули
    //всё так же, только взяли в конце состояние игрока (int dir)
    //для задания направления полёта пули
    Bullet(Image &image, float X, float Y, int W, int H, string Name, int dir)
        :Entity(image, X, Y, W, H, Name){
        x = X;
        y = Y;
        direction = dir;
        speed = 0.8;
        w = h = 16;
        life = true;
        //выше инициализация в конструкторе
    }
    void update(float time)
    {
        switch (direction)
        {
        case 0: dx = -speed; dy = 0; break;// state = left
        case 1: dx = speed; dy = 0; break;// state = right
        case 2: dx = 0; dy = -speed; break;// state = up
        case 3: dx = 0; dy = speed; break;// state = down
        //default: dx = 0; dy = 0; break;
        }
        if (life){
            x += dx*time;//само движение пули по х
            y += dy*time;//по у
            if (x <= 0) x = 20;// задержка пули в левой стене, чтобы при проседании кадров
            //она случайно не вылетела за предел карты и не было ошибки (сервер может тормозить!)
            if (y <= 0) y = 20;
            if (x >= 800) x = 780;// задержка пули в правой стене, чтобы при проседании
            //кадров она случайно не вылетела за предел карты и не было ошибки (сервер может тормозить!)
            if (y >= 640) y = 620;
            for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
                for (int j = x / 32; j < (x + w) / 32; j++)
                {
                    if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то
                        life = false;// то пуля умирает
                }
            sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пули
        }
    }
};
