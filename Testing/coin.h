/*
#ifndef COIN
#define COIN
public:
    Coin(sf::Texture& texture, sf::Vector2f position)
    {
        sprite.setTexture(texture);   //создана текстура
        sprite.setPosition(position); //позиция
        isCollected = false;   //флаг6 монетка не собрана
    }

    void OnClick()   //монетка собирается по клику на неё
    {
        if (!isCollected) {
            isCollected = true;
        }
    }

    bool IsCollected() //возвращает текущее состояние монетки
    {
        return isCollected;
    }

    sf::Sprite GetSprite() //возвращает спрайт монетк
    {
        return sprite;
    }

private:
    sf::Sprite sprite;
    bool isCollected;
};

#endif // COIN
*/

