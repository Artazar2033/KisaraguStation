#include "player.h"

Player::Player(Image &image, float X, float Y, int W, int H, string Name, string* MapMap)
                :Entity(image, X, Y, W, H, Name, MapMap){
    numberOfRoom = 1; //начальная комната - 1
    killAllEnemies = false;
    playerScore = 0; //монеты
    state = stay;
    if (name == "Player1"){
        //Задаем спрайту один прямоугольник для
        //вывода одного игрока. IntRect – для приведения типов
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }
    doorBuffer.loadFromFile("sounds/door.wav");
    doorSound.setBuffer(doorBuffer);
}

/*void Player::SpawnCoin() //метод спавна монет (не используется)
{
    //пусто
}*/

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
            if (TileMap[i][j] == '0')//если элемент тайлик земли
            {
                if (Dy > 0) { y = i * 32 - h; dy = 0; }//по Y
                if (Dy < 0) { y = i * 32 + 32; dy = 0; }//столкновение с верхними краями
                if (Dx > 0) { x = j * 32 - w; dx = 0; }//с правым краем карты
                if (Dx < 0) { x = j * 32 + 32; dx = 0; }// с левым краем карты
            }
            if (TileMap[i][j] == 's') {
                playerScore++; //если взяли монету
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

void Player::checkCollisionWithDoor(){
    int numb = numberOfRoom;
    int nextRoom = -1;  // Инициализация переменной для хранения следующей комнаты
    int oldI, oldJ; //переменные для запоминания позиции в каждой комнате

    for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты, соприкасающихся с игроком
        for (int j = x / 32; j<(x + w) / 32; j++)
        {
            int checkDoor = -1;
            // Проверка, находится ли игрок рядом с дверью
            if ((TileMap[i][j + 1] == '?' || TileMap[i][j - 1] == '?' ||
                TileMap[i + 1][j] == '?' || TileMap[i - 1][j] == '?')||
                (TileMap[i][j + 1] == '!' || TileMap[i][j - 1] == '!' ||
                TileMap[i + 1][j] == '!' || TileMap[i - 1][j] == '!')) //право лево
                    checkDoor = 0;
            if ((TileMap[i][j + 1] == '(' || TileMap[i][j - 1] == '(' ||
                TileMap[i + 1][j] == '(' || TileMap[i - 1][j] == '(')||
                (TileMap[i][j + 1] == ')' || TileMap[i][j - 1] == ')' ||
                TileMap[i + 1][j] == ')' || TileMap[i - 1][j] == ')')) //верх низ
                    checkDoor = 1;
            if (checkDoor != -1){
            // Логика для определения следующей комнаты
            switch (numb) {
                case 1:
                    nextRoom = 2; //дверь всегда только справа
                    oldJ = WIDTH_MAP - 2;
                    oldI = HEIGHT_MAP/2 + 2;
                    break;
                case 2:
                    if (!checkDoor) { //дверь слева
                        nextRoom = 1;
                        oldJ = 5;
                        oldI = HEIGHT_MAP/2 + 2;
                    }
                    else { //дверь снизу
                        nextRoom = 3;
                        oldJ = WIDTH_MAP/2 + 2; // x
                        oldI = HEIGHT_MAP - 2; // y
                    }
                    break;
                case 3:
                    if (checkDoor) { //дверь сверху
                        nextRoom = 2;
                        oldJ = WIDTH_MAP/2 + 2;
                        oldI = 5;
                    }
                    else {//дверь справа
                        nextRoom = 4;
                        oldJ = WIDTH_MAP - 2;
                        oldI = HEIGHT_MAP/2 + 2;
                    }
                    break;
                case 4:
                    nextRoom = 3; //дверь всегда только слева
                    oldJ = 5;
                    oldI = HEIGHT_MAP/2 + 2;
                    break;
                }
            }
        }

    // Обработка перехода в следующую комнату
    if (nextRoom != -1) {
        if (killAllEnemies){
            numberOfRoom = nextRoom;  // Обновление номера текущей комнаты

            doorSound.play(); // Воспроизведение звука

            int oldX = (oldJ * 32);  // X-координата центра двери
            int oldY = (oldI * 32);  // Y-координата центра двери

            // Вычисление новых координат в зависимости ОТ СТАРЫХ
            x = (WIDTH_MAP)*32 - oldX;
            y = (HEIGHT_MAP)*32 - oldY;

            cout << "You're in front of the door" << endl;
        }
        else {cout << "You should kill the enemies first!" << endl;}
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

        checkCollisionWithDoor();

        speed = 0; //обнуляем скорость, чтобы персонаж остановился.
        //state = stay;

        sprite.setPosition(x, y); //спрайт в позиции (x, y).
        if (health <= 0)
        {
            life = false; cout << "You're dead!" << endl;
        }//если жизней меньше 0, либо равно 0, то умираем
    }
}


/*void Player::gainCoin(){
     playerScore++;
}

void GainLife() {
        {
            health += 20;
        }*/
