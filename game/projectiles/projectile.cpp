#include "globals.h"
#include "graphics.h"
#include "player.h"
#include "enemy.h"
#include "finalBoss.h"
#include "projectile.h"
#include "doublyLinkedList.h"
#include <cstdio>
#include <math.h>

DLinkedList* enemyProjectileDLL = NULL;
DLinkedList* playerProjectileDLL = NULL;
DLinkedList* bossProjectileDLL = NULL;

void projectileInit(void) {
    enemyProjectileDLL = createDlinkedlist();
    playerProjectileDLL = createDlinkedlist();
    bossProjectileDLL = createDlinkedlist();
}

void generateEnemyProjectile(LLNode* projectileOwner) {
    PROJECTILE* projectile = (PROJECTILE*)malloc(sizeof(PROJECTILE));
    insertHead(enemyProjectileDLL, projectile);
    LLNode* projectileNode = getHead(enemyProjectileDLL);
    projectile->x = ((ENEMY*)getData(projectileOwner))->x;
    projectile->y = ((ENEMY*)getData(projectileOwner))->y - 8;
    projectile->projectileOwner = projectileOwner;
    switch (((ENEMY*)getData(projectileOwner))->enemyType)
    {
    case CHARACTER_TYPE::SHORT_RANGE_ENEMY:
        projectile->projectileType = PROJECTILE_TYPE::SHORT;
        projectile->projectileSpeed = SHORT_RANGE_SPEED;
        projectile->projectileWidth = PROJECTILE_WIDTH;
        projectile->projectileHeight = PROJECTILE_HEIGHT;
        break;
    case CHARACTER_TYPE::LONG_RANGE_ENEMY:
        projectile->projectileType = PROJECTILE_TYPE::LONG;
        projectile->projectileSpeed = LONG_RANGE_SPEED;
        projectile->projectileWidth = PROJECTILE_WIDTH;
        projectile->projectileHeight = PROJECTILE_HEIGHT;
        break;
    case CHARACTER_TYPE::MISSILE_ENEMY:
        projectile->projectileType = PROJECTILE_TYPE::MISSILE;
        projectile->projectileSpeed = MISSILE_SPEED;
        projectile->projectileWidth = MISSILE_WIDTH;
        projectile->projectileHeight = MISSILE_HEIGHT;
        break;
    }
    if (projectile->projectileType == MISSILE) {
        double dx = getPlayer()->x - projectile->x;
        double dy = getPlayer()->y - projectile->y;
        if (dy > 0) {
            dy = -1;
        }
        // Normalize direction vector
        double length = sqrt(pow(dx, 2) + pow(dy, 2));
        dx = round(dx / length);
        dy = round(dy / length) - 1; // bias y direction down
        // Renormalize direction vector after bias
        length = sqrt(pow(dx, 2) + pow(dy, 2));
        dx = round(dx / length);
        dy = round(dy / length);
        PROJECTILE_DIRECTION* projectileDirection = (PROJECTILE_DIRECTION*)malloc(sizeof(PROJECTILE_DIRECTION));
        projectile->projectileDirection = projectileDirection;
        projectile->projectileDirection->dx = dx;
        projectile->projectileDirection->dy = dy;
        projectile->projectileDirection->pdx = dx;
        projectile->projectileDirection->pdy = dy;
    } else {
        // Non missile enemy projectiles go down
        PROJECTILE_DIRECTION* projectileDirection = (PROJECTILE_DIRECTION*)malloc(sizeof(PROJECTILE_DIRECTION));
        projectile->projectileDirection = projectileDirection;
        projectile->projectileDirection->dx = 0;
        projectile->projectileDirection->dy = -1;
        projectile->projectileDirection->dx = 0;
        projectile->projectileDirection->dy = -1;
    }
    BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    boundingBox->topLeft.x = projectile->x - (projectile->projectileWidth - 1) / 2;
    boundingBox->topLeft.y = projectile->y + (projectile->projectileHeight - 1) / 2;
    boundingBox->bottomRight.x = projectile->x + (projectile->projectileWidth - 1) / 2;
    boundingBox->bottomRight.y = projectile->y - (projectile->projectileHeight - 1) / 2;
    projectile->boundingBox = boundingBox;
    projectile->projectileTick = 0;
    drawEnemyProjectile(projectileNode);
}

