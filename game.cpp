#include "game.h"
#include "arrmaps.h"

Game::Game(Image& im, Image& vmIm) :
    window(VideoMode(WIDTH_MAP * 32, HEIGHT_MAP * 32), "Station Demo"),
    p(im, 100, 100, 70, 96, "Player1", map1.GetTileMap()),
    VMachine(vmIm, 334, 32, 120, 169, "vm", SafeRoom.GetTileMap()),
    text("", font, 30), floatingText("", font, 30), map1(ArrMap1, 1), map2(ArrMap2, 2),
    map3(ArrMap3, 3), map4(ArrMap4, 4), SafeRoom(ArrMap0, 0)
{
    gameTime = 0;
    hpDownPlayerTimer = 0;//���������� ��� ����� ��� ������������ ������ ����� ��������� �����
    createBulletsTimer = 0;//���������� ��� ����� ��� �������� ��������
    displayTextTimer = 0;//���������� ��� ����� ��� ��������� ������
    text1Ready, text2Ready = false;
    loadTextures();
}

Game::~Game() { //�� ������ ������
    for (it = Bullets.begin(); it != Bullets.end(); it++)//���������� �� ������
        it = Bullets.erase(it);

    for (eit = enemies.begin(); eit != enemies.end(); eit++)//���������� �� ������
        eit = enemies.erase(eit);

    //gunshotSound.stop(); // ���������� ����������!!!!
    //gostSound.stop();
    //backgroundSound.stop();
}

void Game::run() {
    backgroundMusic.play();
    while (window.isOpen()) {
        handleEvents();
        update();
        draw();
        checkOptionForWindow();
    }
}

void Game::loadTextures() {
    font.loadFromFile("MP Manga.ttf");

    text.setColor(Color::Red);//��������� ����� � �������
    text.setStyle(Text::Bold);//������ �����.
    floatingText.setStyle(Text::Bold);
    floatingText.setPosition(100, 400);//������ ������� ������

    easyEnemyImage.loadFromFile("images/enemy.png"); // ��������� ����������� �����
    easyEnemyImage.createMaskFromColor(Color(255, 255, 255)); //������� ����� ����

    miniBossImage.loadFromFile("images/miniboss.png"); // ��������� ����������� �����
    miniBossImage.createMaskFromColor(Color(255, 255, 255)); //������� ����� ����

    BulletImage.loadFromFile("images/bullet.png");//��������� �������� � ������ �����������
    BulletImage.createMaskFromColor(Color(255, 255, 255));
    BulletImage.createMaskFromColor(Color(0, 0, 0)); //������� ������ ����

    map_image.loadFromFile("images/map_new.png");//��������� ���� ��� �����
    map.loadFromImage(map_image);//�������� �������� ���������
    s_map.setTexture(map);//�������� �������� ��������

    safeRoom_image.loadFromFile("images/saferoom_map.png");//��������� ���� ��� ����� ���������� �������
    safeRoomMap.loadFromImage(safeRoom_image);//�������� �������� ���������
    s_SafeRoomMap.setTexture(safeRoomMap);//�������� �������� ��������

    backgroundMusic.openFromFile("sounds/Arseny-St-Hollow.wav");
    backgroundMusic.setVolume(30); // ������� ��������� (0-100)
    backgroundMusic.setLoop(true); // ��������� ���������������

    gunshotBuffer.loadFromFile("sounds/bullet.wav");
    gunshotSound.setBuffer(gunshotBuffer);
    gunshotSound.setVolume(20);

    gostBuffer.loadFromFile("sounds/gost_death.wav");
    gostSound.setBuffer(gostBuffer);
    gostSound.setVolume(20);

    VMTakeDamageBuf.loadFromFile("sounds/VMTakeDamage.wav");
    VMTakeDamage.setBuffer(VMTakeDamageBuf);
    VMTakeDamage.setVolume(20);

    VMDestroyBuf.loadFromFile("sounds/VMDestroy.wav");
    VMDestroy.setBuffer(VMDestroyBuf);
    VMDestroy.setVolume(80);
}

