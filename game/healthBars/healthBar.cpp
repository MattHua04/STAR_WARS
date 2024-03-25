#include "graphics.h"
#include "healthBar.h"
#include "enemy.h"
#include "player.h"
#include "finalBoss.h"
#include "doublyLinkedList.h"
#include <cstdio>

DLinkedList* enemyHealthBarDLL = NULL;
LLNode* playerHealthBarNode = NULL;
LLNode* bossHealthBarNode = NULL;

void healthBarInit(void) {
    enemyHealthBarDLL = createDlinkedlist();
}

void updateBars(void) {
    // Update enemy bars
    LLNode* currentNode = getHead(enemyHealthBarDLL);
    while (currentNode) {
        eraseHealthBar(currentNode);
        ((BAR*)getData(currentNode))->x = ((ENEMY*)getData(((BAR*)getData(currentNode))->barOwner))->x;
        ((BAR*)getData(currentNode))->y = ((ENEMY*)getData(((BAR*)getData(currentNode))->barOwner))->y + 9; // need to figure out offset amount
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
        eraseHealthBar(playerHealthBarNode); 
        ((BAR*)getData(playerHealthBarNode))->x = getPlayer()->x;
        ((BAR*)getData(playerHealthBarNode))->y = getPlayer()->y + 9; // need to figure out offset amount
        drawHealthBar(playerHealthBarNode);
        if (((BAR*)getData(playerHealthBarNode))->numHearts <= 0) {
            free(playerHealthBarNode);
            playerHealthBarNode = NULL;
        }
    }
    // Update boss bar
    if (bossHealthBarNode) {
        eraseHealthBar(bossHealthBarNode);
        ((BAR*)getData(bossHealthBarNode))->x = getBoss()->x;
        ((BAR*)getData(bossHealthBarNode))->y = getBoss()->y + 15; // need to figure out offset amount
        drawHealthBar(bossHealthBarNode);
        if (((BAR*)getData(bossHealthBarNode))->numHearts <= 0) {
            deleteBossHealthBar();
        }
    }
}

LLNode* createPlayerBar(LLNode* character) {
    BAR* playerHealthBar = (BAR*)malloc(sizeof(BAR));
    playerHealthBarNode = createLlnode(playerHealthBar);
    playerHealthBar->barOwner = character;
    playerHealthBar->x = getPlayer()->x;
    playerHealthBar->y = getPlayer()->y + 9; // need to figure out offset amount
    playerHealthBar->barType = THREE_HEARTS;
    playerHealthBar->numHearts = playerHealthBar->barType;
    return playerHealthBarNode;
}

LLNode* createEnemyBar(LLNode* character) {
    BAR* healthBar = (BAR*)malloc(sizeof(BAR));
    insertHead(enemyHealthBarDLL, healthBar);
    LLNode* healthBarNode = getHead(enemyHealthBarDLL);
    healthBar->barOwner = character;
    healthBar->x = ((ENEMY*)getData(character))->x;
    healthBar->y = ((ENEMY*)getData(character))->y + 9; // need to figure out offset amount
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
    return healthBarNode;
}

LLNode* createBossBar(LLNode* character) {
    BAR* bossHealthBar = (BAR*)malloc(sizeof(BAR));
    bossHealthBarNode = createLlnode(bossHealthBar);
    bossHealthBar->barOwner = character;
    bossHealthBar->x = ((BOSS*)getData(character))->x;
    bossHealthBar->y = ((BOSS*)getData(character))->y + 15; // need to figure out offset amount
    bossHealthBar->barType = TEN_HEARTS;
    bossHealthBar->numHearts = bossHealthBar->barType;
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

void deleteBossHealthBar(void) {
    if (bossHealthBarNode == NULL) {
        return;
    }
    free(bossHealthBarNode);
    bossHealthBarNode = NULL;
}