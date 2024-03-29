#ifndef GAMEMUSIC_H
#define GAMEMUSIC_H

#include "mbed.h"
#include "globals.h"
#include "hardware.h"

typedef struct {
    char     riff[4];
    uint32_t fileSize;
    char     wave[4];
} WAVHeader;

const char songs[3][20] = {"Day_One", "Time", "Cornfield_Chase"};

/**
 * Use to change game audio volume
 */
double* getVolume(void);

/**
 * Prepares background music and game effects to be played
 */
void musicInit(void);

/**
 * Plays background music
 */
void playMusic(void);

/**
 * Plays next song track
 */
void playNextTrack(void);

/**
 * Plays prev song track
 */
void playPrevTrack(void);

/**
 * Loads 16kB buffers for background music
 */
void loadMusic(void);

/**
 * Plays normal projectile sound effect
 */
void projectileSound(void);

/**
 * Plays laser projectile sound effect
 */
void laserSound(void);

/**
 * Plays missile projectile sound effect
 */
void missileSound(void);

/**
 * Plays explosion sound effect
 */
void explosionSound(void);

/**
 * Plays projectile hit sound effect
 */
void hitSound(void);

/**
 * Plays button press sound effect
 */
void buttonSound(void);

/**
 * Plays winning sound effect
 */
void winSound(void);

/**
 * Plays losing sound effect
 */
void loseSound(void);

#endif