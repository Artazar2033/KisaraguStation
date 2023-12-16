#include "player.h"

Player::Player(Image &image, float X, float Y, int W, int H, string Name, string* MapMap)
                :Entity(image, X, Y, W, H, Name, MapMap){
    numberOfRoom = 1; //начальная комната - 1
    playerScore = 0;
    state = stay;
    if (name == "Player1"){
        //Задаем спрайту один прямоугольник для
        //вывода одного игрока. IntRect – для приведения типов
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }
}

void Player::SpawnCoin() //метод спавна монет (не используется)
{
    //пусто
}

void Player::control(){
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

void Player::checkCollisionWithMap(float Dx, float Dy) {
    for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
        for (int j = x / 32; j<(x + w) / 32; j++)
        {

            if ((TileMap[i][j+1] == '?')||TileMap[i][j+1] == '!')//если элемент - дверь
            {
                teleport(&i, &j, &numberOfRoom);
                cout << "You're front of the door" << endl;
            }

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

void Player::teleport(int* i, int* j, int* num) {
    int numb = *num;
    switch (numb) {
    case 1:
        x = 2 * 32;
        *num = 2;
        break;
    case 2:
        if (*i == 10){ //левая дверь
            x = 20 * 32;
            *num = 1;
        }
        if (*i == 18){ //нижняя дверь
            y = 2 * 32;
            //x = 10 * 32;
            *num = 3;
        }
        break;
    case 3:
        if (*i <= 4){ //верхняя дверь
            y = 10 * 32;
            *num = 2;
        }
        if (*i == 10){ //правая дверь
            x = 2 * 32;
            *num = 4;
        }
        break;
    case 4:
        x = 23 * 32;
        *num = 3;
        break;
    }
}

void Player::update(float time) //метод "оживления/обновления" объекта класса.
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
        x += dx*time; //движение по “X”
        checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
        y += dy*time; //движение по “Y”
        checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
        speed = 0; //обнуляем скорость, чтобы персонаж остановился.
        //state = stay;

        sprite.setPosition(x, y); //спрайт в позиции (x, y).
        if (health <= 0){ life = false; }//если жизней меньше 0, либо равно 0, то умираем
    }
}