void Game::handleEvents() {
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();

        //�������� �� ������� ������� "E"
        if (event.type == Event::KeyPressed)
        {
            if ((event.key.code == Keyboard::E) && (createBulletsTimer > FIRE_SPEED) && (p.life))
                //�������� �� ������� ������� "E"
                //���� ��������� ������ 1 �������, �� ���� �� ���������
            {
                Bullets.push_back(new Bullet(BulletImage, p.x + 32, p.y + 50, 16, 16, "Bullet",
                    p.savedState, SafeRoom.GetTileMap()));
                createBulletsTimer = 0;//�������� ������
                gunshotSound.play();
            }
            if ((event.key.code == Keyboard::Space) && (VMachine.getRect().intersects(p.getRect())))
            {
                //����� ����� �� ��� ��� ����������� ��������� � ��������� � �� ������� ������� "space"
                if (VMachine.life)
                    VMachine.exchangeCoins(p);
                else {
                    displayTextTimer = 0;
                    floatingText.setString(BROKEN_VENDING);
                }
            }
        }
    }
}

void Game::update() {
    float time = clock.getElapsedTime().asMicroseconds();
    if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//������� ����� �
    //�������� ��� ������, ���� ��� �����. ������������� ��� time ��� �� ����.
    //��� �� ��������� ������ ����
    clock.restart();
    time /= 800;

    createBulletsTimer += time; //���������� �������
    hpDownPlayerTimer += time;

    //����� ���������� ������
    if ((p.playerKey != 0) && !text1Ready) {
        displayTextTimer = 0;
        floatingText.setString(PLAYER_TAKE_KEY);
        text1Ready = true;
    }
    if ((p.playerScore >= 50) && !text2Ready) {
        displayTextTimer = 0;
        floatingText.setString(TOO_MUCH_MONEY);
        text2Ready = true;
    }

    updateFloatingText(time);

    if (enemyAlsoCreatedOnMap && roomNumber != 0) { //���� ��� �� ���� ������� �� ������� �����, �� ������
        //��������� ������ ��������� �������
        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            float xr = 100 + rand() % 250; // ��������� ���������� ����� �� ���� ���� �� ��� �x�
            float yr = 100 + rand() % 250; // ��������� ���������� ����� �� ���� ���� �� ��� �y�
            //������� ������ � �������� � ������
            enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy", SafeRoom.GetTileMap()));
            //�������� � ����������� ������ ��������, ������ ��� � ��� ��� �������
            enemiesCount += 1; //��������� ������� ������
        }
        int countMiniBoss = rand() % 10; //� ������ ������� � ������������ 33% ����� ��������� ���� ��������� ����
        if (countMiniBoss <= 3) {
            float xr = 150 + rand() % 250;
            float yr = 150 + rand() % 250;
            enemies.push_back(new Enemy(miniBossImage, xr, yr, 96, 96, "MiniBoss", SafeRoom.GetTileMap()));
            //�������� � ����������� ������ ��������, ������ ��� � ��� ��� �������
            enemiesCount += 1; //��������� ������� ������
        }
        enemyAlsoCreatedOnMap = false;
    }

    p.update(time);
    VMachine.update(time);

    //�������� ������
    for (eit = enemies.begin(); eit != enemies.end(); eit++)
    {
        (*eit)->update(time); //��������� ����� update()
    }

    //�������� ����
    for (it = Bullets.begin(); it != Bullets.end(); it++)
    {
        (*it)->update(time); //��������� ����� update()
    }

    //��������� ������ �� ������� "�������" ���� � ������� ��
    for (it = Bullets.begin(); it != Bullets.end(); )//������� ��� ���������� �� ������ �� �����
    {// ���� ���� ������ �����, �� ������� ���
        if ((*it)->life == false) { it = Bullets.erase(it); }
        else it++;//� ���� �������� (����������) � ���� �������.
    }

    //��������� ������ �� ������� "�������" ������ � ������� ��
    for (eit = enemies.begin(); eit != enemies.end(); )//������� ��� ���������� �� ������ �� �����
    {// ���� ���� ������ �����, �� ������� ���
        if ((*eit)->life == false)
        {
            (*eit)->SpawnCoin();
            eit = enemies.erase(eit);
        }
        else eit++;//� ���� �������� (����������) � ���� �������.
    }

    //�������� ����������� ������ � �������
    //���� ����������� ���������, �� "health -= 20". ����� ����, ��� �������� ��������� �� 0,
    //����� ��������������� � ��������� ��������� "you are lose"
    if (p.life == true) {//���� ����� ���
        for (eit = enemies.begin(); eit != enemies.end(); eit++) {//����� �� ������ ������
            if ((p.getRect().intersects((*eit)->getRect())) && (((*eit)->name == "EasyEnemy") ||
                ((*eit)->name == "MiniBoss")))
            {
                if (hpDownPlayerTimer > 1000) {
                    p.health -= (*eit)->damage;
                    cout << "You take the damage!\n";
                    hpDownPlayerTimer = 0;//�������� ������
                }
            }
        }
    }

    //����������� ���� � ������
    for (eit = enemies.begin(); eit != enemies.end(); eit++) {//����� �� ������ ������
        for (it = Bullets.begin(); it != Bullets.end(); it++) {//�� ������ ����
            if (((*it)->getRect().intersects((*eit)->getRect())) && ((*it)->name == "Bullet") &&
                (((*eit)->name == "EasyEnemy") || ((*eit)->name == "MiniBoss")))
            {
                cout << "Exellent hit!\n";

                //��� ��������� ���� � ����� ���������� ��������
                (*eit)->health -= PLAYER_DAMAGE;
                if ((*eit)->health <= 0) {
                    (*eit)->life = false;
                    enemiesCount -= 1; //��������� ���������� ������ � ����
                    gostSound.play();
                    cout << "Enemy destroyed!\n";
                }
                (*it)->life = false;
            }
        }
    }
    //����������� ���� � ���������
    if (roomNumber == 0)
        for (it = Bullets.begin(); it != Bullets.end(); it++) {//����� �� ������ ����
            if (((*it)->getRect().intersects(VMachine.getRect())) && ((*it)->name == "Bullet"))
            {
                //��� ��������� ���� � �������� ���������� ��������
                VMachine.health -= PLAYER_DAMAGE;
                VMTakeDamage.play();
                if (VMachine.health <= 0 && VMachine.life) {
                    VMachine.life = false;
                    VMachine.sprite.setTextureRect(IntRect(VMachine.w, 0, VMachine.w, VMachine.h));
                    VMDestroy.play();
                    cout << "You destroyed the Vending Machine!\n";

                    srand(std::time(0));
                    int randN = rand() % 3 + 1;
                    for (int i = 0; i <= randN; i++)
                        VMachine.spawnFood();
                    VMachine.spawnKey();
                }
                (*it)->life = false;
            }
        }
}

