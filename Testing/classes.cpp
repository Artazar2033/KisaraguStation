#include "player.h" //подключили код с картой

////////////////////////////КЛАСС ВРАГА////////////////////////
class Enemy :public Entity{
public:
    int direction;//направление движения врага
    Enemy(Image &image, float X, float Y, int W, int H, string Name, string* MapMap) :Entity(image, X,
                                                                                  Y, W, H, Name, MapMap){
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
    Bullet(Image &image, float X, float Y, int W, int H, string Name, int dir, string* MapMap)
        :Entity(image, X, Y, W, H, Name, MapMap){
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
