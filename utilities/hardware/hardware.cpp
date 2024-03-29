#include "globals.h"
#include "doublyLinkedList.h"
#include "player.h"
#include "hardware.h"
#include "game.h"
#include "uLCD.hpp"

uLCD_4DGL uLCD(p13,p14,p15);
Nav_Switch myNav(p26, p29, p28, p30, p27);
DigitalIn normalAttack(p22);
DigitalIn superAttack(p23);
DigitalIn pauseResume(p24);
DigitalIn quitGame(p25);
SDBlockDevice sd_block(p5,p6,p7,p8);
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
    gameInputs->center = myNav.center();
    gameInputs->normalAttack = normalAttack;
    gameInputs->superAttack = superAttack;
    gameInputs->pauseResume = pauseResume;
    gameInputs->quitGame = quitGame;
    return gameInputs;
}
