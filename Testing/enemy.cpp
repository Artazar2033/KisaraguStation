#include "enemy.h"

Enemy::Enemy(Image &image, float X, float Y, int W, int H, string Name, string* MapMap) :Entity(image, X,
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
void Enemy::checkCollisionWithMap(float Dx, float Dy)//ф-ция проверки столкновений с картой
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

void Enemy::update(float time)
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
