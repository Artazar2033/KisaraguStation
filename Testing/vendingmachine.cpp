// class VendingMachine

#include "vendingmachine.h"

VendingMachine::VendingMachine(Image &image, float X, float Y, int W, int H, string Name, string* MapMap)
                :Entity(image, X, Y, W, H, Name, MapMap){
        price = 5;
        health = 200;
        state = stay;
        if (name == "vm"){
            //Задаем спрайту один прямоугольник для
            sprite.setTextureRect(IntRect(0, 0, w, h));
        }
        //VendingBuffer.loadFromFile("sounds/vending.wav");
        //VendingSound.setBuffer(doorBuffer);
        //VendingSound.setVolume(50);
    }

bool VendingMachine::canAfford(Player& player) {
    return (player.playerScore >= price);
}

/*void VendingMachine::exchangeCoins(Player& player) {
    if (canAfford(player)) {
        player.playerScore -= price;
        spawnFood();
    }
}

void VendingMachine::spawnFood()
{

}*/

void VendingMachine::update(float time) //метод "оживления/обновления" объекта класса.
{
    if (name == "vm")
        sprite.setPosition(x, y);
}

void draw(sf::RenderWindow& window) {
    // Отрисовка вендингового аппарата
    // ...
}


