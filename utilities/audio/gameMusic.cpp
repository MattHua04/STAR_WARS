#include "gameMusic.hpp"
#include "LPC17xx.h"
#include "PinNames.h"
#include "game.hpp"
#include "globals.hpp"
#include "menu.hpp"
#include "normalProjectileSound.hpp"
#include "laserSound.hpp"
#include "explosionSound.hpp"
#include "hit.hpp"
#include "button.hpp"
#include "win.hpp"
#include "lose.hpp"
#include "missile.hpp"
#include <cstdint>

#define MAX_DATA_SIZE 8192

AnalogOut sound(p18);
double volume = 0.25;

// 16KB audio buffers, swapped and loaded when needed
unsigned short *bufferZero = (unsigned short*)0x2007C000;
unsigned short *bufferOne = (unsigned short*)0x2007E000;

// Audio file info
WAVHeader header;
unsigned int filePos = 78;
unsigned int fileEnd = 0;

// Interupt for playing audio
Ticker gameSound;

// Audio file positions, -1 means not playing
int bufferReady = 0;
int bufferInUse = 0;
int mCounter = 0;
int pCounter = -1;
int lCounter = -1;
int missileCounter = -1;
int xCounter = -1;
int hCounter = -1;
int bCounter = -1;
int winCounter = -1;
int loseCounter = -1;

// Current song file and index
FILE *songFile;
unsigned short currentSong = 0;

double* getVolume(void) {
    return &volume;
}

void musicInit(void) {
    int attempts = 0;
    currentSong = -1;
    FILE* tempFile;
    while (1) {
        tempFile = fopen("/sd/bootupSound.wav", "rb");
        if (tempFile == NULL) {
            attempts++;
            printf("Failed to open song file\n");
            if (attempts < 10) {
                continue;
            } else {
                return;
            }
        } else {
            songFile = tempFile;
            break;
        }
    }
    // Get file size to know when to loop back
    fread(&header, sizeof(header), 1, songFile);
    fileEnd = header.fileSize;
    // Clear junk values
    for (int i = 0; i < MAX_DATA_SIZE / sizeof(unsigned short); i++) {
        bufferZero[i] = 0;
        bufferOne[i] = 0;
    }
    gameSound.attach(&playMusic, 1.0 / 11025.0);
}

void playNextTrack(void) {
    int attempts = 0;
    fclose(songFile);
    char fileName[100] = "/sd/";
    strcat(fileName, songs[(currentSong < 2) ? ++currentSong : (currentSong = 0)]);
    strcat(fileName, ".wav");
    FILE* tempFile;
openFile: tempFile = fopen(fileName, "rb");
    if (tempFile == NULL) {
        attempts++;
        printf("Failed to open song file\n");
        if (attempts < 10) {
            goto openFile;
        } else {
            return;
        }
    }
    songFile = tempFile;
    fread(&header, sizeof(header), 1, songFile);
    fileEnd = header.fileSize;
}

void playPrevTrack(void) {
    int attempts = 0;
    fclose(songFile);
    char fileName[100] = "/sd/";
    strcat(fileName, songs[(currentSong > 0) ? --currentSong : (currentSong = 2)]);
    strcat(fileName, ".wav");
    FILE* tempFile;
openFile: tempFile = fopen(fileName, "rb");
    if (tempFile == NULL) {
        attempts++;
        printf("Failed to open song file\n");
        if (attempts < 10) {
            goto openFile;
        } else {
            return;
        }
    }
    songFile = tempFile;
    fread(&header, sizeof(header), 1, songFile);
    fileEnd = header.fileSize;
}

void loadMusic(void) {
    if (!bufferReady && songFile) {
        unsigned short *addr;
        if (bufferInUse == 0) {
            addr = bufferOne;
        } else {
            addr = bufferZero;
        }
        // Read the audio data
        fread(addr, sizeof(unsigned short), MAX_DATA_SIZE / sizeof(unsigned short), songFile);
        // Check for end of file
        if (filePos >= fileEnd) {
            // Reset file position to the beginning
            filePos = 78;
            playNextTrack();
            fread(addr, sizeof(unsigned short), MAX_DATA_SIZE / sizeof(unsigned short), songFile);
        }
        filePos += MAX_DATA_SIZE;
        bufferReady = 1;
    }
}

void playMusic(void) {
    // Mix all sound effects
    unsigned short data = 32768;
    data += (pCounter >= 0) ? pSoundData[pCounter++] - 32768 : 0;
    data += (lCounter >= 0) ? lSoundData[lCounter++] - 32768 : 0;
    data += (missileCounter >= 0) ? missileSoundData[missileCounter++] - 32768 : 0;
    data += (xCounter >= 0) ? xSoundData[xCounter++] - 32768 : 0;
    data += (hCounter >= 0) ? hSoundData[hCounter++] - 32768 : 0;
    data += (bCounter >= 0) ? bSoundData[bCounter++] - 32768 : 0;
    data += (winCounter >= 0) ? winSoundData[winCounter++] - 32768 : 0;
    data += (loseCounter >= 0) ? loseSoundData[loseCounter++] - 32768 : 0;
    // Output the mixed audio
    if (bufferInUse == 0) {
        sound.write_u16((unsigned short)(((bufferZero[mCounter++] + data) & 0xffff) * volume));
    } else if (bufferInUse == 1) {
        sound.write_u16((unsigned short)(((bufferOne[mCounter++] + data) & 0xffff) * volume));
    }
    // Iterate or reset all audio counters if necessary
    if (mCounter >= MAX_DATA_SIZE / sizeof(unsigned short)) {
        mCounter = 0;
        bufferInUse = !bufferInUse;
        bufferReady = 0;
    }
    (pCounter >= 2360) ? pCounter = -1 : pCounter;
    (lCounter >= 33748) ? lCounter = -1 : lCounter;
    (missileCounter >= 4608) ? missileCounter = -1 : missileCounter;
    (xCounter >= 26217) ? xCounter = -1 : xCounter;
    (hCounter >= 2048) ? hCounter = -1 : hCounter;
    (bCounter >= 1882) ? bCounter = -1 : bCounter;
    (winCounter >= 28922) ? winCounter = -1 : winCounter;
    (loseCounter >= 9939) ? loseCounter = -1 : loseCounter;
}

void projectileSound(void) {
    pCounter = 0;
}

void laserSound(void) {
    lCounter = 0;
}

void missileSound(void) {
    missileCounter = 0;
}

void explosionSound(void) {
    xCounter = 0;
}

void hitSound(void) {
    hCounter = 0;
}

void buttonSound(void) {
    bCounter = 0;
}

void winSound(void) {
    winCounter = 0;
}

void loseSound(void) {
    loseCounter = 0;
}