void Game::updateFloatingText(float time) {
    displayTextTimer += time;
    if (displayTextTimer < displayTime + fadeTime) {
        // ������� ���������
        float alpha = 255.f * std::min(1.f, displayTextTimer / fadeTime);
        floatingText.setColor(sf::Color(0, 0, 0, alpha));
    }
    else if (displayTextTimer < displayTime + 2 * fadeTime) {
        // ������������
        float alpha = 255.f * std::max(0.f, (displayTime + fadeTime * 2 - displayTextTimer) / fadeTime);
        floatingText.setColor(sf::Color(0, 0, 0, alpha));
    }
    else {
        // ����� ��������� ����� - ������� �����
        floatingText.setString("");
    }
}

void Game::draw() {
    window.clear();

    if (enemiesCount == 0) p.killAllEnemies = true;

    changeMapLogic();

    //////////////////////������ ������ ����� � ������� � � ������������ �������/////////////////
    switch (roomNumber)
    {
    case 0:
        SafeRoom.draw(&s_SafeRoomMap, &window);
        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            (*it)->TileMap = SafeRoom.GetTileMap();
        }
        p.TileMap = SafeRoom.GetTileMap();
        break;
    case 1:
        map1.draw(&s_map, &window);
        for (eit = enemies.begin(); eit != enemies.end(); eit++)//������� ��� ���������� �� ������ �� �����
        {
            (*eit)->TileMap = map1.GetTileMap();
        }

        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            (*it)->TileMap = map1.GetTileMap();
        }

        p.TileMap = map1.GetTileMap();
        break;
    case 2: map2.draw(&s_map, &window);
        for (eit = enemies.begin(); eit != enemies.end(); eit++)//������� ��� ���������� �� ������ �� �����
        {
            (*eit)->TileMap = map2.GetTileMap();
        }

        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            (*it)->TileMap = map2.GetTileMap();
        }

        p.TileMap = map2.GetTileMap();
        break;
    case 3: map3.draw(&s_map, &window);
        for (eit = enemies.begin(); eit != enemies.end(); eit++)//������� ��� ���������� �� ������ �� �����
        {
            (*eit)->TileMap = map3.GetTileMap();
        }

        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            (*it)->TileMap = map3.GetTileMap();
        }

        p.TileMap = map3.GetTileMap();
        break;
    case 4: map4.draw(&s_map, &window);
        for (eit = enemies.begin(); eit != enemies.end(); eit++)//������� ��� ���������� �� ������ �� �����
        {
            (*eit)->TileMap = map4.GetTileMap();
        }

        for (it = Bullets.begin(); it != Bullets.end(); it++)
        {
            (*it)->TileMap = map4.GetTileMap();
        }

        p.TileMap = map4.GetTileMap();
        break;
    }           //���� ������ s_map ����� ������ ������ ��������� �� ������ ������,
                //� ����� ����� ����� ��������� ��-�������

    //�������� ���������� �������� � �������
    ostringstream playerHealthString, gameTimeString, playerScoreString;
    playerHealthString << p.health; gameTimeString << gameTime;//��������� ������
    playerScoreString << p.playerScore; //������� � ��� ����� �����, �� ���� ��������� ������
    text.setString("HP " + playerHealthString.str() + "\nTime: " +
        gameTimeString.str() + "\nScore: " + playerScoreString.str());//������ ������ ������
    text.setPosition(40, 27);//������ ������� ������
    window.draw(text);//������ ���� �����
    window.draw(floatingText);

    window.draw(p.sprite);//������ ������ ������� �p� ������ �Player�


    if (roomNumber == 0) window.draw(VMachine.sprite);
    ////////////////////////////////////////////////////////////////////

    //������ ������
    for (eit = enemies.begin(); eit != enemies.end(); eit++)
    {
        if ((*eit)->life) //���� ����� ����
            window.draw((*eit)->sprite); //������
    }
    //������ ����
    for (it = Bullets.begin(); it != Bullets.end(); it++)
    {
        if ((*it)->life) //���� ���� ����
            window.draw((*it)->sprite); //������ �������
    }

    window.display();
}

