#ifndef ENEMY_H
#define ENEMY_H

#include "doublyLinkedList.hpp"
#include "mbed.h"
#include "healthBar.hpp"
#include "globals.hpp"
#include "graphics.hpp"

#define ENEMY_WIDTH 11
#define ENEMY_HEIGHT 11
#define SHORT_RANGE_ENEMY_SPEED 2
#define LONG_RANGE_ENEMY_SPEED 1
#define MISSILE_ENEMY_SPEED 1
#define SHORT_RANGE_ENEMY_YPOS 42
#define MISSILE_ENEMY_YPOS 63
#define LONG_RANGE_ENEMY_YPOS 90

typedef struct {
    int x;
    int y;
    int px;
    int py;
    CHARACTER_TYPE enemyType;
    int speed;
    BOUNDINGBOX* boundingBox;
    int width;
    int height;
    LLNode* healthBar;
    bool entering;
    int targetY;
    int tick;
    int fireInterval;
    CHARACTER_STATUS status;
    CHARACTER_DISPLAY enemyDisplay;
    int damagedTick;
    int destroyedTick;
} ENEMY;

/** Get enemyDLL
*/
DLinkedList* getEnemyDLL(void);

/** Create a DLL for enemies
*/
void enemyInit(void);

/** Add an enemy to enemyDLL.
 * Initialize enemy and drawEnemy.
 */
void enemyGenerator(CHARACTER_TYPE enemyType);

/** Update enemy position depending on the enemy type and drawEnemy
 */
void enemyUpdatePos(LLNode* enemy);

/** Loop through all enemies in enemyDLL and checks if the enemy is able to fire and call enemyUpdatePos on each one
 * If an enemy has been destroyed then deallocate its memory and remove from dll
 */
void enemiesUpdate(void);

/** Call generateProjectile and pass associated enemy
 */
void enemyFireProjectile(LLNode* enemy);

/** Deallocate all memory used for enemies
 */
void deallocateEnemies(void);

#endif