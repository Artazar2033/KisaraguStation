// class VendingMachine

#include "vendingmachine.h"
#include "player.h"

VendingMachine::VendingMachine(Image &image, float X, float Y, int W, int H, string Name, string* MapMap)
                :Entity(image, X, Y, W, H, Name, MapMap){

        price = 1;

        if (name == "vm"){
            //Задаем спрайту один прямоугольник для
            sprite.setTextureRect(IntRect(0, 0, w, h));
        }
    }

/*
bool VendingMachine::canAfford(Player& player) {
    return (player.playerScore >= price);}


void VendingMachine::SpawnLives()
{
    for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
        for (int j = x / 32; j < (x + w) / 32; j++)
        {
            if (TileMap[i][j] == ' ')//если элемент - пустое поле
            {
                TileMap[i][j]='s';
                break;
            }
        }
}

void VendingMachine::exchangeCoins(Player& player) {
    if (canAfford(player)) {
        player.playerScore -= price;
  //      SpawnLives();
    //    cout << "Take live!\n";
        player.health += 20;
        //player.gainLife();
    }
}
*/

void VendingMachine::update(float time) //метод "оживления/обновления" объекта класса.
{
    if (name == "vm")
        sprite.setPosition(x, y);
}

/*
void draw(sf::RenderWindow& window) {
    // Отрисовка вендингового аппарата
    // ...
}
*/

