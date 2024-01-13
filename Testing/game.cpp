#include "game.h"
#include "arrmaps.h"

Game::Game(Image& im): //loadTextures(), //loadMaps(),
    window(VideoMode(WIDTH_MAP * 32, HEIGHT_MAP * 32), "Station Demo"),
    p(im, 100, 100, 70, 96, "Player1", map1.GetTileMap()),
    text("", font, 30), map1(ArrMap1, 1), map2(ArrMap2, 2),
    map3(ArrMap3, 3), map4(ArrMap4, 4)
{
    gameTime = 0;
    hpDownPlayerTimer = 0;//Переменная под время для неуязвимости игрока после получения урона
    createBulletsTimer = 0;//Переменная под время для задержки выстрела
    backgroundMusicTimer = 0;

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

    //heroImage.loadFromFile("images/hero.png"); // загружаем изображение игрока
    //heroImage.createMaskFromColor(Color(255, 255, 255));

    vendingMachineImage.loadFromFile("images/vending.png");
    vendingMachineImage.createMaskFromColor(Color(255, 255, 255));

    easyEnemyImage.loadFromFile("images/enemy.png");
    easyEnemyImage.loadFromFile("images/enemy.png"); // загружаем изображение врага
    easyEnemyImage.createMaskFromColor(Color(255, 255, 255)); //убираем белый цвет

    BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения
    BulletImage.createMaskFromColor(Color(255, 255, 255));
    BulletImage.createMaskFromColor(Color(0, 0, 0)); //убираем черный цвет

    map_image.loadFromFile("images/map_new.png");//загружаем файл для карты
    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    s_map.setTexture(map);//заливаем текстуру спрайтом

    backgroundMusic.openFromFile("sounds/Arseny-St-Hollow.wav");
    backgroundMusic.setVolume(30); // Уровень громкости (0-100)
    backgroundMusic.setLoop(true); // Повторять воспроизведение

    gunshotBuffer.loadFromFile("sounds/bullet.wav");
    gunshotSound.setBuffer(gunshotBuffer);
    gunshotSound.setVolume(20);

    gostBuffer.loadFromFile("sounds/gost_death.wav");
    gostSound.setBuffer(gostBuffer);
    gostSound.setVolume(20);
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
                                             p.savedState, map1.GetTileMap()));
                createBulletsTimer = 0;//обнуляем таймер
                gunshotSound.play();
            }
            if (event.key.code == Keyboard::Space) { //обмен монет на жизнь по нажатию клавиши "space"
                //vm.exchangeCoins(p);
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
    time /= 800;

    createBulletsTimer += time; //наращиваем таймеры
    hpDownPlayerTimer += time;


    if (enemyAlsoCreatedOnMap){ //Если ещё не были созданы на текущей карте, то создаём
        //Заполняем список объектами врагами
        for (int i = 0; i < ENEMY_COUNT; i++)
        {
            float xr = 150 + rand() % 250; // случайная координата врага на поле игры по оси “x”
            float yr = 150 + rand() % 250; // случайная координата врага на поле игры по оси “y”
            //создаем врагов и помещаем в список
            enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy", map1.GetTileMap()));
            enemiesCount += 1; //увеличили счётчик врагов
        }
    enemyAlsoCreatedOnMap = false;
    }

    p.update(time);

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
            if ((p.getRect().intersects((*eit)->getRect())) && ((*eit)->name == "EasyEnemy"))
            {
                if (hpDownPlayerTimer>1000){
                    p.health -= 20;
                    cout << "you take the damage!\n";
                    hpDownPlayerTimer = 0;//обнуляем таймер
                }
            }
        }
    }

    //пересечение пули с врагом
    for (eit = enemies.begin(); eit != enemies.end(); eit++){//бежим по списку врагов
        for (it = Bullets.begin(); it != Bullets.end(); it++){//по списку пуль
            if (((*it)->getRect().intersects((*eit)->getRect())) &&
                ((*eit)->name == "EasyEnemy") && ((*it)->name == "Bullet"))
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
}

void Game::draw() {
    window.clear();

    if (enemiesCount==0) p.killAllEnemies = true;

    changeMapLogic();

    //////////////////////Рисуем нужную карту и передаём её в существующие объекты/////////////////
    switch (roomNumber)
    {
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

    window.draw(p.sprite);//рисуем спрайт объекта “p” класса “Player”
    //window.draw(vm.sprite);

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
    if (map3.isPassed && p.killAllEnemies)
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
