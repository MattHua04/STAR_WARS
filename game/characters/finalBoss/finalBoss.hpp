#ifndef FINALBOSS_H
#define FINALBOSS_H

#include "doublyLinkedList.hpp"
#include "mbed.h"
#include "healthBar.hpp"
#include "globals.hpp"
#include "graphics.hpp"

#define BOSS_WIDTH 19
#define BOSS_HEIGHT 19
#define BOSS_SPEED 2
#define BOSS_YPOS 64
#define BOSS_XPOS 64

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
    int specialAttackInterval;
    bool specialAttacking;
    CHARACTER_STATUS status;
    CHARACTER_DISPLAY bossDisplay;
    int damagedTick;
    int destroyedTick;
} BOSS;

/** Get final boss
 */
BOSS* getBoss(void);

/** Create a final boss
 */
void bossInit(void);

/** Update boss position and render
 */
void bossUpdate(void);

/** Generate three offset long range projectiles
 */
void bossFireProjectile(LLNode* boss);

/** Generate three offset guided missiles
 */
void bossSpecialAttack(LLNode* boss);

/** Delete final boss
 */
void deleteBoss(void);

#endif