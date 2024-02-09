// class VendingMachine

#include "vendingmachine.h"

VendingMachine::VendingMachine(Image& image, float X, float Y, int W, int H, string Name, string* MapMap)
    :Entity(image, X, Y, W, H, Name, MapMap) {
    price = 5;
    health = 400;
    state = stay;
    if (name == "vm") {
        //Задаем спрайту один прямоугольник для
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }
    dealBuffer.loadFromFile("sounds/VMdeal.wav");
    dealSound.setBuffer(dealBuffer);
    dealSound.setVolume(50);
}

void VendingMachine::update(float time) //метод "оживления/обновления" объекта класса.
{
    if (name == "vm")
        sprite.setPosition(x, y);
}

void VendingMachine::exchangeCoins(Player& player)
{
    if (player.playerScore >= price) {
        player.playerScore -= price;
        dealSound.play();
        spawnFood();
    }
    else cout << "You don't have enough money" << endl;
}

void VendingMachine::spawnFood()
{
    int i = y / 32 + 1;
    int j = (x + w) / 32 + 1;
    int schet = 1;
    while (TileMap[i][j] == 'h')
    {
        j = (x + w / 2) / 16 - j;
        if (schet == 2) { i++; schet = 0; }
        schet++;
    }
    TileMap[i][j] = 'h';
}

void VendingMachine::spawnKey()
{
    int i = y / 32 + 1;
    int j = (x + w) / 32 + 1;
    int schet = 1;
    while (TileMap[i][j] == 'h')
    {
        j = (x + w / 2) / 16 - j;
        if (schet == 2) { i++; schet = 0; }
        schet++;
    }
    TileMap[i][j] = 'k';
}
