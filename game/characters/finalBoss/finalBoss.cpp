#include "enemy.hpp"
#include "finalBoss.hpp"
#include "globals.hpp"
#include "graphics.hpp"
#include "projectile.hpp"
#include "player.hpp"

BOSS* boss = NULL;
LLNode* bossNode = NULL;

BOSS* getBoss(void) {
    return boss;
}

void bossInit(void) {
    boss = (BOSS*)malloc(sizeof(BOSS));
    bossNode = createLlnode(boss);
    ((BOSS*)getData(bossNode))->enemyType = CHARACTER_TYPE::BOSS_CHARACTER;
    ((BOSS*)getData(bossNode))->x = BOSS_XPOS;
    ((BOSS*)getData(bossNode))->y = 118;
    ((BOSS*)getData(bossNode))->px = BOSS_XPOS;
    ((BOSS*)getData(bossNode))->py = 118;
    ((BOSS*)getData(bossNode))->speed = BOSS_SPEED;
    ((BOSS*)getData(bossNode))->targetY = BOSS_YPOS;
    ((BOSS*)getData(bossNode))->fireInterval = 20;
    ((BOSS*)getData(bossNode))->specialAttackInterval = 100;
    ((BOSS*)getData(bossNode))->specialAttacking = false;
    ((BOSS*)getData(bossNode))->width = BOSS_WIDTH;
    ((BOSS*)getData(bossNode))->height = BOSS_HEIGHT;
    BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    boundingBox->topLeft.x = ((BOSS*)getData(bossNode))->x - (((BOSS*)getData(bossNode))->width - 1) / 2;
    boundingBox->topLeft.y = ((BOSS*)getData(bossNode))->y + (((BOSS*)getData(bossNode))->height - 1) / 2;
    boundingBox->bottomRight.x = ((BOSS*)getData(bossNode))->x + (((BOSS*)getData(bossNode))->width - 1) / 2;
    boundingBox->bottomRight.y = ((BOSS*)getData(bossNode))->y - (((BOSS*)getData(bossNode))->height - 1) / 2;
    ((BOSS*)getData(bossNode))->boundingBox = boundingBox;
    ((BOSS*)getData(bossNode))->healthBar = createBossBar(bossNode);
    ((BOSS*)getData(bossNode))->entering = true;
    ((BOSS*)getData(bossNode))->tick = 1;
    ((BOSS*)getData(bossNode))->status = CHARACTER_STATUS::ACTIVE;
    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::DEFAULT;
    ((BOSS*)getData(bossNode))->damagedTick = 0; // Lasts for 5 ticks
    ((BOSS*)getData(bossNode))->destroyedTick = 0; // Lasts for 20 ticks
    drawBoss(bossNode);
}

