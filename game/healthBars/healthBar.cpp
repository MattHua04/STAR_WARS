#include "graphics.hpp"
#include "healthBar.hpp"
#include "enemy.hpp"
#include "opponent.hpp"
#include "player.hpp"
#include "finalBoss.hpp"
#include "doublyLinkedList.hpp"
#include <cstdio>

DLinkedList* enemyHealthBarDLL = NULL;
LLNode* playerHealthBarNode = NULL;
LLNode* opponentHealthBarNode = NULL;
LLNode* bossHealthBarNode = NULL;

void healthBarInit(void) {
    enemyHealthBarDLL = createDlinkedlist();
}

void updateBars(void) {
    // Update enemy bars
    LLNode* currentNode = getHead(enemyHealthBarDLL);
    while (currentNode) {
        //eraseHealthBar(currentNode);
        ((BAR*)getData(currentNode))->px = ((BAR*)getData(currentNode))->x;
        ((BAR*)getData(currentNode))->py = ((BAR*)getData(currentNode))->y;
        ((BAR*)getData(currentNode))->x = ((ENEMY*)getData(((BAR*)getData(currentNode))->barOwner))->x;
        ((BAR*)getData(currentNode))->y = ((ENEMY*)getData(((BAR*)getData(currentNode))->barOwner))->y + 9;
        ((BAR*)getData(currentNode))->boundingBox->topLeft.x = ((BAR*)getData(currentNode))->x - (BAR_WIDTH - 1) / 2;
        ((BAR*)getData(currentNode))->boundingBox->topLeft.y = ((BAR*)getData(currentNode))->y + (BAR_HEIGHT - 1) / 2;
        ((BAR*)getData(currentNode))->boundingBox->bottomRight.x = ((BAR*)getData(currentNode))->x + (BAR_WIDTH - 1) / 2;
        ((BAR*)getData(currentNode))->boundingBox->bottomRight.y = ((BAR*)getData(currentNode))->y - (BAR_HEIGHT - 1) / 2;
        drawHealthBar(currentNode);
        if (((BAR*)getData(currentNode))->numHearts <= 0) {
            LLNode* nextNode = currentNode->next;
            deleteNode(enemyHealthBarDLL, currentNode);
            currentNode = nextNode;
            continue;
        }
        currentNode = currentNode->next;
    }
    // Update player bar
    if (playerHealthBarNode) {
        //eraseHealthBar(playerHealthBarNode);
        ((BAR*)getData(playerHealthBarNode))->px = ((BAR*)getData(playerHealthBarNode))->x;
        ((BAR*)getData(playerHealthBarNode))->py = ((BAR*)getData(playerHealthBarNode))->y;
        ((BAR*)getData(playerHealthBarNode))->x = getPlayer()->x;
        ((BAR*)getData(playerHealthBarNode))->y = getPlayer()->y + 9;
        ((BAR*)getData(playerHealthBarNode))->boundingBox->topLeft.x = ((BAR*)getData(playerHealthBarNode))->x - (BAR_WIDTH - 1) / 2;
        ((BAR*)getData(playerHealthBarNode))->boundingBox->topLeft.y = ((BAR*)getData(playerHealthBarNode))->y + (BAR_HEIGHT - 1) / 2;
        ((BAR*)getData(playerHealthBarNode))->boundingBox->bottomRight.x = ((BAR*)getData(playerHealthBarNode))->x + (BAR_WIDTH - 1) / 2;
        ((BAR*)getData(playerHealthBarNode))->boundingBox->bottomRight.y = ((BAR*)getData(playerHealthBarNode))->y - (BAR_HEIGHT - 1) / 2;
        drawHealthBar(playerHealthBarNode);
        if (((BAR*)getData(playerHealthBarNode))->numHearts <= 0) {
            deletePlayerHealthBar();
            playerHealthBarNode = NULL;
        }
    }
    // Update opponent player bar
    if (opponentHealthBarNode) {
        //eraseHealthBar(opponentHealthBarNode);
        ((BAR*)getData(opponentHealthBarNode))->px = ((BAR*)getData(opponentHealthBarNode))->x;
        ((BAR*)getData(opponentHealthBarNode))->py = ((BAR*)getData(opponentHealthBarNode))->y;
        ((BAR*)getData(opponentHealthBarNode))->x = getOpponent()->x;
        ((BAR*)getData(opponentHealthBarNode))->y = getOpponent()->y + 9;
        ((BAR*)getData(opponentHealthBarNode))->boundingBox->topLeft.x = ((BAR*)getData(opponentHealthBarNode))->x - (BAR_WIDTH - 1) / 2;
        ((BAR*)getData(opponentHealthBarNode))->boundingBox->topLeft.y = ((BAR*)getData(opponentHealthBarNode))->y + (BAR_HEIGHT - 1) / 2;
        ((BAR*)getData(opponentHealthBarNode))->boundingBox->bottomRight.x = ((BAR*)getData(opponentHealthBarNode))->x + (BAR_WIDTH - 1) / 2;
        ((BAR*)getData(opponentHealthBarNode))->boundingBox->bottomRight.y = ((BAR*)getData(opponentHealthBarNode))->y - (BAR_HEIGHT - 1) / 2;
        drawHealthBar(opponentHealthBarNode);
        if (((BAR*)getData(opponentHealthBarNode))->numHearts <= 0) {
            deleteOpponentHealthBar();
            opponentHealthBarNode = NULL;
        }
    }
    // Update boss bar
    if (bossHealthBarNode) {
        //eraseHealthBar(bossHealthBarNode);
        ((BAR*)getData(bossHealthBarNode))->px = ((BAR*)getData(bossHealthBarNode))->x;
        ((BAR*)getData(bossHealthBarNode))->py = ((BAR*)getData(bossHealthBarNode))->y;
        ((BAR*)getData(bossHealthBarNode))->x = getBoss()->x;
        ((BAR*)getData(bossHealthBarNode))->y = getBoss()->y + 15;
        ((BAR*)getData(bossHealthBarNode))->boundingBox->topLeft.x = ((BAR*)getData(bossHealthBarNode))->x - (BAR_WIDTH - 1) / 2;
        ((BAR*)getData(bossHealthBarNode))->boundingBox->topLeft.y = ((BAR*)getData(bossHealthBarNode))->y + (BAR_HEIGHT - 1) / 2;
        ((BAR*)getData(bossHealthBarNode))->boundingBox->bottomRight.x = ((BAR*)getData(bossHealthBarNode))->x + (BAR_WIDTH - 1) / 2;
        ((BAR*)getData(bossHealthBarNode))->boundingBox->bottomRight.y = ((BAR*)getData(bossHealthBarNode))->y - (BAR_HEIGHT - 1) / 2;
        drawHealthBar(bossHealthBarNode);
        if (((BAR*)getData(bossHealthBarNode))->numHearts <= 0) {
            deleteBossHealthBar();
            bossHealthBarNode = NULL;
        }
    }
}

