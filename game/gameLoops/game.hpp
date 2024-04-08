#ifndef GAME_H
#define GAME_H

#define SYNC_TIME 10000

/**
 * Structure that represents all the inputs to the game.
 * If additional hardware is added, new elements should be added to this struct.
 */
typedef enum {
    PAUSED,
    RUNNING,
    WON,
    LOST,
} GAMESTATUS;

typedef struct {
    GAMESTATUS gameStatus;
    int level;
} GAMELOOP;

/** Generate a random number
 */
int randomNum(void);

/** Get current gameLoop status
 */
GAMELOOP* getGameLoop(void);

/** Make a new gameLoop
 */
void generateGameLoop(int level);

/** Testing level
 */
void testLevel(void);

/** Make an easy level
 */
void levelOne(void);

/** Make a medium level
 */
void levelTwo(void);

/** Make a hard level
 */
void levelThree(void);

/** Make an infinite duration level
 */
void infiniteDuration(void);

/** Make a score capped level
 */
void scoreCap(void);

/** Make a pvp game
 */
void pvp(void);

/** Start a gameLoop
 */
void start(bool infinite, bool scoreCap, bool pvp);

#endif