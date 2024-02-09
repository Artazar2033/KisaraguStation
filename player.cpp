#include "player.h"

Player::Player(Image& image, float X, float Y, int W, int H, string Name, string* MapMap)
    :Entity(image, X, Y, W, H, Name, MapMap) {
    numberOfRoom = 1; //��������� ������� - 1
    killAllEnemies = false;
    playerScore = 1000; //������
    playerKey = 0; //�����
    state = stay;
    if (name == "Player1") {
        //������ ������� ���� ������������� ���
        //������ ������ ������. IntRect � ��� ���������� �����
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }
    doorBuffer.loadFromFile("sounds/door.wav");
    doorSound.setBuffer(doorBuffer);
    doorSound.setVolume(50);
}

void Player::control() {
    // ���������� ��� ������������ ������� ������
    bool keyA = Keyboard::isKeyPressed(Keyboard::A);
    bool keyD = Keyboard::isKeyPressed(Keyboard::D);
    bool keyW = Keyboard::isKeyPressed(Keyboard::W);
    bool keyS = Keyboard::isKeyPressed(Keyboard::S);

    if (keyA) {
        state = left;
        speed = 0.1;
    }
    if (keyD) {
        state = right;
        speed = 0.1;
    }
    if (keyW) {
        state = up;
        speed = 0.1;
    }
    if (keyS) {
        state = down;
        speed = 0.1;
    }
    //������� �������� � ������������ ������������
    if (keyW && keyA) {
        state = upLeft;
        speed = 0.1;
    }
    if (keyW && keyD) {
        state = upRight;
        speed = 0.1;
    }
    if (keyS && keyA) {
        state = downLeft;
        speed = 0.1;
    }
    if (keyS && keyD) {
        state = downRight;
        speed = 0.1;
    }
}

void Player::update(float time) //����� "���������/����������" ������� ������.
{
    if (life) {//���������, ��� �� �����
        control();//������� ���������� ����������
        switch (state)//�������� ��������� �������� � ����������� �� ���������
        {
        case right: {//��������� ���� ������
            dx = speed;
            dy = 0;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 195, 96, 96));
            break;
        }
        case left: {//��������� ���� �����
            dx = -speed;
            dy = 0;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 100, 96, 96));
            break;
        }
        case up: {//���� �����
            dx = 0;
            dy = -speed;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 292, 96, 96));
            break;
        }
        case down: {//���� ����
            dx = 0;
            dy = speed;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
            break;
        }
        case upLeft: // ���� �����-�����
            dx = -speed;
            dy = -speed;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 100, 96, 96));
            break;

        case upRight: // ���� �����-������
            dx = speed;
            dy = -speed;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 195, 96, 96));
            break;

        case downLeft: // ���� ����-�����
            dx = -speed;
            dy = speed;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 100, 96, 96));
            break;

        case downRight: // ���� ����-������
            dx = speed;
            dy = speed;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 195, 96, 96));
            break;
        case stay: {//�����
            dy = 0;
            dx = 0;
            break;
        }
        }
        x += dx * time; //�������� �� �X�
        checkCollisionWithMap(dx, 0);//������������ ������������ �� �
        if (numberOfRoom == 0) checkCollisionWithVM(dx, 0);
        y += dy * time; //�������� �� �Y�
        checkCollisionWithMap(0, dy);//������������ ������������ �� Y
        if (numberOfRoom == 0) checkCollisionWithVM(0, dy);

        checkCollisionWithDoor();

        speed = 0; //�������� ��������, ����� �������� �����������.
        if (state != stay) savedState = state;
        state = stay;

        sprite.setPosition(x, y); //������ � ������� (x, y).
        if (health <= 0)
        {
            life = false; cout << "You're dead!" << endl;
        }//���� ������ ������ 0, ���� ����� 0, �� �������
    }
}

void Player::checkCollisionWithMap(float Dx, float Dy) {
    for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
        for (int j = x / 32; j < (x + w) / 32; j++)
        {
            if (TileMap[i][j] == '0')//���� ������� ������ �����
            {
                if (Dy > 0) { y = i * 32 - h; dy = 0; }//�� Y
                if (Dy < 0) { y = i * 32 + 32; dy = 0; }//������������ � �������� ������
                if (Dx > 0) { x = j * 32 - w; dx = 0; }//� ������ ����� �����
                if (Dx < 0) { x = j * 32 + 32; dx = 0; }// � ����� ����� �����
            }
            if (TileMap[i][j] == 's') {
                playerScore++; //���� ����� ������
                TileMap[i][j] = ' ';
            }
            if (TileMap[i][j] == 'f') {
                health -= 40;//���� ��������� � �������� ����
                TileMap[i][j] = ' ';//������� ����
            }
            if (TileMap[i][j] == 'h') {
                health += 20;//���� ����� ��������
                TileMap[i][j] = ' ';//������ ��������
            }
            if (TileMap[i][j] == 'k') {
                playerKey++; //���� ����� ����
                TileMap[i][j] = ' ';
            }
        }
}

