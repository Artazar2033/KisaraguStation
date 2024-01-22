#include "game.h"
#include "arrmaps.h"

Game::Game(Image& im, Image& vmIm):
    window(VideoMode(WIDTH_MAP * 32, HEIGHT_MAP * 32), "Station Demo"),
    p(im, 100, 100, 70, 96, "Player1", map1.GetTileMap()),
    VMachine(vmIm, 334, 32, 120, 169, "vm", SafeRoom.GetTileMap()),
    text("", font, 30), floatingText("", font, 30), map1(ArrMap1, 1), map2(ArrMap2, 2),
    map3(ArrMap3, 3), map4(ArrMap4, 4), SafeRoom(ArrMap0, 0)
{
    gameTime = 0;
    hpDownPlayerTimer = 0;//Переменная под время для неуязвимости игрока после получения урона
    createBulletsTimer = 0;//Переменная под время для задержки выстрела
    displayTextTimer = 0;//Переменная под время для появления текста
    text1Ready, text2Ready = false;

    loadTextures();
}

Game::~Game() { //на всякий случай
    for (it = Bullets.begin(); it != Bullets.end(); it++)//проходимся по списку
        it = Bullets.erase(it);

    for (eit = enemies.begin(); eit != enemies.end(); eit++)//проходимся по списку
        eit = enemies.erase(eit);

    //gunshotSound.stop(); // Деструктор пригодился!!!!
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

    text.setColor(Color::Red);//покрасили текст в красный
    text.setStyle(Text::Bold);//жирный текст.
    floatingText.setStyle(Text::Bold);
    floatingText.setPosition(100, 400);//задаем позицию текста

    //heroImage.loadFromFile("images/hero.png"); // загружаем изображение игрока
    //heroImage.createMaskFromColor(Color(255, 255, 255));

    easyEnemyImage.loadFromFile("images/enemy.png"); // загружаем изображение врага
    easyEnemyImage.createMaskFromColor(Color(255, 255, 255)); //убираем белый цвет

    miniBossImage.loadFromFile("images/miniboss.png"); // загружаем изображение врага
    miniBossImage.createMaskFromColor(Color(255, 255, 255)); //убираем белый цвет

    BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения
    BulletImage.createMaskFromColor(Color(255, 255, 255));
    BulletImage.createMaskFromColor(Color(0, 0, 0)); //убираем черный цвет

    map_image.loadFromFile("images/map_new.png");//загружаем файл для карты
    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    s_map.setTexture(map);//заливаем текстуру спрайтом

    safeRoom_image.loadFromFile("images/saferoom_map.png");//загружаем файл для карты безопасной комнаты
    safeRoomMap.loadFromImage(safeRoom_image);//заряжаем текстуру картинкой
    s_SafeRoomMap.setTexture(safeRoomMap);//заливаем текстуру спрайтом

    backgroundMusic.openFromFile("sounds/Arseny-St-Hollow.wav");
    backgroundMusic.setVolume(30); // Уровень громкости (0-100)
    backgroundMusic.setLoop(true); // Повторять воспроизведение

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

        //стреляем по нажатию клавиши "E"
        if (event.type == Event::KeyPressed)
        {
            if ((event.key.code == Keyboard::E) && (createBulletsTimer > FIRE_SPEED) && (p.life))
                //стреляем по нажатию клавиши "E"
                //если нарастили меньше 1 секунды, то пуля не рождается
            {
                Bullets.push_back(new Bullet(BulletImage, p.x+32, p.y+50, 16, 16, "Bullet",
                                             p.savedState, SafeRoom.GetTileMap()));
                createBulletsTimer = 0;//обнуляем таймер
                gunshotSound.play();
            }
            if ((event.key.code == Keyboard::Space)&&(VMachine.getRect().intersects(p.getRect())))
            {
                //обмен монет на еду при пересечении персонажа с автоматом и по нажатию клавиши "space"
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
    if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в
            //секундах идёт вперед, пока жив игрок. Перезагружать как time его не надо.
            //оно не обновляет логику игры
    clock.restart();
    time /= 700;

    createBulletsTimer += time; //наращиваем таймеры
    hpDownPlayerTimer += time;

    //Вывод временного текста
    if ((p.playerKey != 0) && !text1Ready) {
            displayTextTimer = 0;
            floatingText.setString(PLAYER_TAKE_KEY);
            text1Ready = true;
    }
    if ((p.playerScore >= 50) && !text2Ready){
            displayTextTimer = 0;
            floatingText.setString(TOO_MUCH_MONEY);
            text2Ready = true;
    }

    updateFloatingText(time);

    if (enemyAlsoCreatedOnMap && roomNumber!=0){ //Если ещё не были созданы на текущей карте, то создаём
        //Заполняем список объектами врагами
        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            float xr = 100 + rand() % 250; // случайная координата врага на поле игры по оси “x”
            float yr = 100 + rand() % 250; // случайная координата врага на поле игры по оси “y”
            //создаем врагов и помещаем в список
            enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy", SafeRoom.GetTileMap()));
                //передали в конструктор массив сейфрума, потому что в ней нет преград
            enemiesCount += 1; //увеличили счётчик врагов
        }
        int countMiniBoss = rand() % 10; //в каждой комнате с вероятностью 33% может появиться один усиленный враг
        if (countMiniBoss <= 3) {
            float xr = 150 + rand() % 250;
            float yr = 150 + rand() % 250;
            enemies.push_back(new Enemy(miniBossImage, xr, yr, 96, 96, "MiniBoss", SafeRoom.GetTileMap()));
                //передали в конструктор массив сейфрума, потому что в ней нет преград
            enemiesCount += 1; //увеличили счётчик врагов
        }
    enemyAlsoCreatedOnMap = false;
    }

    p.update(time);
    VMachine.update(time);

    //оживляем врагов
    for (eit = enemies.begin(); eit != enemies.end(); eit++)
    {
        (*eit)->update(time); //запускаем метод update()
    }

    //оживляем пули
    for (it = Bullets.begin(); it != Bullets.end(); it++)
    {
        (*it)->update(time); //запускаем метод update()
    }

    //Проверяем список на наличие "мертвых" пуль и удаляем их
    for (it = Bullets.begin(); it != Bullets.end(); )//говорим что проходимся от начала до конца
    {// если этот объект мертв, то удаляем его
        if ((*it)-> life == false) { it = Bullets.erase(it); }
        else it++;//и идем курсором (итератором) к след объекту.
    }

    //Проверяем список на наличие "мертвых" врагов и удаляем их
    for (eit = enemies.begin(); eit != enemies.end(); )//говорим что проходимся от начала до конца
    {// если этот объект мертв, то удаляем его
        if ((*eit)-> life == false)
        {
            (*eit)-> SpawnCoin();
            eit = enemies.erase(eit);
        }
        else eit++;//и идем курсором (итератором) к след объекту.
    }

    //Проверка пересечения игрока с врагами
    //Если пересечение произошло, то "health -= 20". После того, как здоровье опустится до 0,
    //игрок обездвиживается и выводится сообщение "you are lose"
    if (p.life == true){//если игрок жив
        for (eit = enemies.begin(); eit != enemies.end(); eit++){//бежим по списку врагов
            if ((p.getRect().intersects((*eit)->getRect())) && (((*eit)->name == "EasyEnemy")||
                                                                ((*eit)->name == "MiniBoss")))
            {
                if (hpDownPlayerTimer>1000){
                    p.health -= (*eit)->damage;
                    cout << "You take the damage!\n";
                    hpDownPlayerTimer = 0;//обнуляем таймер
                }
            }
        }
    }

    //пересечение пули с врагом
    for (eit = enemies.begin(); eit != enemies.end(); eit++){//бежим по списку врагов
        for (it = Bullets.begin(); it != Bullets.end(); it++){//по списку пуль
            if (((*it)->getRect().intersects((*eit)->getRect())) && ((*it)->name == "Bullet") &&
                (((*eit)->name == "EasyEnemy")||((*eit)->name == "MiniBoss")))
            {
                cout << "Exellent hit!\n";

                //при попадании пули у врага отнимается здоровье
                (*eit)-> health -= PLAYER_DAMAGE;
                if ((*eit)-> health <= 0) {
                    (*eit)-> life = false;
                    enemiesCount -= 1; //уменьшаем количество врагов в игре
                    gostSound.play();
                    cout << "Enemy destroyed!\n";
                }
                (*it)-> life = false;
            }
        }
    }
    //пересечение пули с автоматом
    if (roomNumber == 0)
        for (it = Bullets.begin(); it != Bullets.end(); it++){//бежим по списку пуль
            if (((*it)->getRect().intersects(VMachine.getRect())) && ((*it)->name == "Bullet"))
            {
                //при попадании пули у автомата отнимается здоровье
                VMachine.health -= PLAYER_DAMAGE;
                VMTakeDamage.play();
                if (VMachine.health <= 0 && VMachine.life) {
                    VMachine.life = false;
                    VMachine.sprite.setTextureRect(IntRect(VMachine.w, 0, VMachine.w, VMachine.h));
                    VMDestroy.play();
                    cout << "You destroyed the Vending Machine!\n";

                    srand(std::time(0));
                    int randN = std::rand() % 3 + 1;
                    for (int i = 0; i <= randN; i++)
                        VMachine.spawnFood();
                    VMachine.spawnKey();
                }
                (*it)-> life = false;
            }
        }
}

void Game::updateFloatingText(float time){
    displayTextTimer += time;
    if (displayTextTimer < displayTime + fadeTime) {
        // Плавное появление
        float alpha = 255.f * std::min(1.f, displayTextTimer / fadeTime);
        floatingText.setColor(sf::Color(0, 0, 0, alpha));
    } else if (displayTextTimer < displayTime + 2*fadeTime){
        // Исчезновение
        float alpha = 255.f * std::max(0.f, (displayTime + fadeTime*2 - displayTextTimer) / fadeTime);
        floatingText.setColor(sf::Color(0, 0, 0, alpha));
    }
    else {
        // Текст полностью исчез - очищаем текст
        floatingText.setString("");
    }
}

void Game::draw() {
    window.clear();

    if (enemiesCount==0) p.killAllEnemies = true;

    changeMapLogic();

    //////////////////////Рисуем нужную карту и передаём её в существующие объекты/////////////////
    switch (roomNumber)
    {
    case 0:
        SafeRoom.draw(&s_SafeRoomMap, &window);
        for (it = Bullets.begin(); it != Bullets.end(); it++)
            {(*it)-> TileMap = SafeRoom.GetTileMap();
        }
        p.TileMap = SafeRoom.GetTileMap();
        break;
    case 1:
        map1.draw(&s_map, &window);
        for (eit = enemies.begin(); eit != enemies.end(); eit++)//говорим что проходимся от начала до конца
            {(*eit)-> TileMap = map1.GetTileMap();}

        for (it = Bullets.begin(); it != Bullets.end(); it++)
            {(*it)-> TileMap = map1.GetTileMap();}

        p.TileMap = map1.GetTileMap();
        break;
    case 2: map2.draw(&s_map, &window);
        for (eit = enemies.begin(); eit != enemies.end(); eit++)//говорим что проходимся от начала до конца
            {(*eit)-> TileMap = map2.GetTileMap();}

        for (it = Bullets.begin(); it != Bullets.end(); it++)
            {(*it)-> TileMap = map2.GetTileMap();}

        p.TileMap = map2.GetTileMap();
        break;
    case 3: map3.draw(&s_map, &window);
        for (eit = enemies.begin(); eit != enemies.end(); eit++)//говорим что проходимся от начала до конца
            {(*eit)-> TileMap = map3.GetTileMap();}

        for (it = Bullets.begin(); it != Bullets.end(); it++)
            {(*it)-> TileMap = map3.GetTileMap();}

        p.TileMap = map3.GetTileMap();
        break;
    case 4: map4.draw(&s_map, &window);
        for (eit = enemies.begin(); eit != enemies.end(); eit++)//говорим что проходимся от начала до конца
            {(*eit)-> TileMap = map4.GetTileMap();}

        for (it = Bullets.begin(); it != Bullets.end(); it++)
            {(*it)-> TileMap = map4.GetTileMap();}

        p.TileMap = map4.GetTileMap();
        break;
    }           //сюда вместо s_map можно просто подать указатель на другой спрайт,
                //и тогда карта будет выглядеть по-другому

    //объявили переменную здоровья и времени
    ostringstream playerHealthString, gameTimeString, playerScoreString;
    playerHealthString << p.health; gameTimeString << gameTime;//формируем строку
    playerScoreString << p.playerScore; //занесли в нее число очков, то есть формируем строку
    text.setString("HP " + playerHealthString.str() + "\nTime: " +
                   gameTimeString.str()+ "\nScore: " + playerScoreString.str());//задаем строку тексту
    text.setPosition(40, 27);//задаем позицию текста
    window.draw(text);//рисуем этот текст
    window.draw(floatingText);

    window.draw(p.sprite);//рисуем спрайт объекта “p” класса “Player”


    if (roomNumber == 0) window.draw(VMachine.sprite);
    ////////////////////////////////////////////////////////////////////

    //рисуем врагов
    for (eit = enemies.begin(); eit != enemies.end(); eit++)
    {
        if ((*eit)->life) //если враги живы
            window.draw((*eit)->sprite); //рисуем
    }
    //рисуем пули
    for (it = Bullets.begin(); it != Bullets.end(); it++)
    {
        if ((*it)->life) //если пули живы
            window.draw((*it)->sprite); //рисуем объекты
    }

    window.display();
}

void Game::checkOptionForWindow(){
    ////////////////Экран смерти/////////////////
    if (p.life == false)
    {
        backgroundMusic.stop();
        DeathAnimation deathAnimation(window);
        deathAnimation.playAnimation(window);
    }
    ////////////////Конец игры///////////////////////
    if ((roomNumber == 4) && map3.isPassed && p.killAllEnemies)
    {
        backgroundMusic.stop();
        Ending ending(window);
        ending.playAnimation();
        cout << "You won!!!" << endl;
    }
}

void Game::changeMapLogic(){
    if (roomNumber != p.numberOfRoom) //эти флаги меняются только при смене комнаты
    {
        switch (roomNumber) { //p.numberOfRoom обновляется только если все враги в комнате убиты.
        case 1: map1.isPassed = true;
            break;
        case 2: map2.isPassed = true;
            break;
        case 3: map3.isPassed = true;
            break;
        case 4: map4.isPassed = true;
            break;
        }

        roomNumber = p.numberOfRoom; //обновляем комнату

        switch (roomNumber) { //если эта комната была пройдена
        case 1: if (!map1.isPassed) {enemyAlsoCreatedOnMap = true; p.killAllEnemies = false;}
            break;
        case 2: if (!map2.isPassed) {enemyAlsoCreatedOnMap = true; p.killAllEnemies = false;}
            break;
        case 3: if (!map3.isPassed) {enemyAlsoCreatedOnMap = true; p.killAllEnemies = false;}
            break;
        case 4: if (!map4.isPassed) {enemyAlsoCreatedOnMap = true; p.killAllEnemies = false;}
            break;
        }
    }
}