void bossUpdate(void) {
    if (bossNode == NULL) {
        return;
    }
    ((BOSS*)getData(bossNode))->px = ((BOSS*)getData(bossNode))->x;
    ((BOSS*)getData(bossNode))->py = ((BOSS*)getData(bossNode))->y;
    if (((BOSS*)getData(bossNode))->status == CHARACTER_STATUS::DEAD) {
        if (((BOSS*)getData(bossNode))->destroyedTick < 20) {
            ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::DESTROYED;
            ((BOSS*)getData(bossNode))->destroyedTick++;
            drawBoss(bossNode);
            return;
        } else {
            eraseBoss(bossNode);
            deleteBoss();
            return;
        }
    }
    if (((BOSS*)getData(bossNode))->entering) {
        if (((BOSS*)getData(bossNode))->bossDisplay >= 4 && ((BOSS*)getData(bossNode))->damagedTick < 5) {
            ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::VERTICALDAMAGED;
            ((BOSS*)getData(bossNode))->damagedTick++;
        } else {
            ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::VERTICAL;
            ((BOSS*)getData(bossNode))->damagedTick = 0;
        }
        if (((BOSS*)getData(bossNode))->y - ((BOSS*)getData(bossNode))->speed > ((BOSS*)getData(bossNode))->targetY) {
            ((BOSS*)getData(bossNode))->y -= ((BOSS*)getData(bossNode))->speed;
            ((BOSS*)getData(bossNode))->boundingBox->topLeft.x = ((BOSS*)getData(bossNode))->x - (((BOSS*)getData(bossNode))->width - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->topLeft.y = ((BOSS*)getData(bossNode))->y + (((BOSS*)getData(bossNode))->height - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->bottomRight.x = ((BOSS*)getData(bossNode))->x + (((BOSS*)getData(bossNode))->width - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->bottomRight.y = ((BOSS*)getData(bossNode))->y - (((BOSS*)getData(bossNode))->height - 1) / 2;
        } else {
            ((BOSS*)getData(bossNode))->y = ((BOSS*)getData(bossNode))->targetY;
            ((BOSS*)getData(bossNode))->boundingBox->topLeft.x = ((BOSS*)getData(bossNode))->x - (((BOSS*)getData(bossNode))->width - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->topLeft.y = ((BOSS*)getData(bossNode))->y + (((BOSS*)getData(bossNode))->height - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->bottomRight.x = ((BOSS*)getData(bossNode))->x + (((BOSS*)getData(bossNode))->width - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->bottomRight.y = ((BOSS*)getData(bossNode))->y - (((BOSS*)getData(bossNode))->height - 1) / 2;
            ((BOSS*)getData(bossNode))->entering = false;
        }
        drawBoss(bossNode);
        return;
    }
    if (getPlayer()->x != ((BOSS*)getData(bossNode))->x && (abs(getPlayer()->x - ((BOSS*)getData(bossNode))->x) > 20  || ((BOSS*)getData(bossNode))->tick % ((BOSS*)getData(bossNode))->fireInterval >= ((BOSS*)getData(bossNode))->fireInterval - 10)) {
        if (((BOSS*)getData(bossNode))->boundingBox->bottomRight.x + ((BOSS*)getData(bossNode))->speed * (getPlayer()->x - ((BOSS*)getData(bossNode))->x) / abs(getPlayer()->x - ((BOSS*)getData(bossNode))->x) <= 127 
            && ((BOSS*)getData(bossNode))->boundingBox->topLeft.x + ((BOSS*)getData(bossNode))->speed * (getPlayer()->x - ((BOSS*)getData(bossNode))->x) / abs(getPlayer()->x - ((BOSS*)getData(bossNode))->x) >= 0) {
            
            // Update enemyDisplay based on calculated movement direction
            if ((getPlayer()->x - ((BOSS*)getData(bossNode))->x) / abs(getPlayer()->x - ((BOSS*)getData(bossNode))->x) > 0) {
                if (((BOSS*)getData(bossNode))->bossDisplay >= 4 && ((BOSS*)getData(bossNode))->damagedTick < 5) {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                    ((BOSS*)getData(bossNode))->damagedTick++;
                } else {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::RIGHT;
                    ((BOSS*)getData(bossNode))->damagedTick = 0;
                }
            } else if ((getPlayer()->x - ((BOSS*)getData(bossNode))->x) / abs(getPlayer()->x - ((BOSS*)getData(bossNode))->x) < 0) {
                if (((BOSS*)getData(bossNode))->bossDisplay >= 4 && ((BOSS*)getData(bossNode))->damagedTick < 5) {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                    ((BOSS*)getData(bossNode))->damagedTick++;
                } else {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::LEFT;
                    ((BOSS*)getData(bossNode))->damagedTick = 0;
                }
            } else {
                if (((BOSS*)getData(bossNode))->bossDisplay >= 4 && ((BOSS*)getData(bossNode))->damagedTick < 5) {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                    ((BOSS*)getData(bossNode))->damagedTick++;
                } else {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::DEFAULT;
                    ((BOSS*)getData(bossNode))->damagedTick = 0;
                }
            }
            ((BOSS*)getData(bossNode))->x += ((BOSS*)getData(bossNode))->speed * (getPlayer()->x - ((BOSS*)getData(bossNode))->x) / abs(getPlayer()->x - ((BOSS*)getData(bossNode))->x);
            ((BOSS*)getData(bossNode))->boundingBox->topLeft.x = ((BOSS*)getData(bossNode))->x - (((BOSS*)getData(bossNode))->width - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->topLeft.y = ((BOSS*)getData(bossNode))->y + (((BOSS*)getData(bossNode))->height - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->bottomRight.x = ((BOSS*)getData(bossNode))->x + (((BOSS*)getData(bossNode))->width - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->bottomRight.y = ((BOSS*)getData(bossNode))->y - (((BOSS*)getData(bossNode))->height - 1) / 2;
        } else {
            // If calculated movement direction is invalid then display the enemy as not moved
            if (((BOSS*)getData(bossNode))->bossDisplay >= 4 && ((BOSS*)getData(bossNode))->damagedTick < 5) {
                ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                ((BOSS*)getData(bossNode))->damagedTick++;
            } else {
                ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::DEFAULT;
                ((BOSS*)getData(bossNode))->damagedTick = 0;
            }
        }
    } else {
        int nudge = ((BOSS*)getData(bossNode))->speed * ((getPlayer()->x == ((BOSS*)getData(bossNode))->x) ? ((randomNum() % 2 == 0) ? -1 : 1) : 0);
        if (nudge + ((BOSS*)getData(bossNode))->boundingBox->bottomRight.x - ((BOSS*)getData(bossNode))->speed * (getPlayer()->x - ((BOSS*)getData(bossNode))->x) / abs(getPlayer()->x - ((BOSS*)getData(bossNode))->x) <= 127
            && nudge + ((BOSS*)getData(bossNode))->boundingBox->topLeft.x - ((BOSS*)getData(bossNode))->speed * (getPlayer()->x - ((BOSS*)getData(bossNode))->x) / abs(getPlayer()->x - ((BOSS*)getData(bossNode))->x) >= 0) {
            
            // Update bossDisplay based on calculated movement direction
            if (nudge +(getPlayer()->x - ((BOSS*)getData(bossNode))->x) / abs(getPlayer()->x - ((BOSS*)getData(bossNode))->x) < 0) {
                if (((BOSS*)getData(bossNode))->bossDisplay >= 4 && ((BOSS*)getData(bossNode))->damagedTick < 5) {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                    ((BOSS*)getData(bossNode))->damagedTick++;
                } else {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::RIGHT;
                    ((BOSS*)getData(bossNode))->damagedTick = 0;
                }
            } else if (nudge + (getPlayer()->x - ((BOSS*)getData(bossNode))->x) / abs(getPlayer()->x - ((BOSS*)getData(bossNode))->x) > 0) {
                if (((BOSS*)getData(bossNode))->bossDisplay >= 4 && ((BOSS*)getData(bossNode))->damagedTick < 5) {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                    ((BOSS*)getData(bossNode))->damagedTick++;
                } else {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::LEFT;
                    ((BOSS*)getData(bossNode))->damagedTick = 0;
                }
            } else {
                if (((BOSS*)getData(bossNode))->bossDisplay >= 4 && ((BOSS*)getData(bossNode))->damagedTick < 5) {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                    ((BOSS*)getData(bossNode))->damagedTick++;
                } else {
                    ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::DEFAULT;
                    ((BOSS*)getData(bossNode))->damagedTick = 0;
                }
            }
            ((BOSS*)getData(bossNode))->x -= ((BOSS*)getData(bossNode))->speed * (nudge + getPlayer()->x - ((BOSS*)getData(bossNode))->x) / abs(nudge + getPlayer()->x - ((BOSS*)getData(bossNode))->x);
            ((BOSS*)getData(bossNode))->boundingBox->topLeft.x = nudge + ((BOSS*)getData(bossNode))->x - (((BOSS*)getData(bossNode))->width - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->topLeft.y = ((BOSS*)getData(bossNode))->y + (((BOSS*)getData(bossNode))->height - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->bottomRight.x = nudge + ((BOSS*)getData(bossNode))->x + (((BOSS*)getData(bossNode))->width - 1) / 2;
            ((BOSS*)getData(bossNode))->boundingBox->bottomRight.y = ((BOSS*)getData(bossNode))->y - (((BOSS*)getData(bossNode))->height - 1) / 2;
        } else {
            // If calculated movement direction is invalid then display the BOSS as not moved
            if (((BOSS*)getData(bossNode))->bossDisplay >= 4 && ((BOSS*)getData(bossNode))->damagedTick < 5) {
                ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                ((BOSS*)getData(bossNode))->damagedTick++;
            } else {
                ((BOSS*)getData(bossNode))->bossDisplay = CHARACTER_DISPLAY::DEFAULT;
                ((BOSS*)getData(bossNode))->damagedTick = 0;
            }
        }
    }
    // Check to see if boss can shoot
    if (((BOSS*)getData(bossNode))->tick % ((BOSS*)getData(bossNode))->specialAttackInterval == 0) {
        if (randomNum() & 1) {
            ((BOSS*)getData(bossNode))->specialAttacking = true;
            missileSound();
            bossSpecialAttack(bossNode);
        }
    } else if (((BOSS*)getData(bossNode))->tick % ((BOSS*)getData(bossNode))->fireInterval == 0) {
        if (randomNum() & 1) {
            projectileSound();
            bossFireProjectile(bossNode);
        }
    }
    ((BOSS*)getData(bossNode))->tick++;
    drawBoss(bossNode);
}

void bossFireProjectile(LLNode* boss) {
    generateBossProjectile(bossNode);
}

void bossSpecialAttack(LLNode* boss) {
    generateBossProjectile(bossNode);
    ((BOSS*)getData(boss))->specialAttacking = false;
}

void deleteBoss(void) {
    if (bossNode == NULL) {
        return;
    }
    free(((PLAYER*)getData(bossNode))->boundingBox);
    free(bossNode);
    bossNode = NULL;
    boss = NULL;
}