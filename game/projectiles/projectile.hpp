#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "doublyLinkedList.hpp"
#include "mbed.h"
#include "globals.hpp"
#include "graphics.hpp"

#define PROJECTILE_WIDTH 1
#define PROJECTILE_HEIGHT 5
#define MISSILE_WIDTH 3
#define MISSILE_HEIGHT 7
#define LASER_WIDTH 3
#define LASER_HEIGHT 120
#define SHORT_RANGE_SPEED 2
#define LONG_RANGE_SPEED 3
#define MISSILE_SPEED 1
#define PVP_SPEED 6 // Make things more interesting

// Projectile types and their damage
typedef enum {
    SHORT=1,
    LONG=2,
    MISSILE=3,
    LASER=1,
} PROJECTILE_TYPE;

// Info about projectile movement
typedef struct {
    double pdx;
    double pdy;
    double dx;
    double dy;
} PROJECTILE_DIRECTION;

// Projectile struct
typedef struct {
    int x;
    int y;
    int px;
    int py;
    int projectileWidth;
    int projectileHeight;
    BOUNDINGBOX* boundingBox;
    LLNode* projectileOwner;
    PROJECTILE_TYPE projectileType;
    int projectileSpeed;
    PROJECTILE_DIRECTION* projectileDirection;
    int projectileTick; // only used for laser attack
} PROJECTILE;

/** Create a DLL for projectiles
*/
void projectileInit(void);

/** Add an enemyProjectile to projectileDLL.
 * Initialize projectile and drawEnemyProjectile.
*/
void generateEnemyProjectile(LLNode* projectileOwner);

/** Add a playerProjectile to projectileDLL.
 * Initialize projectile and drawPlayerProjectile.
*/
void generatePlayerProjectile(LLNode* projectileOwner);

/** Add a opponentProjectile to projectileDLL.
 * Initialize projectile and drawOpponentProjectile.
*/
void generateOpponentProjectile(LLNode* projectileOwner);

/** Add a bossProjectile to projectileDLL.
 * Initialize projectile and drawEnemyProjectile.
*/
void generateBossProjectile(LLNode* projectileOwner);

/** Loop through all projectiles in enemyProjectileDLL and
 * move each projectile by default distance scaled by projectileSpeed
 * (If projectileType is missile, calculate x and y offsets in direction of player)
 * Update projectileDirection for each projectile appropriately.
 * Check if projectile collides with screen bounds.
 * Check if projectile collides with player.
*/
void updateEnemyProjectiles(void);

/** Loop through all projectiles in playerProjectileDLL and
 * move each projectile by default distance scaled by projectileSpeed
 * Check if projectile collides with screen bounds.
 * Check if projectile collides with enemy.
*/
void updatePlayerProjectiles(void);

/** Loop through all projectiles in opponentProjectileDLL and
 * move each projectile by default distance scaled by projectileSpeed
 * Check if projectile collides with screen bounds.
 * Check if projectile collides with enemy.
*/
void updateOpponentProjectiles(void);

/** Loop through all projectiles in bossProjectileDLL and
 * move each projectile by default distance scaled by projectileSpeed
 * (If projectileType is missile, calculate x and y offsets in direction of player)
 * Update projectileDirection for each projectile appropriately.
 * Check if projectile collides with screen bounds.
 * Check if projectile collides with player.
*/
void updateBossProjectiles(void);

/** Detects intersections between projectile and character
*/
int projectileHit(BOUNDINGBOX* pBox, BOUNDINGBOX* cBox);

/** Deallocate all enemy projectiles
*/
void deallocateEnemyProjectileDLL(void);

/** Deallocate all player projectiles
*/
void deallocatePlayerProjectileDLL(void);

/** Deallocate all boss projectiles
*/
void deallocateBossProjectileDLL(void);

#endif