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
 * Just return the current game inputs
 * Useful for maintaing a constant set of inputs through one gameloop iteration
 */
GAME_INPUTS* getCurrentInputs(void);

/**
 * Read all the user and opponent inputs. 
 */
GAME_INPUTS* readInputs(void);

/**
 * Read only the user inputs. 
 */
GAME_INPUTS* readMyInputs(void);

/**
 * Read only the opponent inputs. 
 */
GAME_INPUTS* readOpponentInputs(void);

/**
 * Tell the opponent that player is in PvP mode
 */
void notifyPvp(bool ready);

/**
 * See if the opponent is in PvP mode
 */
int readPvp(void);

/**
 * Send a signal to other device during syncing
 */
void syncSignal(bool state);

/**
 * Read the state of syncing pin
 */
int readSyncState(void);

#endif // HARDWARE_H