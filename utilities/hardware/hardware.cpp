#include "PinNameAliases.h"
#include "PinNames.h"
#include "gameMusic.hpp"
#include "globals.hpp"
#include "doublyLinkedList.hpp"
#include "menu.hpp"
#include "opponent.hpp"
#include "player.hpp"
#include "hardware.hpp"
#include "game.hpp"

uLCD_4DGL uLCD(p13, p14, p15); // Would be p9, p10, p11 for provided schematic

// u, d, l, r
// Using DI and DO for controls to generate sustained signals between input reads needed for pvp input syncing
DigitalIn readUp(p26);
DigitalOut writeUp(p26);
DigitalIn readDown(p29);
DigitalOut writeDown(p29);
DigitalIn readLeft(p28);
DigitalOut writeLeft(p28);
DigitalIn readRight(p30);
DigitalOut writeRight(p30);

DigitalIn normalAttack(p22); // Would be p21 for provided schematic
DigitalOut writeNormalAttack(p22);
DigitalIn superAttack(p23); // Would be p22 for provided schematic
DigitalOut writeSuperAttack(p23);
DigitalIn pauseResume(p24); // Would be p23 for provided schematic
DigitalOut writePauseResume(p24);
DigitalIn quitGame(p25); // Would be p24 for provided schematic
DigitalOut writeQuitGame(p25);

// u, d, l, r
// Only using DI for opponent controls since don't need to write anything
DigitalIn readOpUp(p9);
DigitalIn readOpDown(p10);
DigitalIn readOpLeft(p11);
DigitalIn readOpRight(p12);

DigitalIn opponentNormalAttack(p17); // Would be p26 for provided schematic
DigitalIn opponentSuperAttackButton(p19); // Would be p27 for provided schematic
DigitalIn opponentPauseResume(p20); // Would be p28 for provided schematic
DigitalIn opponentQuitGame(p21); // Would be p29 for provided schematic

DigitalOut notifyInPvpMode(p16); // Also used for syncing devices. Would be p13 for provided schematic
DigitalIn readInPvpMode(p27); // Also used for syncing devices. Would be p30 for provided schematic

SDBlockDevice sd_block(p5, p6, p7, p8);
FATFileSystem fs("sd", &sd_block);
AnalogOut DACout(p18);

GAME_INPUTS* gameInputs;

int hardware_init(void)
{
    uLCD.baudrate(1500000);
    // Player controls
    readUp.mode(PullUp);
    writeUp.write(1);
    readDown.mode(PullUp);
    writeDown.write(1);
    readLeft.mode(PullUp);
    writeLeft.write(1);
    readRight.mode(PullUp);
    writeRight.write(1);
    // Opponent controls
    readOpUp.mode(PullNone);
    readOpDown.mode(PullNone);
    readOpLeft.mode(PullNone);
    readOpRight.mode(PullNone);
    // Player buttons
    normalAttack.mode(PullUp); 
    writeNormalAttack.write(1);
    superAttack.mode(PullUp);
    writeSuperAttack.write(1);
    pauseResume.mode(PullUp);
    writePauseResume.write(1);
    quitGame.mode(PullUp);
    writeQuitGame.write(1);
    //Opponent buttons
    opponentNormalAttack.mode(PullNone);
    opponentSuperAttackButton.mode(PullNone);
    opponentPauseResume.mode(PullNone);
    opponentQuitGame.mode(PullNone);
    // Device syncing pins
    notifyInPvpMode.write(0);
    readInPvpMode.mode(PullNone);

    gameInputs = (GAME_INPUTS*)malloc(sizeof(GAME_INPUTS));
    fs.mount(&sd_block);
    return 0;
}

void deallocateHardware(void)
{
    free(gameInputs);
}

GAME_INPUTS* getCurrentInputs(void) {
    return gameInputs;
}

GAME_INPUTS* readInputs(void) {
    // Reset NavSwitch and buttons
    writeUp.write(1);
    writeDown.write(1);
    writeLeft.write(1);
    writeRight.write(1);
    writeNormalAttack.write(1);
    writeSuperAttack.write(1);
    writePauseResume.write(1);
    writeQuitGame.write(1);
    int ups = 0, downs = 0, lefts = 0, rights = 0, normalAttacks = 0, superAttacks = 0, pauseResumes = 0, quitGames = 0;
    int opUps = 0, opDowns = 0, opLefts = 0, opRights = 0, opNormalAttacks = 0, opSuperAttacks = 0, opPauseResumes = 0, opQuitGames = 0;
    Timer pollInputs;
    pollInputs.start();
    // Poll inputs for some time
    while (pollInputs.elapsed_time().count() < 1000) {
        ups += !readUp;
        downs += !readDown;
        lefts += !readLeft;
        rights += !readRight;
        normalAttacks += !normalAttack;
        superAttacks += !superAttack;
        pauseResumes += !pauseResume;
        quitGames += !quitGame;

        opUps += !readOpUp;
        opDowns += !readOpDown;
        opLefts += !readOpLeft;
        opRights += !readOpRight;
        opNormalAttacks += !opponentNormalAttack;
        opSuperAttacks += !opponentSuperAttackButton;
        opPauseResumes += !opponentPauseResume;
        opQuitGames += !opponentQuitGame;
    }
    writeUp.write(!(gameInputs->up = (ups > 0) ? 1 : 0));
    writeDown.write(!(gameInputs->down = (downs > 0 && ups == 0) ? 1 : 0));
    writeLeft.write(!(gameInputs->left = (lefts > 0 && ups == 0 && downs == 0) ? 1 : 0));
    writeRight.write(!(gameInputs->right = (rights > 0 && ups == 0 && downs == 0 && lefts == 0) ? 1 : 0));
    writeNormalAttack.write(gameInputs->normalAttack = (normalAttacks > 0) ? 0 : 1);
    writeSuperAttack.write(gameInputs->superAttack = (superAttacks > 0) ? 0 : 1);
    writePauseResume.write(gameInputs->pauseResume = (pauseResumes > 0) ? 0 : 1);
    writeQuitGame.write(gameInputs->quitGame = (quitGames > 0) ? 0 : 1);
    gameInputs->opUp = (opUps > 0) ? 1 : 0;
    gameInputs->opDown = (opDowns > 0 && opUps == 0) ? 1 : 0;
    gameInputs->opLeft = (opLefts > 0 && opUps == 0 && opDowns == 0) ? 1 : 0;
    gameInputs->opRight = (opRights > 0 && opUps == 0 && opDowns == 0 && opLefts == 0) ? 1 : 0;
    gameInputs->opNormalAttack = (opNormalAttacks > 0) ? 0 : 1;
    gameInputs->opSuperAttack = (opSuperAttacks > 0) ? 0 : 1;
    gameInputs->opPauseResume = (opPauseResumes > 0) ? 0 : 1;
    gameInputs->opQuitGame = (opQuitGames > 0) ? 0 : 1;
    return gameInputs;
}

