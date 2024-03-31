#include "globals.hpp"
#include "doublyLinkedList.hpp"
#include "player.hpp"
#include "hardware.hpp"
#include "game.hpp"
#include "uLCD.hpp"

uLCD_4DGL uLCD(p13, p14, p15);
Nav_Switch myNav(p26, p29, p28, p30, p30); // Don't need center so just saving a pin. Would be p12, p15, p14, p16, p16 for provided schematic
DigitalIn normalAttack(p22); // Would be p21 for provided schematic
DigitalIn superAttack(p23); // Would be p22 for provided schematic
DigitalIn pauseResume(p24); // Would be p23 for provided schematic
DigitalIn quitGame(p25); // Would be p24 for provided schematic
Nav_Switch opponentNav(p9, p10, p11, p12, p12); // Don't need center so just saving a pin. Would be p17, p19, p20, p25, p25 for provided schematic
DigitalIn opponentNormalAttack(p17); // Would be p26 for provided schematic
DigitalIn opponentSuperAttack(p19); // Would be p27 for provided schematic
DigitalIn opponentPauseResume(p20); // Would be p28 for provided schematic
DigitalIn opponentQuitGame(p21); // Would be p29 for provided schematic
DigitalOut notifyInPvpMode(p16); // Would be p13 for provided schematic
DigitalIn readInPvpMode(p27); // Would be p30 for provided schematic
SDBlockDevice sd_block(p5, p6, p7, p8);
FATFileSystem fs("sd", &sd_block);
AnalogOut DACout(p18);

GAME_INPUTS* gameInputs;

int hardware_init(void)
{
    uLCD.baudrate(1500000);
    normalAttack.mode(PullUp); 
    superAttack.mode(PullUp);
    pauseResume.mode(PullUp);
    quitGame.mode(PullUp);
    opponentNormalAttack.mode(PullUp); 
    opponentSuperAttack.mode(PullUp);
    opponentPauseResume.mode(PullUp);
    opponentQuitGame.mode(PullUp);
    notifyInPvpMode.write(0);
    gameInputs = (GAME_INPUTS*)malloc(sizeof(GAME_INPUTS));
    fs.mount(&sd_block);
    return ERROR_NONE;
}

void deallocateHardware(void)
{
    free(gameInputs);
}

GAME_INPUTS* readInputs(void) {
    gameInputs->up = myNav.up();
    gameInputs->down = myNav.down();
    gameInputs->left = myNav.left();
    gameInputs->right = myNav.right();
    gameInputs->normalAttack = normalAttack;
    gameInputs->superAttack = superAttack;
    gameInputs->pauseResume = pauseResume;
    gameInputs->quitGame = quitGame;
    gameInputs->opUp = opponentNav.up();
    gameInputs->opDown = opponentNav.down();
    gameInputs->opLeft = opponentNav.left();
    gameInputs->opRight = opponentNav.right();
    gameInputs->opNormalAttack = opponentNormalAttack;
    gameInputs->opSuperAttack = opponentSuperAttack;
    gameInputs->opPauseResume = opponentPauseResume;
    gameInputs->opQuitGame = opponentQuitGame;
    return gameInputs;
}

void notifyPvp(bool ready) {
    notifyInPvpMode.write(ready);
}

int readPvp(void) {
    return readInPvpMode == 1;
}