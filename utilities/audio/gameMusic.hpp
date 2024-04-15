#ifndef GAMEMUSIC_H
#define GAMEMUSIC_H

#include "mbed.h"
#include "globals.hpp"
#include "hardware.hpp"

// Wav file info
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
 * Starts playing background music
 */
void playMusic(void);

/**
 * Starts playing next song track
 */
void playNextTrack(void);

/**
 * Starts playing prev song track
 */
void playPrevTrack(void);

/**
 * Loads 16kB buffers for background music
 */
void loadMusic(void);

/**
 * Starts playing normal projectile sound effect
 */
void projectileSound(void);

/**
 * Starts playing laser projectile sound effect
 */
void laserSound(void);

/**
 * Starts playing missile projectile sound effect
 */
void missileSound(void);

/**
 * Starts playing explosion sound effect
 */
void explosionSound(void);

/**
 * Starts playing projectile hit sound effect
 */
void hitSound(void);

/**
 * Starts playing button press sound effect
 */
void buttonSound(void);

/**
 * Starts playing winning sound effect
 */
void winSound(void);

/**
 * Starts playing losing sound effect
 */
void loseSound(void);

#endif