void Game::checkOptionForWindow() {
    ////////////////����� ������/////////////////
    if (p.life == false)
    {
        backgroundMusic.stop();
        DeathAnimation deathAnimation(window);
        deathAnimation.playAnimation(window);
    }
    ////////////////����� ����///////////////////////
    if ((roomNumber == 4) && map3.isPassed && p.killAllEnemies)
    {
        backgroundMusic.stop();
        Ending ending(window);
        ending.playAnimation();
        cout << "You won!!!" << endl;
    }
}

void Game::changeMapLogic() {
    if (roomNumber != p.numberOfRoom) //��� ����� �������� ������ ��� ����� �������
    {
        switch (roomNumber) { //p.numberOfRoom ����������� ������ ���� ��� ����� � ������� �����.
        case 1: map1.isPassed = true;
            break;
        case 2: map2.isPassed = true;
            break;
        case 3: map3.isPassed = true;
            break;
        case 4: map4.isPassed = true;
            break;
        }

        roomNumber = p.numberOfRoom; //��������� �������

        switch (roomNumber) { //���� ��� ������� ���� ��������
        case 1: if (!map1.isPassed) { enemyAlsoCreatedOnMap = true; p.killAllEnemies = false; }
              break;
        case 2: if (!map2.isPassed) { enemyAlsoCreatedOnMap = true; p.killAllEnemies = false; }
              break;
        case 3: if (!map3.isPassed) { enemyAlsoCreatedOnMap = true; p.killAllEnemies = false; }
              break;
        case 4: if (!map4.isPassed) { enemyAlsoCreatedOnMap = true; p.killAllEnemies = false; }
              break;
        }
    }
}