GAME_INPUTS* readMyInputs(void) {
    // Reset NavSwitch and buttons
    writeUp.write(1);
    writeDown.write(1);
    writeLeft.write(1);
    writeRight.write(1);
    writeNormalAttack.write(1);
    writeSuperAttack.write(1);
    writePauseResume.write(1);
    writeQuitGame.write(1);
    int ups = 0, downs = 0, lefts = 0, rights = 0, normalAttacks = 0, superAttacks = 0, pauseResumes = 0, quitGames = 0;
    Timer pollInputs;
    pollInputs.start();
    // Poll inputs for some time
    while (pollInputs.elapsed_time().count() < 1000) {
        ups += !readUp;
        downs += !readDown;
        lefts += !readLeft;
        rights += !readRight;
        normalAttacks += !normalAttack;
        superAttacks += !superAttack;
        pauseResumes += !pauseResume;
        quitGames += !quitGame;
    }
    writeUp.write(!(gameInputs->up = (ups > 0) ? 1 : 0));
    writeDown.write(!(gameInputs->down = (downs > 0 && ups == 0) ? 1 : 0));
    writeLeft.write(!(gameInputs->left = (lefts > 0 && ups == 0 && downs == 0) ? 1 : 0));
    writeRight.write(!(gameInputs->right = (rights > 0 && ups == 0 && downs == 0 && lefts == 0) ? 1 : 0));
    writeNormalAttack.write(gameInputs->normalAttack = (normalAttacks > 0) ? 0 : 1);
    writeSuperAttack.write(gameInputs->superAttack = (superAttacks > 0) ? 0 : 1);
    writePauseResume.write(gameInputs->pauseResume = (pauseResumes > 0) ? 0 : 1);
    writeQuitGame.write(gameInputs->quitGame = (quitGames > 0) ? 0 : 1);
    return gameInputs;
}

GAME_INPUTS* readOpponentInputs(void) {
    int opUps = 0, opDowns = 0, opLefts = 0, opRights = 0, opNormalAttacks = 0, opSuperAttacks = 0, opPauseResumes = 0, opQuitGames = 0;
    Timer pollInputs;
    pollInputs.start();
    // Poll inputs for some time
    while (pollInputs.elapsed_time().count() < 1000) {
        opUps += !readOpUp;
        opDowns += !readOpDown;
        opLefts += !readOpLeft;
        opRights += !readOpRight;
        opNormalAttacks += !opponentNormalAttack;
        opSuperAttacks += !opponentSuperAttackButton;
        opPauseResumes += !opponentPauseResume;
        opQuitGames += !opponentQuitGame;
    }
    gameInputs->opUp = (opUps > 0) ? 1 : 0;
    gameInputs->opDown = (opDowns > 0 && opUps == 0) ? 1 : 0;
    gameInputs->opLeft = (opLefts > 0 && opUps == 0 && opDowns == 0) ? 1 : 0;
    gameInputs->opRight = (opRights > 0 && opUps == 0 && opDowns == 0 && opLefts == 0) ? 1 : 0;
    gameInputs->opNormalAttack = (opNormalAttacks > 0) ? 0 : 1;
    gameInputs->opSuperAttack = (opSuperAttacks > 0) ? 0 : 1;
    gameInputs->opPauseResume = (opPauseResumes > 0) ? 0 : 1;
    gameInputs->opQuitGame = (opQuitGames > 0) ? 0 : 1;
    return gameInputs;
}

void notifyPvp(bool ready) {
    notifyInPvpMode.write((ready) ? 1 : 0);
}

int readSyncState(void) {
    return notifyInPvpMode.read();
}

int readPvp(void) {
    return readInPvpMode.read();
}

void syncDevices(void) {
    if (!readSyncState()) {
        notifyPvp(true);
        while (!readPvp() && getOpponent() != NULL && getOpponent()->playerDisplay != CHARACTER_DISPLAY::DESTROYED && getPlayer()->playerDisplay != CHARACTER_DISPLAY::DESTROYED);
    } else {
        notifyPvp(false);
        while (readPvp() && getOpponent() != NULL && getOpponent()->playerDisplay != CHARACTER_DISPLAY::DESTROYED && getPlayer()->playerDisplay != CHARACTER_DISPLAY::DESTROYED);
    }
}