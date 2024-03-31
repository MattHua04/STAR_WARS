#ifndef HARDWARE_H
#define HARDWARE_H

/**
 * Structure that represents all the inputs to the game.
 * If additional hardware is added, new elements should be added to this struct.
 */
#include "DigitalOut.h"
typedef struct {
    // Player controls
    int up, down, left, right, normalAttack, superAttack, pauseResume, quitGame;
    // Opponent controls
    int opUp, opDown, opLeft, opRight, opNormalAttack, opSuperAttack, opPauseResume, opQuitGame;
} GAME_INPUTS;

/**
 * Initialize all the hardware.
 */
int hardware_init(void);

/**
 * Free memory allocated for hardware.
 */
void deallocateHardware(void);

/**
 * Read all the user inputs. 
 * This is all input hardware interaction should happen.
 * Returns a GameInputs struct that has all the inputs recorded.
 * This GameInputs is used elsewhere to compute the game update.
 */
GAME_INPUTS* readInputs(void);

/**
 * Tell the opponent that player is in PvP mode
 */
void notifyPvp(bool ready);

/**
 * See if the opponent is in PvP mode
 */
int readPvp(void);

#endif // HARDWARE_H