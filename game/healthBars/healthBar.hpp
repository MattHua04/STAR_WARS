#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "doublyLinkedList.hpp"
#include "mbed.h"
#include "globals.hpp"
#include "graphics.hpp"

//==== [private settings] ====
#define BAR_HEIGHT 3
#define BAR_WIDTH 11
#define BAR_COLOR 0xFF0000 //red

// HealthBar types and their health values
typedef enum {
    TWO_HEARTS=2,
    THREE_HEARTS=3,
    TEN_HEARTS=10,
} BAR_TYPE;

typedef struct {
    int x;
    int y;
    BAR_TYPE barType;
    int numHearts;
    LLNode* barOwner;
} BAR;

/** Create a DLL for healthBars 
 */
void healthBarInit(void);

/** Loop through all healthBars and update healthBar positions accordingly
 */
void updateBars(void);

/** Create a new healthBar for an enemy 
 */
LLNode* createEnemyBar(LLNode* character);

/** Create a new healthBar for a player 
 */
LLNode* createPlayerBar(LLNode* character);

/** Create a new healthBar for a opponent player 
 */
LLNode* createOpponentBar(LLNode* character);

/** Create a new healthBar for a final boss 
 */
LLNode* createBossBar(LLNode* character);

/** Deallocate all memory used for enemy health bars
 */
void deallocateEnemyHealthBars(void);

/** Delete player health bar
 */
void deletePlayerHealthBar(void);

/** Delete opponent player health bar
 */
void deleteOpponentHealthBar(void);

/** Delete boss health bar
 */
void deleteBossHealthBar(void);

#endif