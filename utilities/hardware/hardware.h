#ifndef HARDWARE_H
#define HARDWARE_H

/**
 * Structure that represents all the inputs to the game.
 * If additional hardware is added, new elements should be added to this struct.
 */
typedef struct {
    int up, down, left, right, center, normalAttack, superAttack, pauseResume, quitGame;
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

#endif // HARDWARE_H