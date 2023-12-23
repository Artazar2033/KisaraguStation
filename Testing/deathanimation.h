#ifndef DEATHANIMATION
#define DEATHANIMATION

#include "libs.h"

class DeathAnimation {
public:
    DeathAnimation(RenderWindow& window);
    void playAnimation(RenderWindow& originalWindow);

private:
    RenderWindow& window;
    Font font;
    Text text;
    Clock animationTimer;
    SoundBuffer deathSoundBuffer;
    Sound deathSound;
};

#endif // DEATHANIMATION

