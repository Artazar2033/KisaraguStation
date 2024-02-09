#include "deathanimation.h"

DeathAnimation::DeathAnimation(RenderWindow& window) : window(window) {
    // ��������� �������� � ��������� �������
    if (!font.loadFromFile("MP Manga.ttf")) {
        // ��������� ������ �������� ������
    }
    text.setFont(font);
    text.setString("You died");
    text.setCharacterSize(48);
    text.setColor(Color::Red);

    // ��������� ��������� ������� ������ (������������ �� ������ ������)
    FloatRect textBounds = text.getLocalBounds();
    text.setPosition((window.getSize().x - textBounds.width) / 2, (window.getSize().y - textBounds.height) / 2);

    // ��������� ������� ��� �������� (���� �����)
    animationTimer.restart();
}

void DeathAnimation::playAnimation(RenderWindow& originalWindow) {
    // �������� ��������� �����
    SoundBuffer deathSoundBuffer;
    if (!deathSoundBuffer.loadFromFile("sounds/death_sound.wav")) {
        // ��������� ������ �������� ��������� �����
    }

    // �������� ������� �����
    Sound deathSound(deathSoundBuffer);
    deathSound.setVolume(80);

    // ��������������� �����
    deathSound.play();

    while (animationTimer.getElapsedTime().asSeconds() < 5.0) { // ����������� �������� � ������� 3 ������ (��������� ��� ���� �����)
        window.clear();

        // ����������� ��������
        window.draw(text);

        window.display();
    }
    // ������� ������������
    Clock fadeTimer;
    while (fadeTimer.getElapsedTime().asSeconds() < 1.0) { // ��������, ������� ������������ �� 1 �������
        window.clear();

        // ��������� ������������ ������
        Color textColor = text.getColor();
        textColor.a = static_cast<Uint8>(255 - 255 * (fadeTimer.getElapsedTime().asSeconds() / 1.0));
        text.setColor(textColor);

        // ����������� ��������
        window.draw(text);

        window.display();
    }
    // ������� � ��������� ���� ����
    window.close();
    originalWindow.display();
}
