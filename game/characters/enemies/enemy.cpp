#include "doublyLinkedList.h"
#include "gameMusic.h"
#include "hardware.h"
#include "mbed.h"
#include "healthBar.h"
#include "globals.h"
#include "graphics.h"
#include "enemy.h"
#include "player.h"
#include "projectile.h"

DLinkedList* enemyDLL = NULL;

DLinkedList* getEnemyDLL(void) {
    return enemyDLL;
}

void enemyInit(void) {
    enemyDLL = createDlinkedlist();
}

void enemyGenerator(CHARACTER_TYPE enemyType) {
    ENEMY* enemy = (ENEMY*)malloc(sizeof(ENEMY));
    insertHead(enemyDLL, enemy);
    LLNode* enemyNode = getHead(enemyDLL);
    // Generate a random starting x pos for enemy
    enemy->enemyType = enemyType;
    enemy->x = (randomNum() % (121 - 5 + 1)) + 5;
    enemy->y = 121;
    enemy->px = enemy->x;
    enemy->py = enemy->y;
    switch (enemyType) {
    case CHARACTER_TYPE::SHORT_RANGE_ENEMY:
        enemy->speed = SHORT_RANGE_ENEMY_SPEED;
        enemy->targetY = SHORT_RANGE_ENEMY_YPOS;
        enemy->fireInterval = 10;
        break;
    case CHARACTER_TYPE::LONG_RANGE_ENEMY:
        enemy->speed = LONG_RANGE_ENEMY_SPEED;
        enemy->targetY = LONG_RANGE_ENEMY_YPOS;
        enemy->fireInterval = 30;
        break;
    case CHARACTER_TYPE::MISSILE_ENEMY:
        enemy->speed = MISSILE_ENEMY_SPEED;
        enemy->targetY = MISSILE_ENEMY_YPOS;
        enemy->fireInterval = 40;
    }
    enemy->width = ENEMY_WIDTH;
    enemy->height = ENEMY_HEIGHT;
    BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    boundingBox->topLeft.x = enemy->x - (enemy->width - 1) / 2;
    boundingBox->topLeft.y = enemy->y + (enemy->height - 1) / 2;
    boundingBox->bottomRight.x = enemy->x + (enemy->width - 1) / 2;
    boundingBox->bottomRight.y = enemy->y - (enemy->height - 1) / 2;
    enemy->boundingBox = boundingBox;
    enemy->healthBar = createEnemyBar(enemyNode);
    enemy->entering = true;
    enemy->tick = randomNum() % 40; // Set enemy tick to a random number so that they don't fire in sync
    enemy->status = CHARACTER_STATUS::ACTIVE;
    enemy->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
    enemy->damagedTick = 0; // Lasts for 10 ticks
    enemy->destroyedTick = 0; // Lasts for 20 ticks
    drawEnemy(enemyNode);
}

