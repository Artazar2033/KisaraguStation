#include "ending.h"

Ending::Ending(RenderWindow& window) : window(window) {
    // ��������� �������� � ��������� �������
    if (!font.loadFromFile("MP Manga.ttf")) {
        // ��������� ������ �������� ������
    }
    text.setFont(font);
    text.setString("              Congratulations!\nYou escaped the damned Kisaragu Station\n");
    text.setCharacterSize(28);
    text.setColor(Color::Red);

    // ��������� ��������� ������� ������ (������������ �� ������ ������)
    FloatRect textBounds = text.getLocalBounds();
    text.setPosition((window.getSize().x - textBounds.width) / 2, (window.getSize().y - textBounds.height) / 2);

    // ��������� ������� ��� �������� (���� �����)
    animationTimer.restart();
}

void Ending::playAnimation() {
    // �������� ��������� �����
    SoundBuffer endingSoundBuffer;
    if (!endingSoundBuffer.loadFromFile("sounds/ending_sound.wav")) {
        // ��������� ������ �������� ��������� �����
    }

    // �������� ������� �����
    Sound endingSound(endingSoundBuffer);
    endingSound.setVolume(50);

    // ��������������� �����
    endingSound.play();

    while (animationTimer.getElapsedTime().asSeconds() < 10) {
        window.clear();

        // ����������� ��������
        window.draw(text);

        window.display();
    }
    // ������� ������������ �� 6 ������
    Clock fadeTimer;
    while (fadeTimer.getElapsedTime().asSeconds() < 3) {
        window.clear();

        // ��������� ������������ ������
        Color textColor = text.getColor();
        textColor.a = static_cast<Uint8>(255 - 255 * (fadeTimer.getElapsedTime().asSeconds() / 3.0));
        text.setColor(textColor);

        // ����������� ��������
        window.draw(text);

        window.display();
    }
    // ������� � ��������� ���� ����
    window.close();
    //originalWindow.display();
}