void generatePlayerProjectile(LLNode* projectileOwner) {
    PROJECTILE* projectile = (PROJECTILE*)malloc(sizeof(PROJECTILE));
    insertHead(playerProjectileDLL, projectile);
    LLNode* projectileNode = getHead(playerProjectileDLL);
    projectile->projectileOwner = projectileOwner;
    if (((PLAYER*)getData(projectileOwner))->superActive) {
        projectile->x = ((PLAYER*)getData(projectileOwner))->x;
        projectile->y = ((PLAYER*)getData(projectileOwner))->y + 65;
        projectile->projectileType = PROJECTILE_TYPE::LASER;
        projectile->projectileWidth = LASER_WIDTH;
        projectile->projectileHeight = LASER_HEIGHT;
    } else {
        projectile->x = ((PLAYER*)getData(projectileOwner))->x;
        projectile->y = ((PLAYER*)getData(projectileOwner))->y + 8;
        projectile->projectileType = PROJECTILE_TYPE::LONG;
        projectile->projectileSpeed = LONG_RANGE_SPEED;
        projectile->projectileWidth = PROJECTILE_WIDTH;
        projectile->projectileHeight = PROJECTILE_HEIGHT;
    }
    // Player projectile goes up
    PROJECTILE_DIRECTION* projectileDirection = (PROJECTILE_DIRECTION*)malloc(sizeof(PROJECTILE_DIRECTION));
    projectile->projectileDirection = projectileDirection;
    projectile->projectileDirection->dx = 0;
    projectile->projectileDirection->dy = 1;
    projectile->projectileDirection->pdx = 0;
    projectile->projectileDirection->pdy = 1;
    BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    boundingBox->topLeft.x = projectile->x - (projectile->projectileWidth - 1) / 2;
    boundingBox->topLeft.y = projectile->y + (projectile->projectileHeight - 1) / 2;
    boundingBox->bottomRight.x = projectile->x + (projectile->projectileWidth - 1) / 2;
    boundingBox->bottomRight.y = projectile->y - (projectile->projectileHeight - 1) / 2;
    projectile->boundingBox = boundingBox;
    projectile->projectileTick = 0;
    drawPlayerProjectile(projectileNode);
}

void generateBossProjectile(LLNode* projectileOwner) {
    for (int offset = -7; offset <= 7; offset+=7) {
        PROJECTILE* projectile = (PROJECTILE*)malloc(sizeof(PROJECTILE));
        insertHead(bossProjectileDLL, projectile);
        LLNode* projectileNode = getHead(bossProjectileDLL);
        projectile->x = ((BOSS*)getData(projectileOwner))->x + offset;
        projectile->y = ((BOSS*)getData(projectileOwner))->y - 11;
        projectile->projectileOwner = projectileOwner;
        if (((BOSS*)getData(projectileOwner))->specialAttacking) {
            projectile->projectileType = PROJECTILE_TYPE::MISSILE;
            projectile->projectileSpeed = MISSILE_SPEED;
            projectile->projectileWidth = MISSILE_WIDTH;
            projectile->projectileHeight = MISSILE_HEIGHT;
        } else {
            projectile->projectileType = PROJECTILE_TYPE::LONG;
            projectile->projectileSpeed = LONG_RANGE_SPEED;
            projectile->projectileWidth = PROJECTILE_WIDTH;
            projectile->projectileHeight = PROJECTILE_HEIGHT;
        }
        if (projectile->projectileType == MISSILE) {
            double dx = getPlayer()->x - projectile->x;
            double dy = getPlayer()->y - projectile->y;
            if (dy > 0) {
                dy = -1;
            }
            // Normalize direction vector
            double length = sqrt(pow(dx, 2) + pow(dy, 2));
            dx = round(dx / length);
            dy = round(dy / length) - 1; // bias y direction down
            // Renormalize direction vector after bias
            length = sqrt(pow(dx, 2) + pow(dy, 2));
            dx = round(dx / length);
            dy = round(dy / length);
            PROJECTILE_DIRECTION* projectileDirection = (PROJECTILE_DIRECTION*)malloc(sizeof(PROJECTILE_DIRECTION));
            projectile->projectileDirection = projectileDirection;
            projectile->projectileDirection->dx = dx;
            projectile->projectileDirection->dy = dy;
            projectile->projectileDirection->pdx = dx;
            projectile->projectileDirection->pdy = dy;
        } else {
            // Non missile enemy projectiles go down
            PROJECTILE_DIRECTION* projectileDirection = (PROJECTILE_DIRECTION*)malloc(sizeof(PROJECTILE_DIRECTION));
            projectile->projectileDirection = projectileDirection;
            projectile->projectileDirection->dx = 0;
            projectile->projectileDirection->dy = -1;
            projectile->projectileDirection->pdx = 0;
            projectile->projectileDirection->pdy = -1;
        }
        BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
        boundingBox->topLeft.x = projectile->x - (projectile->projectileWidth - 1) / 2;
        boundingBox->topLeft.y = projectile->y + (projectile->projectileHeight - 1) / 2;
        boundingBox->bottomRight.x = projectile->x + (projectile->projectileWidth - 1) / 2;
        boundingBox->bottomRight.y = projectile->y - (projectile->projectileHeight - 1) / 2;
        projectile->boundingBox = boundingBox;
        projectile->projectileTick = 0;
        drawEnemyProjectile(projectileNode);
    }
}

