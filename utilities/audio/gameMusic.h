#ifndef GAMEMUSIC_H
#define GAMEMUSIC_H

#include "mbed.h"
#include "globals.h"
#include "hardware.h"

/**
 * Plays background music
 */
void printBuffer(void);

void prepareMusicLoader(void);

void musicInit(void);

void playMusic(void);

void loadMusic(void);

void projectileSound(void);

void playProjectileSound(void);

void laserSound(void);

void playLaserSound(void);

void missileSound(void);

void playMissileSound(void);

void explosionSound(void);

void playExplosionSound(void);

void hitSound(void);

void playHitSound(void);

void buttonSound(void);

void playButtonSound(void);

void winSound(void);

void playWinSound(void);

void loseSound(void);

void playLoseSound(void);

#endif