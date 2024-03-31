#ifndef PLAYER_H
#define PLAYER_H

#include "doublyLinkedList.h"
#include "mbed.h"
#include "healthBar.h"
#include "globals.h"
#include "graphics.h"

//==== [private settings] ====
#define PLAYER_YPOS 11
#define PLAYER_XPOS 64
#define PLAYER_SPEED 4
#define PLAYER_WIDTH 11
#define PLAYER_HEIGHT 11
#define MAX_SUPER_CHARGE 10

typedef struct {
    int x;
    int y;
    int px;
    int py;
    int width;
    int height;
    BOUNDINGBOX* boundingBox;
    int score;
    int speed;
    int tick;
    int lastFireTick;
    int fireInterval;
    int superChargeStatus; // Maxes out at 5
    bool superActive; // True during super duration, prevents normal attacking
    CHARACTER_TYPE type;
    CHARACTER_STATUS playerStatus;
    CHARACTER_DISPLAY playerDisplay;
    int damagedTick;
    int lastDamaged;
    int destroyedTick;
    LLNode* healthBar;
    int sessionPlayTime;
    int sessionKills;
    int sessionDeaths;
} PLAYER;

/** Get the player object from other files
 */
PLAYER* getPlayer(void);

/** Create a player and healthBar
*/
void playerInit(void);

/** Move up if possible
*/
void playerMoveUp(void);

/** Move down if possible
*/
void playerMoveDown(void);

/** Move left if possible
*/
void playerMoveLeft(void);

/** Move right if possible
*/
void playerMoveRight(void);

/** Fire a normal player projectile
 */
void playerFireProjectile(void);

/** Activate player laser attack
 */
void playerSuperAttack(void);

/** Update player position and render player using drawPlayer
 */
void playerUpdate(void);

/** Deallocate player memory
 */
void deletePlayer(void);

#endif