LLNode* createPlayerBar(LLNode* character) {
    BAR* playerHealthBar = (BAR*)malloc(sizeof(BAR));
    playerHealthBarNode = createLlnode(playerHealthBar);
    playerHealthBar->barOwner = character;
    playerHealthBar->x = getPlayer()->x;
    playerHealthBar->y = getPlayer()->y + 9;
    playerHealthBar->px = getPlayer()->x;
    playerHealthBar->py = getPlayer()->y + 9;
    playerHealthBar->barType = THREE_HEARTS;
    playerHealthBar->numHearts = playerHealthBar->barType;
    BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    boundingBox->topLeft.x = playerHealthBar->x - (BAR_WIDTH - 1) / 2;
    boundingBox->topLeft.y = playerHealthBar->y + (BAR_HEIGHT - 1) / 2;
    boundingBox->bottomRight.x = playerHealthBar->x + (BAR_WIDTH - 1) / 2;
    boundingBox->bottomRight.y = playerHealthBar->y - (BAR_HEIGHT - 1) / 2;
    playerHealthBar->boundingBox = boundingBox;
    return playerHealthBarNode;
}

LLNode* createOpponentBar(LLNode* character) {
    BAR* opponentHealthBar = (BAR*)malloc(sizeof(BAR));
    opponentHealthBarNode = createLlnode(opponentHealthBar);
    opponentHealthBar->barOwner = character;
    opponentHealthBar->x = getOpponent()->x;
    opponentHealthBar->y = getOpponent()->y + 9;
    opponentHealthBar->px = getOpponent()->x;
    opponentHealthBar->py = getOpponent()->y + 9;
    opponentHealthBar->barType = THREE_HEARTS;
    opponentHealthBar->numHearts = opponentHealthBar->barType;
    BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    boundingBox->topLeft.x = opponentHealthBar->x - (BAR_WIDTH - 1) / 2;
    boundingBox->topLeft.y = opponentHealthBar->y + (BAR_HEIGHT - 1) / 2;
    boundingBox->bottomRight.x = opponentHealthBar->x + (BAR_WIDTH - 1) / 2;
    boundingBox->bottomRight.y = opponentHealthBar->y - (BAR_HEIGHT - 1) / 2;
    opponentHealthBar->boundingBox = boundingBox;
    return opponentHealthBarNode;
}