void enemyUpdatePos(LLNode* enemy) {
    // If enemy is still entering, only move down until targetY pos is reached
    ((ENEMY*)getData(enemy))->px = ((ENEMY*)getData(enemy))->x;
    ((ENEMY*)getData(enemy))->py = ((ENEMY*)getData(enemy))->y;
    if (((ENEMY*)getData(enemy))->entering) {
        if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
            ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::VERTICALDAMAGED;
            ((ENEMY*)getData(enemy))->damagedTick++;
        } else {
            ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::VERTICAL;
            ((ENEMY*)getData(enemy))->damagedTick = 0;
        }
        if (((ENEMY*)getData(enemy))->y - ((ENEMY*)getData(enemy))->speed > ((ENEMY*)getData(enemy))->targetY) {
            ((ENEMY*)getData(enemy))->y -= ((ENEMY*)getData(enemy))->speed;
            ((ENEMY*)getData(enemy))->boundingBox->topLeft.x = ((ENEMY*)getData(enemy))->x - (((ENEMY*)getData(enemy))->width - 1) / 2;
            ((ENEMY*)getData(enemy))->boundingBox->topLeft.y = ((ENEMY*)getData(enemy))->y + (((ENEMY*)getData(enemy))->height - 1) / 2;
            ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x = ((ENEMY*)getData(enemy))->x + (((ENEMY*)getData(enemy))->width - 1) / 2;
            ((ENEMY*)getData(enemy))->boundingBox->bottomRight.y = ((ENEMY*)getData(enemy))->y - (((ENEMY*)getData(enemy))->height - 1) / 2;
        } else {
            ((ENEMY*)getData(enemy))->y = ((ENEMY*)getData(enemy))->targetY;
            ((ENEMY*)getData(enemy))->boundingBox->topLeft.x = ((ENEMY*)getData(enemy))->x - (((ENEMY*)getData(enemy))->width - 1) / 2;
            ((ENEMY*)getData(enemy))->boundingBox->topLeft.y = ((ENEMY*)getData(enemy))->y + (((ENEMY*)getData(enemy))->height - 1) / 2;
            ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x = ((ENEMY*)getData(enemy))->x + (((ENEMY*)getData(enemy))->width - 1) / 2;
            ((ENEMY*)getData(enemy))->boundingBox->bottomRight.y = ((ENEMY*)getData(enemy))->y - (((ENEMY*)getData(enemy))->height - 1) / 2;
            ((ENEMY*)getData(enemy))->entering = false;
        }
        drawEnemy(enemy);
        return;
    }
    // Once enemy has entered, move according to enemy type rules
    switch (((ENEMY*)getData(enemy))->enemyType) {
    case CHARACTER_TYPE::SHORT_RANGE_ENEMY:
        if (((ENEMY*)getData(enemy))->x + ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) <= 127 - (((ENEMY*)getData(enemy))->width - 1) / 2
            && ((ENEMY*)getData(enemy))->x + ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) >= 0 + (((ENEMY*)getData(enemy))->width - 1) / 2) {
            
            // Update enemyDisplay based on calculated movement direction
            if ((getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) > 0) {
                if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                    ((ENEMY*)getData(enemy))->damagedTick++;
                } else {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHT;
                    ((ENEMY*)getData(enemy))->damagedTick = 0;
                }
            } else if ((getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) < 0) {
                if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                    ((ENEMY*)getData(enemy))->damagedTick++;
                } else {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::LEFT;
                    ((ENEMY*)getData(enemy))->damagedTick = 0;
                }
            } else {
                if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                    ((ENEMY*)getData(enemy))->damagedTick++;
                } else {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
                    ((ENEMY*)getData(enemy))->damagedTick = 0;
                }
            }
            ((ENEMY*)getData(enemy))->x += ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x);
            ((ENEMY*)getData(enemy))->boundingBox->topLeft.x = ((ENEMY*)getData(enemy))->x - (((ENEMY*)getData(enemy))->width - 1) / 2;
            ((ENEMY*)getData(enemy))->boundingBox->topLeft.y = ((ENEMY*)getData(enemy))->y + (((ENEMY*)getData(enemy))->height - 1) / 2;
            ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x = ((ENEMY*)getData(enemy))->x + (((ENEMY*)getData(enemy))->width - 1) / 2;
            ((ENEMY*)getData(enemy))->boundingBox->bottomRight.y = ((ENEMY*)getData(enemy))->y - (((ENEMY*)getData(enemy))->height - 1) / 2;
        } else {
            // If calculated movement direction is invalid then display the enemy as not moved
            if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                ((ENEMY*)getData(enemy))->damagedTick++;
            } else {
                ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
                ((ENEMY*)getData(enemy))->damagedTick = 0;
            }
        }
        break;
    case CHARACTER_TYPE::LONG_RANGE_ENEMY:
        // Move toward player if more than 10 pixels away from player
        if (abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) > 10 || ((ENEMY*)getData(enemy))->tick % ((ENEMY*)getData(enemy))->fireInterval >= ((ENEMY*)getData(enemy))->fireInterval - 10) {
            if (((ENEMY*)getData(enemy))->x + ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) <= 127 - (((ENEMY*)getData(enemy))->width - 1) / 2
                && ((ENEMY*)getData(enemy))->x + ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) >= 0 + (((ENEMY*)getData(enemy))->width - 1) / 2) {
                
                // Update enemyDisplay based on calculated movement direction
                if ((getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) > 0) {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 20) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                } else if ((getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) < 0) {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 20) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::LEFT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                } else {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                }
                ((ENEMY*)getData(enemy))->x += ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x);
                ((ENEMY*)getData(enemy))->boundingBox->topLeft.x = ((ENEMY*)getData(enemy))->x - (((ENEMY*)getData(enemy))->width - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->topLeft.y = ((ENEMY*)getData(enemy))->y + (((ENEMY*)getData(enemy))->height - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x = ((ENEMY*)getData(enemy))->x + (((ENEMY*)getData(enemy))->width - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->bottomRight.y = ((ENEMY*)getData(enemy))->y - (((ENEMY*)getData(enemy))->height - 1) / 2;
            } else {
                // If calculated movement direction is invalid then display the enemy as not moved
                if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                    ((ENEMY*)getData(enemy))->damagedTick++;
                } else {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
                    ((ENEMY*)getData(enemy))->damagedTick = 0;
                }
            }
        } else {
            int nudge = ((ENEMY*)getData(enemy))->speed * ((getPlayer()->x == ((ENEMY*)getData(enemy))->x) ? ((randomNum() % 2 == 0) ? -1 : 1) : 0);
            if (nudge + ((ENEMY*)getData(enemy))->x - ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) <= 127 - (((ENEMY*)getData(enemy))->width - 1) / 2
                && nudge + ((ENEMY*)getData(enemy))->x - ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) >= 0  + (((ENEMY*)getData(enemy))->width - 1) / 2) {
                
                // Update enemyDisplay based on calculated movement direction
                if (nudge + (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) > 0) {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::LEFT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                } else if (nudge + (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) < 0) {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                } else {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                }
                ((ENEMY*)getData(enemy))->x -= ((ENEMY*)getData(enemy))->speed * (nudge + getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(nudge + getPlayer()->x - ((ENEMY*)getData(enemy))->x);
                ((ENEMY*)getData(enemy))->boundingBox->topLeft.x = nudge + ((ENEMY*)getData(enemy))->x - (((ENEMY*)getData(enemy))->width - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->topLeft.y = ((ENEMY*)getData(enemy))->y + (((ENEMY*)getData(enemy))->height - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x = nudge + ((ENEMY*)getData(enemy))->x + (((ENEMY*)getData(enemy))->width - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->bottomRight.y = ((ENEMY*)getData(enemy))->y - (((ENEMY*)getData(enemy))->height - 1) / 2;
            } else {
                // If calculated movement direction is invalid then display the enemy as not moved
                if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                    ((ENEMY*)getData(enemy))->damagedTick++;
                } else {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
                    ((ENEMY*)getData(enemy))->damagedTick = 0;
                }
            }
        }
        break;
    case CHARACTER_TYPE::MISSILE_ENEMY:
        if (abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) > 20) {
            if (((ENEMY*)getData(enemy))->x + ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) <= 127 - (((ENEMY*)getData(enemy))->width - 1) / 2
                && ((ENEMY*)getData(enemy))->x + ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) >= 0 + (((ENEMY*)getData(enemy))->width - 1) / 2) {
                
                // Update enemyDisplay based on calculated movement direction
                if ((getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) > 0) {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                } else if ((getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) < 0) {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::LEFT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                } else {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                }
                ((ENEMY*)getData(enemy))->x += ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x);
                ((ENEMY*)getData(enemy))->boundingBox->topLeft.x = ((ENEMY*)getData(enemy))->x - (((ENEMY*)getData(enemy))->width - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->topLeft.y = ((ENEMY*)getData(enemy))->y + (((ENEMY*)getData(enemy))->height - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x = ((ENEMY*)getData(enemy))->x + (((ENEMY*)getData(enemy))->width - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->bottomRight.y = ((ENEMY*)getData(enemy))->y - (((ENEMY*)getData(enemy))->height - 1) / 2;
            } else {
                // If calculated movement direction is invalid then display the enemy as not moved
                if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                    ((ENEMY*)getData(enemy))->damagedTick++;
                } else {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
                    ((ENEMY*)getData(enemy))->damagedTick = 0;
                }
            }
        } else {
            int nudge = ((ENEMY*)getData(enemy))->speed * ((getPlayer()->x == ((ENEMY*)getData(enemy))->x) ? ((randomNum() % 2 == 0) ? -1 : 1) : 0);
            if (nudge + ((ENEMY*)getData(enemy))->x - ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) <= 127 - (((ENEMY*)getData(enemy))->width - 1) / 2
                && nudge + ((ENEMY*)getData(enemy))->x - ((ENEMY*)getData(enemy))->speed * (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) >= 0  + (((ENEMY*)getData(enemy))->width - 1) / 2) {
                
                // Update enemyDisplay based on calculated movement direction
                if (nudge + (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) > 0) {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::LEFT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                } else if (nudge + (getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(getPlayer()->x - ((ENEMY*)getData(enemy))->x) < 0) {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::RIGHT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                } else {
                    if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                        ((ENEMY*)getData(enemy))->damagedTick++;
                    } else {
                        ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
                        ((ENEMY*)getData(enemy))->damagedTick = 0;
                    }
                }
                ((ENEMY*)getData(enemy))->x -= ((ENEMY*)getData(enemy))->speed * (nudge + getPlayer()->x - ((ENEMY*)getData(enemy))->x) / abs(nudge + getPlayer()->x - ((ENEMY*)getData(enemy))->x);
                ((ENEMY*)getData(enemy))->boundingBox->topLeft.x = nudge + ((ENEMY*)getData(enemy))->x - (((ENEMY*)getData(enemy))->width - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->topLeft.y = ((ENEMY*)getData(enemy))->y + (((ENEMY*)getData(enemy))->height - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x = nudge + ((ENEMY*)getData(enemy))->x + (((ENEMY*)getData(enemy))->width - 1) / 2;
                ((ENEMY*)getData(enemy))->boundingBox->bottomRight.y = ((ENEMY*)getData(enemy))->y - (((ENEMY*)getData(enemy))->height - 1) / 2;
            } else {
                // If calculated movement direction is invalid then display the enemy as not moved
                if (((ENEMY*)getData(enemy))->enemyDisplay >= 4 && ((ENEMY*)getData(enemy))->damagedTick < 5) {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
                    ((ENEMY*)getData(enemy))->damagedTick++;
                } else {
                    ((ENEMY*)getData(enemy))->enemyDisplay = CHARACTER_DISPLAY::DEFAULT;
                    ((ENEMY*)getData(enemy))->damagedTick = 0;
                }
            }
        }
    }
    drawEnemy(enemy);
}

