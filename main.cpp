#include "game.h"

int main() {
    Image GGimage, vmImage;
    GGimage.loadFromFile("images/hero.png"); // ��������� ����������� ������
    GGimage.createMaskFromColor(Color(255, 255, 255));

    vmImage.loadFromFile("images/vending.png"); // ��������� ����������� �������� � ����
    vmImage.createMaskFromColor(Color(255, 255, 255));

    Game game(GGimage, vmImage);
    game.run();
    return 0;
}
