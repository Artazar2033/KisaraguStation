#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "arrmaps.h"
#include "vendingmachine.h"

int main()
{
    //список констант
    const int PLAYER_DAMAGE = 40; //урон от пули по врагу
    const int FIRE_SPEED = 500; //скорострельность в мс
    int roomNumber = 1;
    const int ENEMY_COUNT = 3; //максимальное количество врагов в игре
    bool enemyAlsoCreatedOnMap = true;
    int enemiesCount = 0; //текущее количество врагов в игре

    //MainMenu.start();

    VideoMode desktop = VideoMode::getDesktopMode();

    RenderWindow window(VideoMode(WIDTH_MAP*32, HEIGHT_MAP*32, desktop.bitsPerPixel), "Station Demo");

    Font font;//шрифт
    font.loadFromFile("MP Manga.ttf");//передаем нашему шрифту файл шрифта

    Text text("", font, 30);//создаем объект текст
    text.setColor(Color::Red);//покрасили текст в красный
    text.setStyle(Text::Bold);//жирный текст.

    Image map_image;//объект изображения для карты
    map_image.loadFromFile("images/map_new.png");//загружаем файл для карты
    Texture map;//текстура карты
    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    Sprite s_map;//создаём спрайт для карты
    s_map.setTexture(map);//заливаем текстуру спрайтом


    Map map1(ArrMap1, 1);
    Map map2(ArrMap2, 2);
    Map map3(ArrMap3, 3);
    Map map4(ArrMap4, 4);

    Clock clock;
    Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры
    int gameTime = 0;//объявили игровое время, инициализировали.

    Image heroImage;
    heroImage.loadFromFile("images/hero.png"); // загружаем изображение игрока
    //heroImage.createMaskFromColor(Color(255, 255, 255));

    Image VendingMachineImage;//изображение торгового автомата для восстановления жизни
    VendingMachineImage.loadFromFile("images/vending.png");
    VendingMachineImage.createMaskFromColor(Color(255, 255, 255));

    Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/enemy.png"); // загружаем изображение врага
    easyEnemyImage.createMaskFromColor(Color(255, 255, 255)); //убираем белый цвет

    Image BulletImage;//изображение для пули
    BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения
    BulletImage.createMaskFromColor(Color(255, 255, 255));
    BulletImage.createMaskFromColor(Color(0, 0, 0)); //убираем черный цвет

    Player p(heroImage, 100, 100, 70, 96, "Player1", map1.GetTileMap());//объект класса игрока
    //VendingMachine vm(VendingMachineImage, 0, 230, 150, 150, "vm", map1.GetTileMap()); //автомат с едой

    list<Enemy*> enemies; //список врагов
    list<Entity*> Bullets; //список пуль
    list<Entity*>::iterator it;//итератор класса Entity
    //list<Entity*>::iterator b;//итератор чтобы проходить по элементам списка
    list<Enemy*>::iterator eit;//итератор по классу Enemy

    int createObjectForMapTimer = 0;//Переменная под время для генерирования камней
    //int hpDownEnemiesTimer = 0;//Переменная под время для неуязвимости врагов после попадания пули
    int hpDownPlayerTimer = 0;//Переменная под время для неуязвимости игрока после получения урона
    int createBulletsTimer = 0;//Переменная под время для задержки выстрела

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в
        //секундах идёт вперед, пока жив игрок. Перезагружать как time его не надо.
        //оно не обновляет логику игры
        clock.restart();
        time /= 700;

        createObjectForMapTimer += time;//наращиваем таймеры
        createBulletsTimer += time;
        //hpDownEnemiesTimer += time;
        hpDownPlayerTimer += time;

        //if (createObjectForMapTimer>1000){
        //    map1.randomMapGenerate();//генерация камней
        //    createObjectForMapTimer = 0;//обнуляем таймер
        //}

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            //стреляем по нажатию клавиши "E"
            if (event.type == Event::KeyPressed)
            {
                if ((event.key.code == Keyboard::E) && (createBulletsTimer > FIRE_SPEED) && (p.life))        //стреляем по нажатию клавиши "E"
                    //если нарастили меньше 1 секунды, то пуля не рождается
                {
                    Bullets.push_back(new Bullet(BulletImage, p.x+32, p.y+50, 16, 16, "Bullet",
                                                 p.state, map1.GetTileMap()));
                    createBulletsTimer = 0;//обнуляем таймер
                }
                if (event.key.code == Keyboard::Space) {            //обмен монет на жизнь по нажатию клавиши "space"
                    //vm.exchangeCoins(p);
                }
            }
        }

        if (enemyAlsoCreatedOnMap){ //Если ещё не были созданы на текущей карте, то создаём
            //Заполняем список объектами врагами
            for (int i = 0; i < ENEMY_COUNT; i++)
            {
                float xr = 150 + rand() % 500; // случайная координата врага на поле игры по оси “x”
                float yr = 150 + rand() % 350; // случайная координата врага на поле игры по оси “y”
                //создаем врагов и помещаем в список
                enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy", map1.GetTileMap()));
                enemiesCount += 1; //увеличили счётчик врагов
            }
        enemyAlsoCreatedOnMap = false;
        }

        p.update(time); //оживляем объект “p” класса “Player”
        //оживляем врагов

        //if (roomNumber == 1)
        //    vm.update(time);

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
                        cout << "Enemy destroyed!\n";
                    }
                    (*it)-> life = false;
                }
            }
        }





        window.clear();

        if (enemiesCount==0) p.killAllEnemies = true;
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
    return 0;
}