void enemiesUpdate(void) {
    if (enemyDLL == NULL) {
        return;
    }
    LLNode* currentNode = getHead(enemyDLL);
    while (currentNode) {
        // Check if the enemy has been destroyed
        if (((ENEMY*)getData(currentNode))->status == CHARACTER_STATUS::DEAD) {
            if (((ENEMY*)getData(currentNode))->destroyedTick < 20) {
                ((ENEMY*)getData(currentNode))->enemyDisplay = CHARACTER_DISPLAY::DESTROYED;
                ((ENEMY*)getData(currentNode))->destroyedTick++;
                drawEnemy(currentNode);
                currentNode = currentNode->next;
                continue;
            } else {
                eraseEnemy(currentNode);
                LLNode* nextNode = currentNode->next;
                free(((ENEMY*)getData(currentNode))->boundingBox);
                deleteNode(enemyDLL, currentNode);
                currentNode = nextNode;
                continue;
            }
        }
        // If not destroyed, then update position and try to fire projectile
        enemyUpdatePos(currentNode);
        if (!((ENEMY*)getData(currentNode))->entering && ((ENEMY*)getData(currentNode))->tick % ((ENEMY*)getData(currentNode))->fireInterval == 0) {
            if (randomNum() & 1) {
                if (((ENEMY*)getData(currentNode))->enemyType == CHARACTER_TYPE::MISSILE_ENEMY) {
                    missileSound();
                } else {
                    projectileSound();
                }
                enemyFireProjectile(currentNode);
            }
        }
        ((ENEMY*)getData(currentNode))->tick++;
        currentNode = currentNode->next;
    }
}

void enemyFireProjectile(LLNode* enemy) {
    generateEnemyProjectile(enemy);
}

void deallocateEnemies(void) {
    deallocateEnemyHealthBars();
    if (enemyDLL == NULL) {
        return;
    }
    LLNode* currentNode = getHead(enemyDLL);
    while (currentNode) {
        LLNode* next_node = getNext(currentNode);
        free(((ENEMY*)getData(currentNode))->boundingBox);
        deleteNode(enemyDLL, currentNode);
        currentNode = next_node;
    }
    free(enemyDLL);
}