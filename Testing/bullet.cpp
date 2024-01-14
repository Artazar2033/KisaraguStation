#include "bullet.h"

Bullet::Bullet(Image &image, float X, float Y, int W, int H, string Name, int dir, string* MapMap)
    :Entity(image, X, Y, W, H, Name, MapMap){
    x = X;
    y = Y;
    direction = dir;
    speed = 0.8;
    w = h = 16;
    life = true;
    //выше инициализация в конструкторе
}

/*void Bullet::SpawnCoin() //метод спавна монет (не используется)
{
    //пусто
}*/

void Bullet::update(float time)
{
    switch (direction)
    {
    case 0: dx = -speed; dy = 0; break;// state = left
    case 1: dx = speed; dy = 0; break;// state = right
    case 2: dx = 0; dy = -speed; break;// state = up
    case 3: dx = 0; dy = speed; break;// state = down
    case 4: dx = -speed; dy = 0; break;// state = upleft
    case 5: dx = speed; dy = 0; break;// state = upright
    case 6: dx = -speed; dy = 0; break;// state = downleft
    case 7: dx = speed; dy = 0; break;// state = downright
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
        char tile;
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j < (x + w) / 32; j++)
            {
                tile = TileMap[i][j];
                if ((tile == '0') || (tile == '?') || (tile == '!') || (tile == '(') || (tile == ')') ||
                        (tile == '/') || (tile == '1') || (tile == '9') || (tile == '-') || (tile == '_'))
                //если элемент тайлик земли или двери, то
                    life = false;// то пуля умирает
            }
        sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пули
    }
}

