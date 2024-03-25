#ifndef GAMEMUSIC_H
#define GAMEMUSIC_H

#include "mbed.h"
#include "globals.h"
#include "hardware.h"

/**
 * Plays background music
 */
void music(void);

void projectileSound(void);

void playProjectileSound(void);

void laserSound(void);

void playLaserSound(void);

void explosionSound(void);

void playExplosionSound(void);

#endif