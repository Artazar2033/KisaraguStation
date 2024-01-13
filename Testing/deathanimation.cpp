#include "deathanimation.h"

DeathAnimation::DeathAnimation(RenderWindow& window) : window(window) {
    // Загрузите текстуры и настройте спрайты
    if (!font.loadFromFile("MP Manga.ttf")) {
        // Обработка ошибки загрузки шрифта
    }
    text.setFont(font);
    text.setString("You died");
    text.setCharacterSize(48);
    text.setColor(Color::Red);

    // Настройка начальной позиции текста (выравнивание по центру экрана)
    FloatRect textBounds = text.getLocalBounds();
    text.setPosition((window.getSize().x - textBounds.width) / 2, (window.getSize().y - textBounds.height) / 2);

    // Настройка таймера для анимации (если нужно)
    animationTimer.restart();
}

void DeathAnimation::playAnimation(RenderWindow& originalWindow) {
    // Загрузка звукового файла
    SoundBuffer deathSoundBuffer;
    if (!deathSoundBuffer.loadFromFile("sounds/death_sound.wav")) {
        // Обработка ошибки загрузки звукового файла
    }

    // Создание объекта звука
    Sound deathSound(deathSoundBuffer);
    deathSound.setVolume(80);

    // Воспроизведение звука
    deathSound.play();

    while (animationTimer.getElapsedTime().asSeconds() < 5.0) { // Продолжайте анимацию в течение 3 секунд (настройте под свои нужды)
        window.clear();

        // Отображение анимации
        window.draw(text);

        window.display();
    }
    // Плавное исчезновение
    Clock fadeTimer;
    while (fadeTimer.getElapsedTime().asSeconds() < 1.0) { // Например, плавное исчезновение за 1 секунду
        window.clear();

        // Изменение прозрачности текста
        Color textColor = text.getColor();
        textColor.a = static_cast<Uint8>(255 - 255 * (fadeTimer.getElapsedTime().asSeconds() / 1.0));
        text.setColor(textColor);

        // Отображение анимации
        window.draw(text);

        window.display();
    }
    // Возврат к основному окну игры
    window.close();
    originalWindow.display();
    }

