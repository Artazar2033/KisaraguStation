#ifndef ENDING
#define ENDING
#include "libs.h"

class Ending {
public:
    Ending(RenderWindow& window);
    void playAnimation(RenderWindow& originalWindow);

private:
    RenderWindow& window;
    Font font;
    Text text;
    Clock animationTimer;
    SoundBuffer endingSoundBuffer;
    Sound endingSound;
};
#endif // ENDING

