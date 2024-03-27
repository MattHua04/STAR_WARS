#include "gameMusic.h"
#include "LPC17xx.h"
#include "PinNames.h"
#include "globals.h"
#include "normalProjectileSound.h"
#include "laserSound.h"
#include "explosionSound.h"
#include "hit.h"
#include <cstdint>

#define MAX_DATA_SIZE 8192

AnalogOut sound(p18);
wave_player audioPlayer(&sound);
unsigned short *bufferZero = (unsigned short*)0x2007C000;
unsigned short *bufferOne = (unsigned short*)0x20080000;
unsigned int filePos = 78;
Ticker mSoundLoader;
Ticker mSound;
Ticker pSound;
Ticker lSound;
Ticker xSound;
Ticker hSound;
int bufferReady = 0;
int bufferInUse = 0;
int mCounter = 0;
int pCounter = 0;
int lCounter = 0;
int xCounter = 0;
int hCounter = 0;
FILE *songFile;

void printBuffer(void) {
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
    
    bufferZero = (unsigned short*) malloc(MAX_DATA_SIZE / sizeof(short));
    bufferOne = (unsigned short*) malloc(MAX_DATA_SIZE / sizeof(short));
    /**for (int i = 0; i < 4; i++) {
        loadMusic();
        bufferInUse = !bufferInUse;
        bufferReady = 0;
        printf("\n");
    }*/
    mSoundLoader.attach(&loadMusic, MAX_DATA_SIZE / 11025.0);
}

void musicInit(void) {
    songFile = fopen("/sd/Interstellar.wav", "rb");
    prepareMusicLoader();
    mSound.attach(&playMusic, 1.0 / 11025.0);
    /**char wav[]="/sd/Interstellar.wav";
    FILE *wave_songFile;
    wave_songFile=fopen(wav,"r");
    if (wave_songFile == NULL) {
        printf("Failed to open songFile\n");
        return;
    }
    audioPlayer.play(wave_songFile);
    fclose(wave_songFile);*/
}

void loadMusic(void) {
    if (!bufferReady) {
        fseek(songFile, filePos, SEEK_SET);

        unsigned int data_size = MAX_DATA_SIZE;

        // Allocate memory for the data
        short *samples = (short *)malloc(data_size * sizeof(unsigned short));
        if (!samples) {
            printf("Error: Memory allocation failed.\n");
            return;
        }

        // Read the data
        size_t bytes_read = fread(samples, sizeof(unsigned short), data_size / sizeof(unsigned short), songFile);

        // Check if end of file is reached
        if (bytes_read == 0) {
            // Reset file position to the beginning
            filePos = 78;
            fseek(songFile, filePos, SEEK_SET);
            // Read data again
            bytes_read = fread(samples, sizeof(unsigned short), data_size, songFile);
        }

        unsigned short *addr;
        if (bufferInUse == 0) {
            addr = bufferOne;
        } else {
            addr = bufferZero;
        }
        for (int i = 0; i < data_size / sizeof(unsigned short); i++) {
            *addr = (samples[i] + 32768) & 0xffff;
            //printf("%d\n", *addr);
            addr++;
        }

        filePos += MAX_DATA_SIZE; // Save the current position in the WAV file
        free(samples);
        
        bufferReady = 1;
    }
}

void playMusic(void) {
    if (pCounter == 0 && lCounter == 0 && xCounter == 0 && hCounter == 0) {
        if (bufferInUse == 0) {
            sound.write_u16(bufferZero[mCounter]>>6);
        } else if (bufferInUse == 1) {
            sound.write_u16(bufferOne[mCounter]>>6);
        }
    }
    mCounter++;
    if (mCounter >= MAX_DATA_SIZE / sizeof(short)) {
        mCounter = 0;
        bufferInUse = !bufferInUse;
        bufferReady = 0;
    }
}

void projectileSound(void) {
    pSound.attach(&playProjectileSound, 1.0 / 11025.0);
}

void playProjectileSound(void) {
    /**if (xCounter != 0 || hCounter != 0 || lCounter != 0) {
        pCounter++;
        if (pCounter >= 2360) {
            pCounter = 0;
            pSound.detach();
        }
    }*/
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
    sound.write_u16(lSoundData[lCounter]>>6);
    lCounter++;
    if (lCounter >= 33748) {
        lCounter = 0;
        lSound.detach();
    }
}

void explosionSound(void) {
    xSound.attach(&playExplosionSound, 1.0 / 11025.0);
}

void playExplosionSound(void) {
    /**if (pCounter != 0 || hCounter != 0 || lCounter != 0) {
        xCounter++;
        if (xCounter >= 2360) {
            xCounter = 0;
            xSound.detach();
        }
    }*/
    sound.write_u16(xSoundData[xCounter]>>6);
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
    /**if (pCounter != 0 || xCounter != 0 || lCounter != 0) {
        hCounter++;
        if (hCounter >= 2360) {
            hCounter = 0;
            hSound.detach();
        }
    }*/
    sound.write_u16(hSoundData[hCounter]>>6);
    hCounter++;
    if (hCounter >= 2048) {
        hCounter = 0;
        hSound.detach();
    }
}