int projectileHit(BOUNDINGBOX* pBox, BOUNDINGBOX* cBox) {
    if (pBox->bottomRight.x <= cBox->bottomRight.x && pBox->bottomRight.x >= cBox->topLeft.x
        && pBox->bottomRight.y >= cBox->bottomRight.y && pBox->bottomRight.y <= cBox->topLeft.y) {
        return 1;
    } else if (pBox->topLeft.x >= cBox->topLeft.x && pBox->topLeft.x <= cBox->bottomRight.x
        && pBox->topLeft.y <= cBox->topLeft.y && pBox->topLeft.y >= cBox->bottomRight.y) {
        return 1;
    } else if (pBox->topLeft.x >= cBox->topLeft.x && pBox->topLeft.x <= cBox->bottomRight.x
        && pBox->topLeft.y >= cBox->topLeft.y && pBox->bottomRight.y <= cBox->bottomRight.y) {
        return 1;
    } else if (pBox->bottomRight.x >= cBox->topLeft.x && pBox->bottomRight.x <= cBox->bottomRight.x
        && pBox->topLeft.y >= cBox->topLeft.y && pBox->bottomRight.y <= cBox->bottomRight.y) {
        return 1;
    }
    return 0;
}

void updateEnemyProjectiles(void) {
    if (enemyProjectileDLL == NULL) {
        return;
    }
    LLNode* currentNode = getHead(enemyProjectileDLL);
    while (currentNode) {
        eraseEnemyProjectile(currentNode);
        // Check for projectile and player collision
        if (getPlayer()->playerStatus != CHARACTER_STATUS::DEAD && projectileHit(((PROJECTILE*)getData(currentNode))->boundingBox, getPlayer()->boundingBox)) {
            ((BAR*)getData(getPlayer()->healthBar))->numHearts -= ((PROJECTILE*)getData(currentNode))->projectileType;
            getPlayer()->lastDamaged = getPlayer()->tick;
            if (((BAR*)getData(getPlayer()->healthBar))->numHearts <= 0) {
                explosionSound();
                ((BAR*)getData(getPlayer()->healthBar))->numHearts = 0;
                getPlayer()->sessionDeaths++;
                getPlayer()->playerStatus = CHARACTER_STATUS::DEAD;
                getPlayer()->playerDisplay = CHARACTER_DISPLAY::DESTROYED;
            } else {
                // If enemy damaged, set display to whatever animation state it was before plus Damaged
                switch(getPlayer()->playerDisplay + 4) { // adding 4 essentially switchs state to damaged form
                case DEFAULTDAMAGED:
                    getPlayer()->playerDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                    break;
                case LEFTDAMAGED:
                    getPlayer()->playerDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                    break;
                case RIGHTDAMAGED:
                    getPlayer()->playerDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                    break;
                case VERTICALDAMAGED:
                    getPlayer()->playerDisplay = CHARACTER_DISPLAY::VERTICALDAMAGED;
                    break;
                }
            }
            LLNode* nextNode = currentNode->next;
            free(((PROJECTILE*)getData(currentNode))->boundingBox);
            deleteNode(enemyProjectileDLL, currentNode);
            currentNode = nextNode;
            continue;
        }
        /** Update enemyProjectile positions
         */
        // Make sure projectile is going to be within screen before updating pos
        // Need to account for projectile size
        if (((PROJECTILE*)getData(currentNode))->x + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dx * ((PROJECTILE*)getData(currentNode))->projectileSpeed + (((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2) <= 127
            && ((PROJECTILE*)getData(currentNode))->x + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dx * ((PROJECTILE*)getData(currentNode))->projectileSpeed - (((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2) >= 0
            && ((PROJECTILE*)getData(currentNode))->y + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dy * ((PROJECTILE*)getData(currentNode))->projectileSpeed + (((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2) <= 127
            && ((PROJECTILE*)getData(currentNode))->y + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dy * ((PROJECTILE*)getData(currentNode))->projectileSpeed - (((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2) >= 0) {
            
            ((PROJECTILE*)getData(currentNode))->x += (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dx * ((PROJECTILE*)getData(currentNode))->projectileSpeed);
            ((PROJECTILE*)getData(currentNode))->y += (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dy * ((PROJECTILE*)getData(currentNode))->projectileSpeed);
            ((PROJECTILE*)getData(currentNode))->boundingBox->topLeft.x =  ((PROJECTILE*)getData(currentNode))->x - ( ((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2;
            ((PROJECTILE*)getData(currentNode))->boundingBox->topLeft.y =  ((PROJECTILE*)getData(currentNode))->y + ( ((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2;
            ((PROJECTILE*)getData(currentNode))->boundingBox->bottomRight.x =  ((PROJECTILE*)getData(currentNode))->x + ( ((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2;
            ((PROJECTILE*)getData(currentNode))->boundingBox->bottomRight.y =  ((PROJECTILE*)getData(currentNode))->y - ( ((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2;
        } else {
            LLNode* nextNode = currentNode->next;
            free(((PROJECTILE*)getData(currentNode))->boundingBox);
            deleteNode(enemyProjectileDLL, currentNode);
            // Move on to updating the next projectile if the current one has left the screen
            currentNode = nextNode;
            continue;
        }
        // Recalculate missile direction
        if (((PROJECTILE*)getData(currentNode))->projectileType == MISSILE) {
            ((PROJECTILE*)getData(currentNode))->projectileDirection->pdx = ((PROJECTILE*)getData(currentNode))->projectileDirection->dx;
            ((PROJECTILE*)getData(currentNode))->projectileDirection->pdy = ((PROJECTILE*)getData(currentNode))->projectileDirection->dy;
            double dx = getPlayer()->x - ((PROJECTILE*)getData(currentNode))->x;
            double dy = getPlayer()->y - ((PROJECTILE*)getData(currentNode))->y;
            if (dy > 0) {
                dy = -1;
            }
            // Normalize direction vector
            double length = sqrt(pow(dx, 2) + pow(dy, 2));
            dx = round(dx / length);
            dy = round(dy / length) - 1; // bias y direction down
            // Renormalize direction vector after bias
            length = sqrt(pow(dx, 2) + pow(dy, 2));
            dx = round(dx / length);
            dy = round(dy / length);
            if ((((PROJECTILE*)getData(currentNode))->projectileDirection->pdx < 0 && ((PROJECTILE*)getData(currentNode))->projectileDirection->dx > 0)
                || (((PROJECTILE*)getData(currentNode))->projectileDirection->pdx > 0 && ((PROJECTILE*)getData(currentNode))->projectileDirection->dx < 0)) {
                    ((PROJECTILE*)getData(currentNode))->projectileDirection->dx = 0;
            } else {
                ((PROJECTILE*)getData(currentNode))->projectileDirection->dx = dx;
            }
            ((PROJECTILE*)getData(currentNode))->projectileDirection->dy = dy;
        }
        drawEnemyProjectile(currentNode);
        currentNode = currentNode->next;
    }
}

void updatePlayerProjectiles(void) {
    if (playerProjectileDLL == NULL) {
        return;
    }
    LLNode* currentNode = getHead(playerProjectileDLL);
    while (currentNode) {
        erasePlayerProjectile(currentNode);
        bool projectileRemoved = false;
        /** Check for projectile and character collision
         */
        // Check for projectile and enemy collision
        if (getEnemyDLL()) {
            LLNode* currentEnemy = getHead(getEnemyDLL());
            while (currentEnemy) {
                // Only check for a projectile hit if the enemy is active
                if (((ENEMY*)getData(currentEnemy))->status != CHARACTER_STATUS::DEAD && projectileHit(((PROJECTILE*)getData(currentNode))->boundingBox, ((ENEMY*)getData(currentEnemy))->boundingBox)) {
                    // Reduce enemy health by projectile damage
                    ((BAR*)getData(((ENEMY*)getData(currentEnemy))->healthBar))->numHearts -= ((PROJECTILE*)getData(currentNode))->projectileType;
                    if (((BAR*)getData(((ENEMY*)getData(currentEnemy))->healthBar))->numHearts <= 0) {
                        explosionSound();
                        // If enemy destroyed, set display to Destroyed and add health amount to player score
                        ((BAR*)getData(((ENEMY*)getData(currentEnemy))->healthBar))->numHearts = 0;
                        ((ENEMY*)getData(currentEnemy))->status = CHARACTER_STATUS::DEAD;
                        ((ENEMY*)getData(currentEnemy))->enemyDisplay = CHARACTER_DISPLAY::DESTROYED;
                        getPlayer()->score += ((BAR*)getData(((ENEMY*)getData(currentEnemy))->healthBar))->barType;
                        getPlayer()->sessionKills++;
                        // If player is using super attack, add one to superChargeStatus only on kills
                        if (((PROJECTILE*)getData(currentNode))->projectileType == PROJECTILE_TYPE::LASER) {
                            if (getPlayer()->superChargeStatus < 5) {
                                getPlayer()->superChargeStatus++;
                            }
                        }
                    } else {
                        // If enemy damaged, set display to whatever animation state it was before plus Damaged
                        switch(((ENEMY*)getData(currentEnemy))->enemyDisplay + 4) { // adding 4 essentially switchs state to damaged form
                        case DEFAULTDAMAGED:
                            ((ENEMY*)getData(currentEnemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                            break;
                        case LEFTDAMAGED:
                            ((ENEMY*)getData(currentEnemy))->enemyDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                            break;
                        case RIGHTDAMAGED:
                            ((ENEMY*)getData(currentEnemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                            break;
                        case VERTICALDAMAGED:
                            ((ENEMY*)getData(currentEnemy))->enemyDisplay = CHARACTER_DISPLAY::VERTICALDAMAGED;
                            break;
                        }
                    }
                    // If player is using normal attack, add one to superChargeStatus and remove projectile on every hit
                    if (((PROJECTILE*)getData(currentNode))->projectileType != PROJECTILE_TYPE::LASER) {
                        if (getPlayer()->superChargeStatus < 5) {
                            getPlayer()->superChargeStatus++;
                        }
                        // Need a function to erase projectile from screen
                        LLNode* nextNode = currentNode->next;
                        free(((PROJECTILE*)getData(currentNode))->boundingBox);
                        deleteNode(playerProjectileDLL, currentNode);
                        currentNode = nextNode;
                        projectileRemoved = true;
                        break;
                    }
                }
                currentEnemy = currentEnemy->next;
            }
        }
        // Move on to updating the next projectile if the current one has collided with a character
        if (projectileRemoved) {
            continue;
        }
        // Check for projectile and boss collision
        if (getBoss()) {
            if (getBoss()->status != CHARACTER_STATUS::DEAD && projectileHit(((PROJECTILE*)getData(currentNode))->boundingBox, getBoss()->boundingBox)) {
                // Reduce enemy health by projectile damage
                ((BAR*)getData(getBoss()->healthBar))->numHearts -= ((PROJECTILE*)getData(currentNode))->projectileType;
                //printf("%d\n", ((BAR*)getData(getBoss()->healthBar))->numHearts);
                if (((BAR*)getData(getBoss()->healthBar))->numHearts <= 0) {
                    // If enemy destroyed, set display to Destroyed and add health amount to player score
                    ((BAR*)getData(getBoss()->healthBar))->numHearts = 0;
                    getBoss()->status = CHARACTER_STATUS::DEAD;
                    getBoss()->bossDisplay = CHARACTER_DISPLAY::DESTROYED;
                    getPlayer()->score += ((BAR*)getData(getBoss()->healthBar))->barType;
                    getPlayer()->sessionKills++;
                    // If player is using super attack, add one to superChargeStatus only on kills
                    if (((PROJECTILE*)getData(currentNode))->projectileType == PROJECTILE_TYPE::LASER) {
                        if (getPlayer()->superChargeStatus < 5) {
                            getPlayer()->superChargeStatus++;
                        }
                    }
                } else {
                    // If enemy damaged, set display to whatever animation state it was before plus Damaged
                    switch(getBoss()->bossDisplay + 4) { // adding 4 essentially switchs state to damaged form
                    case DEFAULTDAMAGED:
                        getBoss()->bossDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                        break;
                    case LEFTDAMAGED:
                        getBoss()->bossDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                        break;
                    case RIGHTDAMAGED:
                        getBoss()->bossDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                        break;
                    case VERTICALDAMAGED:
                        getBoss()->bossDisplay = CHARACTER_DISPLAY::VERTICALDAMAGED;
                        break;
                    }
                }
                // If player is using normal attack, add one to superChargeStatus and remove projectile on every hit
                if (((PROJECTILE*)getData(currentNode))->projectileType != PROJECTILE_TYPE::LASER) {
                    if (getPlayer()->superChargeStatus < 5) {
                        getPlayer()->superChargeStatus++;
                    }
                    LLNode* nextNode = currentNode->next;
                    free(((PROJECTILE*)getData(currentNode))->boundingBox);
                    deleteNode(playerProjectileDLL, currentNode);
                    currentNode = nextNode;
                    projectileRemoved = true;
                    break;
                }
            }
        }
        // Move on to updating the next projectile if the current one has collided with a character
        if (projectileRemoved) {
            continue;
        }

        /** Update projectile positions
         */
        if (((PROJECTILE*)getData(currentNode))->projectileType != PROJECTILE_TYPE::LASER) {
            // Update non laser projectiles
            // Make sure projectile is going to be within screen before updating pos
            // Need to account for projectile size
            if (((PROJECTILE*)getData(currentNode))->x + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dx * ((PROJECTILE*)getData(currentNode))->projectileSpeed + (((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2) <= 127
                && ((PROJECTILE*)getData(currentNode))->x + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dx * ((PROJECTILE*)getData(currentNode))->projectileSpeed - (((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2) >= 0
                && ((PROJECTILE*)getData(currentNode))->y + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dy * ((PROJECTILE*)getData(currentNode))->projectileSpeed + (((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2) <= 127
                && ((PROJECTILE*)getData(currentNode))->y + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dy * ((PROJECTILE*)getData(currentNode))->projectileSpeed - (((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2) >= 0) {
                
                ((PROJECTILE*)getData(currentNode))->x += (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dx * ((PROJECTILE*)getData(currentNode))->projectileSpeed);
                ((PROJECTILE*)getData(currentNode))->y += (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dy * ((PROJECTILE*)getData(currentNode))->projectileSpeed);
                ((PROJECTILE*)getData(currentNode))->boundingBox->topLeft.x =  ((PROJECTILE*)getData(currentNode))->x - ( ((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2;
                ((PROJECTILE*)getData(currentNode))->boundingBox->topLeft.y =  ((PROJECTILE*)getData(currentNode))->y + ( ((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2;
                ((PROJECTILE*)getData(currentNode))->boundingBox->bottomRight.x =  ((PROJECTILE*)getData(currentNode))->x + ( ((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2;
                ((PROJECTILE*)getData(currentNode))->boundingBox->bottomRight.y =  ((PROJECTILE*)getData(currentNode))->y - ( ((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2;
            } else {
                LLNode* nextNode = currentNode->next;
                free(((PROJECTILE*)getData(currentNode))->boundingBox);
                deleteNode(playerProjectileDLL, currentNode);
                // Move on to updating the next projectile if the current one has left the screen
                currentNode = nextNode;
                continue;
            }
        } else {
            // Update laser
            if (((PROJECTILE*)getData(currentNode))->projectileTick >= 60) { // one game tick is 0.05s so 60 ticks is 3s
                getPlayer()->superActive = false;
                LLNode* nextNode = currentNode->next;
                free(((PROJECTILE*)getData(currentNode))->boundingBox);
                deleteNode(playerProjectileDLL, currentNode);
                // Move on to updating the next projectile if laser attack has reached timed expiration
                currentNode = nextNode;
                continue;
            } else {
                ((PROJECTILE*)getData(currentNode))->projectileTick++;
                // Move the laser with the player
                ((PROJECTILE*)getData(currentNode))->x = getPlayer()->x;
                ((PROJECTILE*)getData(currentNode))->y = getPlayer()->y + 65;
                ((PROJECTILE*)getData(currentNode))->boundingBox->topLeft.x =  ((PROJECTILE*)getData(currentNode))->x - ( ((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2;
                ((PROJECTILE*)getData(currentNode))->boundingBox->topLeft.y =  ((PROJECTILE*)getData(currentNode))->y + ( ((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2;
                ((PROJECTILE*)getData(currentNode))->boundingBox->bottomRight.x =  ((PROJECTILE*)getData(currentNode))->x + ( ((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2;
                ((PROJECTILE*)getData(currentNode))->boundingBox->bottomRight.y =  ((PROJECTILE*)getData(currentNode))->y - ( ((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2;
            }
        }
        drawPlayerProjectile(currentNode);
        currentNode = currentNode->next;
    }
}

void updateBossProjectiles(void) {
    if (bossProjectileDLL == NULL) {
        return;
    }
    LLNode* currentNode = getHead(bossProjectileDLL);
    while (currentNode) {
        eraseBossProjectile(currentNode);
        bool projectileRemoved = false;
        // Check for projectile and player collision
        if (getPlayer()->playerStatus != CHARACTER_STATUS::DEAD && projectileHit(((PROJECTILE*)getData(currentNode))->boundingBox, getPlayer()->boundingBox)) {
            ((BAR*)getData(getPlayer()->healthBar))->numHearts -= ((PROJECTILE*)getData(currentNode))->projectileType;
            getPlayer()->lastDamaged = getPlayer()->tick;
            if (((BAR*)getData(getPlayer()->healthBar))->numHearts <= 0) {
                explosionSound();
                ((BAR*)getData(getPlayer()->healthBar))->numHearts = 0;
                getPlayer()->sessionDeaths++;
                getPlayer()->playerStatus = CHARACTER_STATUS::DEAD;
                getPlayer()->playerDisplay = CHARACTER_DISPLAY::DESTROYED;
            } else {
                // If enemy damaged, set display to whatever animation state it was before plus Damaged
                switch(getPlayer()->playerDisplay + 4) { // adding 4 essentially switchs state to damaged form
                case DEFAULTDAMAGED:
                    getPlayer()->playerDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                    break;
                case LEFTDAMAGED:
                    getPlayer()->playerDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                    break;
                case RIGHTDAMAGED:
                    getPlayer()->playerDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                    break;
                case VERTICALDAMAGED:
                    getPlayer()->playerDisplay = CHARACTER_DISPLAY::VERTICALDAMAGED;
                    break;
                }
            }
            LLNode* nextNode = currentNode->next;
            free(((PROJECTILE*)getData(currentNode))->boundingBox);
            deleteNode(bossProjectileDLL, currentNode);
            currentNode = nextNode;
            continue;
        }

        /** Update projectile positions
         */
        // Make sure projectile is going to be within screen before updating pos
        if (((PROJECTILE*)getData(currentNode))->x + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dx * ((PROJECTILE*)getData(currentNode))->projectileSpeed + (((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2) <= 127
            && ((PROJECTILE*)getData(currentNode))->x + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dx * ((PROJECTILE*)getData(currentNode))->projectileSpeed - (((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2) >= 0
            && ((PROJECTILE*)getData(currentNode))->y + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dy * ((PROJECTILE*)getData(currentNode))->projectileSpeed + (((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2) <= 127
            && ((PROJECTILE*)getData(currentNode))->y + (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dy * ((PROJECTILE*)getData(currentNode))->projectileSpeed - (((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2) >= 0) {
            
            ((PROJECTILE*)getData(currentNode))->x += (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dx * ((PROJECTILE*)getData(currentNode))->projectileSpeed);
            ((PROJECTILE*)getData(currentNode))->y += (int) round(((PROJECTILE*)getData(currentNode))->projectileDirection->dy * ((PROJECTILE*)getData(currentNode))->projectileSpeed);
            ((PROJECTILE*)getData(currentNode))->boundingBox->topLeft.x =  ((PROJECTILE*)getData(currentNode))->x - ( ((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2;
            ((PROJECTILE*)getData(currentNode))->boundingBox->topLeft.y =  ((PROJECTILE*)getData(currentNode))->y + ( ((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2;
            ((PROJECTILE*)getData(currentNode))->boundingBox->bottomRight.x =  ((PROJECTILE*)getData(currentNode))->x + ( ((PROJECTILE*)getData(currentNode))->projectileWidth - 1) / 2;
            ((PROJECTILE*)getData(currentNode))->boundingBox->bottomRight.y =  ((PROJECTILE*)getData(currentNode))->y - ( ((PROJECTILE*)getData(currentNode))->projectileHeight - 1) / 2;
        } else {
            LLNode* nextNode = currentNode->next;
            free(((PROJECTILE*)getData(currentNode))->boundingBox);
            deleteNode(bossProjectileDLL, currentNode);
            // Move on to updating the next projectile if the current one has left the screen
            currentNode = nextNode;
            continue;
        }
        // Recalculate missile direction
        if (((PROJECTILE*)getData(currentNode))->projectileType == MISSILE) {
            ((PROJECTILE*)getData(currentNode))->projectileDirection->pdx = ((PROJECTILE*)getData(currentNode))->projectileDirection->dx;
            ((PROJECTILE*)getData(currentNode))->projectileDirection->pdy = ((PROJECTILE*)getData(currentNode))->projectileDirection->dy;
            double dx = getPlayer()->x - ((PROJECTILE*)getData(currentNode))->x;
            double dy = getPlayer()->y - ((PROJECTILE*)getData(currentNode))->y;
            if (dy > 0) {
                dy = -1;
            }
            // Normalize direction vector
            double length = sqrt(pow(dx, 2) + pow(dy, 2));
            dx = round(dx / length);
            dy = round(dy / length) - 1; // bias y direction down
            // Renormalize direction vector after bias
            length = sqrt(pow(dx, 2) + pow(dy, 2));
            dx = round(dx / length);
            dy = round(dy / length);
            if ((((PROJECTILE*)getData(currentNode))->projectileDirection->pdx < 0 && ((PROJECTILE*)getData(currentNode))->projectileDirection->dx > 0)
                || (((PROJECTILE*)getData(currentNode))->projectileDirection->pdx > 0 && ((PROJECTILE*)getData(currentNode))->projectileDirection->dx < 0)) {
                    ((PROJECTILE*)getData(currentNode))->projectileDirection->dx = 0;
            } else {
                ((PROJECTILE*)getData(currentNode))->projectileDirection->dx = dx;
            }
            ((PROJECTILE*)getData(currentNode))->projectileDirection->dy = dy;
        }
        drawBossProjectile(currentNode);
        currentNode = currentNode->next;
    }
}

void deallocateEnemyProjectileDLL(void) {
    LLNode* currentNode = getHead(enemyProjectileDLL);
    while (currentNode) {
        LLNode* next_node = getNext(currentNode);
        free(((PROJECTILE*)getData(currentNode))->projectileDirection);
        free(((PROJECTILE*)getData(currentNode))->boundingBox);
        deleteNode(enemyProjectileDLL, currentNode);
        currentNode = next_node;
    }
    free(enemyProjectileDLL);
}

void deallocatePlayerProjectileDLL(void) {
    LLNode* currentNode = getHead(playerProjectileDLL);
    while (currentNode) {
        LLNode* next_node = getNext(currentNode);
        free(((PROJECTILE*)getData(currentNode))->projectileDirection);
        free(((PROJECTILE*)getData(currentNode))->boundingBox);
        deleteNode(playerProjectileDLL, currentNode);
        currentNode = next_node;
    }
    free(playerProjectileDLL);
}

void deallocateBossProjectileDLL(void) {
    LLNode* currentNode = getHead(bossProjectileDLL);
    while (currentNode) {
        LLNode* next_node = getNext(currentNode);
        free(((PROJECTILE*)getData(currentNode))->projectileDirection);
        free(((PROJECTILE*)getData(currentNode))->boundingBox);
        deleteNode(bossProjectileDLL, currentNode);
        currentNode = next_node;
    }
    free(bossProjectileDLL);
}

