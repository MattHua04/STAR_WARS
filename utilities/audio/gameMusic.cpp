#include "gameMusic.h"
#include "LPC17xx.h"
#include "PinNames.h"
#include "game.h"
#include "globals.h"
#include "menu.h"
#include "normalProjectileSound.h"
#include "laserSound.h"
#include "explosionSound.h"
#include "hit.h"
#include "button.h"
#include "win.h"
#include "lose.h"
#include "missile.h"
#include <cstdint>

#define MAX_DATA_SIZE 8192

AnalogOut sound(p18);
unsigned short *bufferZero = (unsigned short*)0x2007C000;
unsigned short *bufferOne = (unsigned short*)0x2007E000;
WAVHeader header;
unsigned int filePos = 78;
unsigned int fileEnd = 0;
Ticker gameSound;
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
FILE *songFile;

void musicInit(void) {
    songFile = fopen("/sd/Interstellar.wav", "rb");
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

void loadMusic(void) {
    if (!bufferReady) {
        //fseek(songFile, filePos, SEEK_SET);
        unsigned short *addr;
        if (bufferInUse == 0) {
            addr = bufferOne;
        } else {
            addr = bufferZero;
        }
        // Read the audio data
        fread(addr, sizeof(unsigned short), MAX_DATA_SIZE / sizeof(unsigned short), songFile);
        // Check if end of file plus a little more is reached so as to not loop back immediately
        if (filePos >= fileEnd + MAX_DATA_SIZE * 3) {
            // Reset file position to the beginning
            filePos = 78;
            fseek(songFile, filePos, SEEK_SET);
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
    double scale = (getMenuSettings()->volume != NULL) ? getMenuSettings()->volume : 0.5;
    // Output the mixed audio
    if (bufferInUse == 0) {
        sound.write_u16((unsigned short)(((bufferZero[mCounter++] + data) & 0xffff) * (0.875 * (pow(scale, 3)+ 0.125))));
    } else if (bufferInUse == 1) {
        sound.write_u16((unsigned short)(((bufferOne[mCounter++] + data) & 0xffff) * (0.875 * (pow(scale, 3)+ 0.125))));
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