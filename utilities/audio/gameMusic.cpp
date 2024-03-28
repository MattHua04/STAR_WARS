#include "gameMusic.h"
#include "LPC17xx.h"
#include "PinNames.h"
#include "globals.h"
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
wave_player audioPlayer(&sound);
unsigned short *bufferZero = (unsigned short*)0x2007C000;
unsigned short *bufferOne = (unsigned short*)0x2007E000;
unsigned int filePos = 78;
Ticker mSoundLoader;
Ticker mSound;
Ticker pSound;
Ticker lSound;
Ticker missileSoundPlayer;
Ticker xSound;
Ticker hSound;
Ticker bSound;
Ticker winSoundPlayer;
Ticker loseSoundPlayer;
int bufferReady = 0;
int bufferInUse = 0;
int mCounter = 0;
int pCounter = 0;
int lCounter = 0;
int missileCounter = 0;
int xCounter = 0;
int hCounter = 0;
int bCounter = 0;
int winCounter = 0;
int loseCounter = 0;
FILE *songFile;

void printBuffer(void) {
    return;
    printf("%d\n", filePos);
    return;
    if (bufferInUse == 0) {
        printf("%d\n", bufferOne[0]);
    } else {
        printf("%d\n", bufferZero[0]);
    }
    bufferInUse = !bufferInUse;
    bufferReady = 0;
}

void prepareMusicLoader(void) {
    songFile = fopen("/sd/Interstellar.wav", "rb");
    //mSoundLoader.attach(&loadMusic, 1.0 / 11025.0);
}

void musicInit(void) {
    //songFile = fopen("/sd/Interstellar.wav", "r");
    //audioPlayer.play(songFile);
    //return;
    prepareMusicLoader();
    mSound.attach(&playMusic, 1.0 / 11025.0);
}

void loadMusic(void) {
    if (!bufferReady && pCounter == 0 && lCounter == 0 && xCounter == 0 && hCounter == 0 && xCounter == 0 && bCounter == 0 && missileCounter == 0 && winCounter == 0 && loseCounter == 0) {
        fseek(songFile, filePos, SEEK_SET);

        unsigned short *addr;
        if (bufferInUse == 0) {
            addr = bufferOne;
        } else {
            addr = bufferZero;
        }

        // Read the data
        size_t bytes_read = fread(addr, sizeof(unsigned short), MAX_DATA_SIZE / sizeof(unsigned short), songFile);

        // Check if end of file is reached
        if (bytes_read == 0) {
            // Reset file position to the beginning
            filePos = 78;
            fseek(songFile, filePos, SEEK_SET);
            // Read data again
            bytes_read = fread(addr, sizeof(unsigned short), MAX_DATA_SIZE / sizeof(unsigned short), songFile);
        }

        filePos += MAX_DATA_SIZE; // Save the current position in the WAV file
        
        bufferReady = 1;
    }
}

void playMusic(void) {
    if ((mCounter > 0 || bufferReady) && pCounter == 0 && lCounter == 0 && xCounter == 0 && hCounter == 0 && xCounter == 0 && bCounter == 0 && missileCounter == 0 && winCounter == 0 && loseCounter == 0) {
        if (bufferInUse == 0) {
            sound.write_u16(((bufferZero[mCounter] + 32768) & 0xffff)>>4);
        } else if (bufferInUse == 1) {
            sound.write_u16(((bufferOne[mCounter] + 32768) & 0xffff)>>4);
        }
    }
    mCounter++;
    if (mCounter >= MAX_DATA_SIZE / sizeof(unsigned short)) {
        mCounter = 0;
        bufferInUse = !bufferInUse;
        bufferReady = 0;
    }
}

void projectileSound(void) {
    pSound.attach(&playProjectileSound, 1.0 / 11025.0);
}

void playProjectileSound(void) {
    if (winCounter > 0 || loseCounter > 0) {
        pCounter = 0;
        pSound.detach();
    }
    sound.write_u16(pSoundData[pCounter]>>6);
    pCounter++;
    if (pCounter >= 2360) {
        pCounter = 0;
        pSound.detach();
    }
}

void laserSound(void) {
    lSound.attach(&playLaserSound, 1.0 / 11025.0);
}

void playLaserSound(void) {
    if (winCounter > 0 || loseCounter > 0) {
        lCounter = 0;
        lSound.detach();
    }
    sound.write_u16(lSoundData[lCounter]>>4);
    lCounter++;
    if (lCounter >= 33748) {
        lCounter = 0;
        lSound.detach();
    }
}

void missileSound(void) {
    missileSoundPlayer.attach(&playMissileSound, 1.0 / 11025.0);
}

void playMissileSound(void) {
    if (winCounter > 0 || loseCounter > 0) {
        missileCounter = 0;
        missileSoundPlayer.detach();
    }
    sound.write_u16(missileSoundData[missileCounter]>>4);
    missileCounter++;
    if (missileCounter >= 4608) {
        missileCounter = 0;
        missileSoundPlayer.detach();
    }
}

void explosionSound(void) {
    xSound.attach(&playExplosionSound, 1.0 / 11025.0);
}

void playExplosionSound(void) {
    if (winCounter > 0 || loseCounter > 0) {
        xCounter = 0;
        xSound.detach();
    }
    sound.write_u16(xSoundData[xCounter]>>7);
    xCounter++;
    if (xCounter >= 5344) {
        xCounter = 0;
        xSound.detach();
    }
}

void hitSound(void) {
    hSound.attach(&playHitSound, 1.0 / 11025.0);
}

void playHitSound(void) {
    if (winCounter > 0 || loseCounter > 0) {
        hCounter = 0;
        hSound.detach();
    }
    sound.write_u16(hSoundData[hCounter]>>6);
    hCounter++;
    if (hCounter >= 2048) {
        hCounter = 0;
        hSound.detach();
    }
}

void buttonSound(void) {
    bSound.attach(&playButtonSound, 1.0 / 11025.0);
}

void playButtonSound(void) {
    sound.write_u16(bSoundData[bCounter]>>6);
    bCounter++;
    if (bCounter >= 1882) {
        bCounter = 0;
        bSound.detach();
    }
}

void winSound(void) {
    winSoundPlayer.attach(&playWinSound, 1.0 / 11025.0);
}

void playWinSound(void) {
    sound.write_u16(winSoundData[winCounter]>>6);
    winCounter++;
    if (winCounter >= 28922) {
        winCounter = 0;
        winSoundPlayer.detach();
    }
}

void loseSound(void) {
    loseSoundPlayer.attach(&playLoseSound, 1.0 / 11025.0);
}

void playLoseSound(void) {
    sound.write_u16(loseSoundData[loseCounter]>>4);
    loseCounter++;
    if (loseCounter >= 9939) {
        loseCounter = 0;
        loseSoundPlayer.detach();
    }
}