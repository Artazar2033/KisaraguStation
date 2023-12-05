#include "classes.cpp" //подключили код с классами сущностей

int main()
{
    VideoMode desktop = VideoMode::getDesktopMode();
    RenderWindow window(VideoMode(800, 640, desktop.bitsPerPixel), "Station Demo");
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
    Clock clock;
    Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры
    int gameTime = 0;//объявили игровое время, инициализировали.
    Image heroImage;
    heroImage.loadFromFile("images/hero.png"); // загружаем изображение игрока
    heroImage.createMaskFromColor(Color(255, 255, 255));
    Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/enemy.png"); // загружаем изображение врага
    easyEnemyImage.createMaskFromColor(Color(255, 255, 255)); //убираем белый цвет
    Image BulletImage;//изображение для пули
    BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения
    BulletImage.createMaskFromColor(Color(255, 255, 255));
    BulletImage.createMaskFromColor(Color(0, 0, 0)); //убираем черный цвет
    Player p(heroImage, 100, 100, 96, 96, "Player1");//объект класса игрока
    list<Entity*> enemies; //список врагов
    list<Entity*> Bullets; //список пуль
    list<Entity*>::iterator it;//итератор чтобы проходить по элементам списка
    list<Entity*>::iterator b;//итератор чтобы проходить по элементам списка
    const int ENEMY_COUNT = 3; //максимальное количество врагов в игре
    int enemiesCount = 0; //текущее количество врагов в игре
    //Заполняем список объектами врагами
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        float xr = 150 + rand() % 500; // случайная координата врага на поле игры по оси “x”
        float yr = 150 + rand() % 350; // случайная координата врага на поле игры по оси “y”
        //создаем врагов и помещаем в список
        enemies.push_back(new Enemy(easyEnemyImage, xr, yr, 96, 96, "EasyEnemy"));
        enemiesCount += 1; //увеличили счётчик врагов
    }
    int createObjectForMapTimer = 0;//Переменная под время для генерирования камней
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в
        //секундах идёт вперед, пока жив игрок. Перезагружать как time его не надо.
        //оно не обновляет логику игры
        clock.restart();
        time /= 800;
        createObjectForMapTimer += time;//наращиваем таймер
        if (createObjectForMapTimer>1000){
            randomMapGenerate();//генерация камней
            createObjectForMapTimer = 0;//обнуляем таймер
        }
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            //стреляем по нажатию клавиши "P"
            if (event.type == Event::KeyPressed)
            {
                if ((event.key.code == Keyboard::E)&&p.state!=4)
                {
                    Bullets.push_back(new Bullet(BulletImage, p.x+32, p.y+50, 16, 16, "Bullet", p.state));
                }
            }
        }
        p.update(time); //оживляем объект “p” класса “Player”
        //оживляем врагов
        for (it = enemies.begin(); it != enemies.end(); it++)
        {
            (*it)->update(time); //запускаем метод update()
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
        for (it = enemies.begin(); it != enemies.end(); )//говорим что проходимся от начала до конца
        {// если этот объект мертв, то удаляем его
            if ((*it)-> life == false) { it = enemies.erase(it); }
            else it++;//и идем курсором (итератором) к след объекту.
        }
        //Проверка пересечения игрока с врагами
        //Если пересечение произошло, то "health = 0", игрок обездвижевается и
        //выводится сообщение "you are lose"
        if (p.life == true){//если игрок жив
            for (it = enemies.begin(); it != enemies.end(); it++){//бежим по списку врагов
                if ((p.getRect().intersects((*it)->getRect())) && ((*it)->name == "EasyEnemy"))
                {
                    p.health = 0;
                    cout << "you lose!\n";
                }
            }
        }
        //пересечение пули с врагом
        for (it = enemies.begin(); it != enemies.end(); it++){//бежим по списку врагов
            for (b = Bullets.begin(); b != Bullets.end(); b++){//по списку пуль
            if (((*b)->getRect().intersects((*it)->getRect())) && ((*it)->name == "EasyEnemy") && ((*b)->name == "Bullet"))
            {
                (*it)-> life = false;
                (*b)-> life = false;
                cout << "enemy destroyed\n";
            }
            }
        }

        window.clear();
        /////////////////////////////Рисуем карту/////////////////////
        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
                if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32, 0, 32, 32));
                if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));
                if (TileMap[i][j] == 'f') s_map.setTextureRect(IntRect(96, 0, 32, 32));//цветок
                if (TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(128, 0, 32, 32));//сердце
                s_map.setPosition(j * 32, i * 32);
                window.draw(s_map);
            }
        //объявили переменную здоровья и времени
        ostringstream playerHealthString, gameTimeString, playerScoreString;
        playerHealthString << p.health; gameTimeString << gameTime;//формируем строку
        playerScoreString << p.playerScore; //занесли в нее число очков, то есть формируем строку
        text.setString("HP " + playerHealthString.str() + "\nTime: " +
                       gameTimeString.str()+ "\nScore: " + playerScoreString.str());//задаем строку тексту
        text.setPosition(40, 27);//задаем позицию текста
        window.draw(text);//рисуем этот текст
        window.draw(p.sprite);//рисуем спрайт объекта “p” класса “Player”
        //рисуем врагов
        for (it = enemies.begin(); it != enemies.end(); it++)
        {
            if ((*it)->life) //если враги живы
                window.draw((*it)->sprite); //рисуем
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
