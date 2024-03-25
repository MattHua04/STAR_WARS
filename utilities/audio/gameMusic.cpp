#include "gameMusic.h"
#include "normalProjectileSound.h"
#include "laserSound.h"
#include "explosionSound.h"

AnalogOut sound(p18);
Ticker pSound;
Ticker lSound;
Ticker xSound;
int pCounter = 0;
int lCounter = 0;
int xCounter = 0;

void music(void) {
    char wav[]="/sd/Interstellar.wav";
    FILE *wave_file;
    wave_file=fopen(wav,"r");
    if (wave_file == NULL) {
        printf("Failed to open file\n");
        return;
    }
    waver.play(wave_file);
    fclose(wave_file);
}

void projectileSound(void) {
    pSound.attach(&playProjectileSound, 1.0 / 11025.0);
}

void playProjectileSound(void) {
    sound.write_u16(pSoundData[pCounter]);
    pCounter++;
    if (pCounter >= 9988) {
        printf("%d\n", pCounter);
        pCounter = 0;
        pSound.detach();
    }
}

void laserSound(void) {
    lSound.attach(&playLaserSound, 1.0 / 11025.0);
}

void playLaserSound(void) {
    sound.write_u16(lSoundData[lCounter]);
    lCounter++;
    if (lCounter >= 228399) {
        lCounter = 0;
        lSound.detach();
    }
}

void explosionSound(void) {
    xSound.attach(&playExplosionSound, 1.0 / 11025.0);
}

void playExplosionSound(void) {
    sound.write_u16(xSoundData[xCounter]);
    xCounter++;
    if (xCounter >= 55168) {
        printf("%d\n", xCounter);
        xCounter = 0;
        xSound.detach();
    }
}