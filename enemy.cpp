#include "enemy.h"

Enemy::Enemy(Image& image, float X, float Y, int W, int H, string Name, string* MapMap) :Entity(image, X,
    Y, W, H, Name, MapMap) {
    if (name == "EasyEnemy") {
        //«адаем спрайту один пр€моугольник дл€
        //вывода одного игрока. IntRect Ц дл€ приведени€ типов
        sprite.setTextureRect(IntRect(0, 0, w, h));
        direction = rand() % (3); //Ќаправление движени€ врага задаЄм случайным образом
        //через генератор случайных чисел
        speed = 0.1;//задаем скорость. этот объект всегда двигаетс€
        dx = speed;
        damage = 20;
    }
    if (name == "MiniBoss") {
        sprite.setTextureRect(IntRect(0, 0, w, h));
        direction = rand() % (3);
        health = 150;
        speed = 0.15;//скорость и здоровье в полтора раза больше, чем у обычного призрака
        dx = speed;
        damage = 30;
    }
}
void Enemy::checkCollisionWithMap(float Dx, float Dy)//ф-ци€ проверки столкновений с картой
{
    for (int i = y / 32; i < (y + h) / 32; i++)//проходимс€ по элементам карты
        for (int j = x / 32; j < (x + w) / 32; j++)
        {
            if (TileMap[i][j] == '0')//если элемент - тайлик земли
            {
                if (Dy > 0) {
                    y = i * 32 - h; dy = -0.1;
                    direction = rand() % (3); //Ќаправление движени€ врага
                }//по Y
                if (Dy < 0) {
                    y = i * 32 + 32; dy = 0.1;
                    direction = rand() % (3);//Ќаправление движени€ врага
                }//столкновение с верхними кра€ми
                if (Dx > 0) {
                    x = j * 32 - w; dx = -0.1;
                    direction = rand() % (3);//Ќаправление движени€ врага
                }//с правым краем карты
                if (Dx < 0) {
                    x = j * 32 + 32; dx = 0.1;
                    direction = rand() % (3); //Ќаправление движени€ врага
                }// с левым краем карты
            }
        }
}

void Enemy::SpawnCoin()
{
    if (name == "EasyEnemy") {
        for (int i = y / 32; i < (y + h / 2) / 32; i++)//проходимс€ по элементам карты
            for (int j = x / 32; j < (x + w / 2) / 32; j++)
            {
                if (TileMap[i][j] == ' ')//если элемент - пустое поле
                {
                    TileMap[i][j] = 's';
                    break;
                }
            }
    }
    if (name == "MiniBoss") { //больше монет!
        for (int i = y / 32; i < (y + h + 1) / 32; i++)//проходимс€ по элементам карты
            for (int j = x / 32; j < (x + w + 1) / 32; j++)
            {
                if (TileMap[i][j] == ' ')//если элемент - пустое поле
                {
                    TileMap[i][j] = 's';
                    break;
                }
            }
    }
}

void Enemy::update(float time)
{
    if ((name == "EasyEnemy") || (name == "MiniBoss")) {//дл€ персонажа с таким именем логика будет такой
        if (life) {//провер€ем, жив ли враг
            switch (direction) {//делаютс€ различные действи€ в зависимости от состо€ни€
            case 0: {//состо€ние идти вправо
                dx = speed;
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
                break;
            }
            case 1: {//состо€ние идти влево
                dx = -speed;
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
                break;
            }
            case 2: {//идти вверх
                dy = -speed;
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
                break;
            }
            case 3: {//идти вниз
                dy = speed;
                CurrentFrame += 0.005 * time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
                break;
            }
            }
            x += dx * time; //движение по УXФ
            checkCollisionWithMap(dx, 0);//обрабатываем столкновение по ’
            y += dy * time; //движение по УYФ
            checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
            sprite.setPosition(x, y); //спрайт в позиции (x, y).
            if (health <= 0) { life = false; }//если жизней меньше 0, либо равно 0, то умираем
        }
    }
}