LLNode* createEnemyBar(LLNode* character) {
    BAR* healthBar = (BAR*)malloc(sizeof(BAR));
    insertHead(enemyHealthBarDLL, healthBar);
    LLNode* healthBarNode = getHead(enemyHealthBarDLL);
    healthBar->barOwner = character;
    healthBar->x = ((ENEMY*)getData(character))->x;
    healthBar->y = ((ENEMY*)getData(character))->y + 9;
    healthBar->px = ((ENEMY*)getData(character))->x;
    healthBar->py = ((ENEMY*)getData(character))->y + 9;
    switch (((ENEMY*)getData(character))->enemyType)
    {
    case CHARACTER_TYPE::SHORT_RANGE_ENEMY:
        healthBar->barType = TWO_HEARTS;
        break;
    case CHARACTER_TYPE::LONG_RANGE_ENEMY:
        healthBar->barType = TWO_HEARTS;
        break;
    case CHARACTER_TYPE::MISSILE_ENEMY:
        healthBar->barType = THREE_HEARTS;
        break;
    }
    healthBar->numHearts = healthBar->barType;
    BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    boundingBox->topLeft.x = healthBar->x - (BAR_WIDTH - 1) / 2;
    boundingBox->topLeft.y = healthBar->y + (BAR_HEIGHT - 1) / 2;
    boundingBox->bottomRight.x = healthBar->x + (BAR_WIDTH - 1) / 2;
    boundingBox->bottomRight.y = healthBar->y - (BAR_HEIGHT - 1) / 2;
    healthBar->boundingBox = boundingBox;
    return healthBarNode;
}

LLNode* createBossBar(LLNode* character) {
    BAR* bossHealthBar = (BAR*)malloc(sizeof(BAR));
    bossHealthBarNode = createLlnode(bossHealthBar);
    bossHealthBar->barOwner = character;
    bossHealthBar->x = ((BOSS*)getData(character))->x;
    bossHealthBar->y = ((BOSS*)getData(character))->y + 15;
    bossHealthBar->px = ((BOSS*)getData(character))->x;
    bossHealthBar->py = ((BOSS*)getData(character))->y + 15;
    bossHealthBar->barType = TEN_HEARTS;
    bossHealthBar->numHearts = bossHealthBar->barType;
    BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    boundingBox->topLeft.x = bossHealthBar->x - (BAR_WIDTH - 1) / 2;
    boundingBox->topLeft.y = bossHealthBar->y + (BAR_HEIGHT - 1) / 2;
    boundingBox->bottomRight.x = bossHealthBar->x + (BAR_WIDTH - 1) / 2;
    boundingBox->bottomRight.y = bossHealthBar->y - (BAR_HEIGHT - 1) / 2;
    bossHealthBar->boundingBox = boundingBox;
    return bossHealthBarNode;
}

void deallocateEnemyHealthBars(void) {
    if (enemyHealthBarDLL == NULL) {
        return;
    }
    LLNode* currentNode = getHead(enemyHealthBarDLL);
    while (currentNode) {
        LLNode* next_node = getNext(currentNode);
        deleteNode(enemyHealthBarDLL, currentNode);
        currentNode = next_node;
    }
    free(enemyHealthBarDLL);
    enemyHealthBarDLL = NULL;
}

void deletePlayerHealthBar(void) {
    if (playerHealthBarNode == NULL) {
        return;
    }
    free(playerHealthBarNode);
    playerHealthBarNode = NULL;
}

void deleteOpponentHealthBar(void) {
    if (opponentHealthBarNode == NULL) {
        return;
    }
    free(opponentHealthBarNode);
    opponentHealthBarNode = NULL;
}

void deleteBossHealthBar(void) {
    if (bossHealthBarNode == NULL) {
        return;
    }
    free(bossHealthBarNode);
    bossHealthBarNode = NULL;
}