void Player::checkCollisionWithVM(float Dx, float Dy) {
    for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
        for (int j = x / 32; j < (x + w) / 32; j++)
        {
            if ((j < (334 + 120) / 32) && (j > (334 - 32) / 32) && (i > 32 / 32) && (i < (32 + 169) / 32))
                //���� ������� - ���� �� ������ ��������
            {
                if (Dy > 0) { y = i * 32 - h; dy = 0; }//�� Y
                if (Dy < 0) { y = i * 32 + 32; dy = 0; }//������������ � �������� ������
                if (Dx > 0) { x = j * 32 - w; dx = 0; }//� ������ ����� �����
                if (Dx < 0) { x = j * 32 + 32; dx = 0; }// � ����� ����� �����
            }
        }
}

void Player::checkCollisionWithDoor() {
    int numb = numberOfRoom;
    int nextRoom = -1;  // ������������� ���������� ��� �������� ��������� �������
    int oldI, oldJ; //���������� ��� ����������� ������� � ������ �������

    for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����, ��������������� � �������
        for (int j = x / 32; j < (x + w) / 32; j++)
        {
            enum { left, right, up, down, none } checkDoor;
            checkDoor = none;
            // ��������, ��������� �� ����� ����� � ������
            if ((TileMap[i][j + 1] == '?' || TileMap[i][j - 1] == '?' ||
                TileMap[i + 1][j] == '?' || TileMap[i - 1][j] == '?') ||
                (TileMap[i][j + 1] == '!' || TileMap[i][j - 1] == '!' ||
                    TileMap[i + 1][j] == '!' || TileMap[i - 1][j] == '!')) //����
                checkDoor = left;
            if ((TileMap[i][j + 1] == '/' || TileMap[i][j - 1] == '/' ||
                TileMap[i + 1][j] == '/' || TileMap[i - 1][j] == '/') ||
                (TileMap[i][j + 1] == '1' || TileMap[i][j - 1] == '1' ||
                    TileMap[i + 1][j] == '1' || TileMap[i - 1][j] == '1')) //�����
                checkDoor = right;
            if ((TileMap[i][j + 1] == '(' || TileMap[i][j - 1] == '(' ||
                TileMap[i + 1][j] == '(' || TileMap[i - 1][j] == '(') ||
                (TileMap[i][j + 1] == ')' || TileMap[i][j - 1] == ')' ||
                    TileMap[i + 1][j] == ')' || TileMap[i - 1][j] == ')')) //����
                checkDoor = up;
            if ((TileMap[i][j + 1] == '9' || TileMap[i][j - 1] == '9' ||
                TileMap[i + 1][j] == '9' || TileMap[i - 1][j] == '9') ||
                (TileMap[i][j + 1] == '-' || TileMap[i][j - 1] == '-' ||
                    TileMap[i + 1][j] == '-' || TileMap[i - 1][j] == '-')) //���
                checkDoor = down;
            if ((TileMap[i][j + 1] == '_' || TileMap[i][j - 1] == '_' || //��� ��������
                TileMap[i + 1][j] == '_' || TileMap[i - 1][j] == '_'))
                checkDoor = up;
            if (checkDoor != none) {
                // ������ ��� ����������� ��������� �������
                switch (numb) {
                case 0:
                    //����� �� �������� ������ ����
                    nextRoom = 1;
                    oldJ = WIDTH_MAP / 2 + 2; // x
                    oldI = HEIGHT_MAP - 2; // y
                    break;
                case 1:
                    if (checkDoor == right) {//����� ������
                        nextRoom = 2; //����� ������ ������ ������
                        oldJ = WIDTH_MAP - 2;
                        oldI = HEIGHT_MAP / 2 + 2;
                    }
                    if (checkDoor == up) { //����� � �������
                        nextRoom = 0;
                        oldJ = WIDTH_MAP / 2 + 2;
                        oldI = 5;
                    }
                    break;
                case 2:
                    if (checkDoor == left) { //����� �����
                        nextRoom = 1;
                        oldJ = 5;
                        oldI = HEIGHT_MAP / 2 + 2;
                    }
                    if (checkDoor == down) { //����� �����
                        nextRoom = 3;
                        oldJ = WIDTH_MAP / 2 + 2; // x
                        oldI = HEIGHT_MAP - 2; // y
                    }
                    break;
                case 3:
                    if (checkDoor == up) { //����� ������
                        nextRoom = 2;
                        oldJ = WIDTH_MAP / 2 + 2;
                        oldI = 5;
                    }
                    if (checkDoor == right) {//����� ������
                        nextRoom = 4;
                        oldJ = WIDTH_MAP - 2;
                        oldI = HEIGHT_MAP / 2 + 2;
                    }
                    break;
                case 4:
                    if (checkDoor == left) {
                        nextRoom = 3; //����� ������ ������ �����
                        oldJ = 5;
                        oldI = HEIGHT_MAP / 2 + 2;
                    }
                    break;
                }
            }
        }

    // ��������� �������� � ��������� �������
    if (nextRoom != -1) {
        if (killAllEnemies) {
            numberOfRoom = nextRoom;  // ���������� ������ ������� �������

            doorSound.play(); // ��������������� �����

            int oldX = (oldJ * 32);  // X-���������� ������ �����
            int oldY = (oldI * 32);  // Y-���������� ������ �����

            // ���������� ����� ��������� � ����������� �� ������
            x = (WIDTH_MAP) * 32 - oldX;
            y = (HEIGHT_MAP) * 32 - oldY;

            cout << "You're in front of the door" << endl;
        }
        else { cout << "You should kill the enemies first!" << endl; }
    }
}
