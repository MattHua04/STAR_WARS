#include "gameMusic.h"
#include "globals.h"
#include "graphics.h"
#include "doublyLinkedList.h"
#include "hardware.h"
#include "opponent.h"
#include "healthBar.h"
#include "projectile.h"
#include <cstdio>

PLAYER* opponent = NULL;
LLNode* opponentNode = NULL;

PLAYER* getOpponent(void) {
    return opponent;
}

void opponentInit(void) {
    opponent = (PLAYER*)malloc(sizeof(PLAYER));
    opponentNode = createLlnode(opponent);
    ((PLAYER*)getData(opponentNode))->x = PLAYER_XPOS;
    ((PLAYER*)getData(opponentNode))->y = 127 - PLAYER_YPOS;
    ((PLAYER*)getData(opponentNode))->px = PLAYER_XPOS;
    ((PLAYER*)getData(opponentNode))->py = PLAYER_YPOS;
    ((PLAYER*)getData(opponentNode))->width = PLAYER_WIDTH;
    ((PLAYER*)getData(opponentNode))->height = PLAYER_HEIGHT;
    BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    boundingBox->topLeft.x = ((PLAYER*)getData(opponentNode))->x - (((PLAYER*)getData(opponentNode))->width - 1) / 2;
    boundingBox->topLeft.y = ((PLAYER*)getData(opponentNode))->y + (((PLAYER*)getData(opponentNode))->height - 1) / 2;
    boundingBox->bottomRight.x = ((PLAYER*)getData(opponentNode))->x + (((PLAYER*)getData(opponentNode))->width - 1) / 2;
    boundingBox->bottomRight.y = ((PLAYER*)getData(opponentNode))->y - (((PLAYER*)getData(opponentNode))->height - 1) / 2;
    ((PLAYER*)getData(opponentNode))->boundingBox = boundingBox;
    ((PLAYER*)getData(opponentNode))->score = 0;
    ((PLAYER*)getData(opponentNode))->speed = PLAYER_SPEED;
    ((PLAYER*)getData(opponentNode))->tick = 0;
    ((PLAYER*)getData(opponentNode))->lastFireTick = -20;
    ((PLAYER*)getData(opponentNode))->fireInterval = 20;
    ((PLAYER*)getData(opponentNode))->superChargeStatus = 0; // Should start as 0
    ((PLAYER*)getData(opponentNode))->superActive = false;
    ((PLAYER*)getData(opponentNode))->type = CHARACTER_TYPE::OPPONENT_CHARACTER;
    ((PLAYER*)getData(opponentNode))->playerStatus = CHARACTER_STATUS::ACTIVE;
    ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::DEFAULT;
    ((PLAYER*)getData(opponentNode))->damagedTick = 0; // Lasts for 10 ticks
    ((PLAYER*)getData(opponentNode))->lastDamaged = -999; // Player can start healing once its current tick is 60 greater than lastDamaged
    ((PLAYER*)getData(opponentNode))->destroyedTick = 0; // Lasts for 20 ticks
    ((PLAYER*)getData(opponentNode))->healthBar = createOpponentBar(opponentNode);
}

void opponentNoMove(void) {
    ((PLAYER*)getData(opponentNode))->px = ((PLAYER*)getData(opponentNode))->x;
    ((PLAYER*)getData(opponentNode))->py = ((PLAYER*)getData(opponentNode))->y;
    // Update opponent display based on whether or not the opponent is currently taking damage
    if (((PLAYER*)getData(opponentNode))->playerDisplay >= 4 && ((PLAYER*)getData(opponentNode))->damagedTick < 5) {
        ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
        ((PLAYER*)getData(opponentNode))->damagedTick++;
    } else {
        ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::DEFAULT;
        ((PLAYER*)getData(opponentNode))->damagedTick = 0;
    }
}

void opponentMoveUp(void) {
    ((PLAYER*)getData(opponentNode))->px = ((PLAYER*)getData(opponentNode))->x;
    ((PLAYER*)getData(opponentNode))->py = ((PLAYER*)getData(opponentNode))->y;
    if (((PLAYER*)getData(opponentNode))->y + opponent->speed <= 127 - 6) {
        ((PLAYER*)getData(opponentNode))->y += opponent->speed;
        ((PLAYER*)getData(opponentNode))->boundingBox->topLeft.x = ((PLAYER*)getData(opponentNode))->x - (((PLAYER*)getData(opponentNode))->width - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->topLeft.y = ((PLAYER*)getData(opponentNode))->y + (((PLAYER*)getData(opponentNode))->height - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->bottomRight.x = ((PLAYER*)getData(opponentNode))->x + (((PLAYER*)getData(opponentNode))->width - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->bottomRight.y = ((PLAYER*)getData(opponentNode))->y - (((PLAYER*)getData(opponentNode))->height - 1) / 2;
    }
    // Update opponent display based on whether or not the opponent is currently taking damage
    if (((PLAYER*)getData(opponentNode))->playerDisplay >= 4 && ((PLAYER*)getData(opponentNode))->damagedTick < 5) {
        ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::VERTICALDAMAGED;
        ((PLAYER*)getData(opponentNode))->damagedTick++;
    } else {
        ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::VERTICAL;
        ((PLAYER*)getData(opponentNode))->damagedTick = 0;
    }
}

void opponentMoveDown(void) {
    ((PLAYER*)getData(opponentNode))->px = ((PLAYER*)getData(opponentNode))->x;
    ((PLAYER*)getData(opponentNode))->py = ((PLAYER*)getData(opponentNode))->y;
    if (((PLAYER*)getData(opponentNode))->y - opponent->speed >= 127 - 36 + 10) {
        ((PLAYER*)getData(opponentNode))->y -= opponent->speed;
        ((PLAYER*)getData(opponentNode))->boundingBox->topLeft.x = ((PLAYER*)getData(opponentNode))->x - (((PLAYER*)getData(opponentNode))->width - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->topLeft.y = ((PLAYER*)getData(opponentNode))->y + (((PLAYER*)getData(opponentNode))->height - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->bottomRight.x = ((PLAYER*)getData(opponentNode))->x + (((PLAYER*)getData(opponentNode))->width - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->bottomRight.y = ((PLAYER*)getData(opponentNode))->y - (((PLAYER*)getData(opponentNode))->height - 1) / 2;
    }
    // Update opponent display based on whether or not the opponent is currently taking damage
    if (((PLAYER*)getData(opponentNode))->playerDisplay >= 4 && ((PLAYER*)getData(opponentNode))->damagedTick < 5) {
        ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::VERTICALDAMAGED;
        ((PLAYER*)getData(opponentNode))->damagedTick++;
    } else {
        ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::VERTICAL;
        ((PLAYER*)getData(opponentNode))->damagedTick = 0;
    }
}

void opponentMoveLeft(void) {
    ((PLAYER*)getData(opponentNode))->px = ((PLAYER*)getData(opponentNode))->x;
    ((PLAYER*)getData(opponentNode))->py = ((PLAYER*)getData(opponentNode))->y;
    if (((PLAYER*)getData(opponentNode))->x - opponent->speed >= 0 + 6) {
        ((PLAYER*)getData(opponentNode))->x -= opponent->speed;
        ((PLAYER*)getData(opponentNode))->boundingBox->topLeft.x = ((PLAYER*)getData(opponentNode))->x - (((PLAYER*)getData(opponentNode))->width - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->topLeft.y = ((PLAYER*)getData(opponentNode))->y + (((PLAYER*)getData(opponentNode))->height - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->bottomRight.x = ((PLAYER*)getData(opponentNode))->x + (((PLAYER*)getData(opponentNode))->width - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->bottomRight.y = ((PLAYER*)getData(opponentNode))->y - (((PLAYER*)getData(opponentNode))->height - 1) / 2;
    }
    // Update opponent display based on whether or not the opponent is currently taking damage
    if (((PLAYER*)getData(opponentNode))->playerDisplay >= 4 && ((PLAYER*)getData(opponentNode))->damagedTick < 5) {
        ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
        ((PLAYER*)getData(opponentNode))->damagedTick++;
    } else {
        ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::RIGHT;
        ((PLAYER*)getData(opponentNode))->damagedTick = 0;
    }
}

void opponentMoveRight(void) {
    ((PLAYER*)getData(opponentNode))->px = ((PLAYER*)getData(opponentNode))->x;
    ((PLAYER*)getData(opponentNode))->py = ((PLAYER*)getData(opponentNode))->y;
    if (((PLAYER*)getData(opponentNode))->x + opponent->speed <= 127 - 6) {
        ((PLAYER*)getData(opponentNode))->x += opponent->speed;
        ((PLAYER*)getData(opponentNode))->boundingBox->topLeft.x = ((PLAYER*)getData(opponentNode))->x - (((PLAYER*)getData(opponentNode))->width - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->topLeft.y = ((PLAYER*)getData(opponentNode))->y + (((PLAYER*)getData(opponentNode))->height - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->bottomRight.x = ((PLAYER*)getData(opponentNode))->x + (((PLAYER*)getData(opponentNode))->width - 1) / 2;
        ((PLAYER*)getData(opponentNode))->boundingBox->bottomRight.y = ((PLAYER*)getData(opponentNode))->y - (((PLAYER*)getData(opponentNode))->height - 1) / 2;
    }
    // Update opponent display based on whether or not the opponent is currently taking damage
    if (((PLAYER*)getData(opponentNode))->playerDisplay >= 4 && ((PLAYER*)getData(opponentNode))->damagedTick < 5) {
        ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
        ((PLAYER*)getData(opponentNode))->damagedTick++;
    } else {
        ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::LEFT;
        ((PLAYER*)getData(opponentNode))->damagedTick = 0;
    }
}

void opponentFireProjectile(void) {
    // Only fire a projectile if the opponent has waited for fireInterval and super attack is not active
    if (opponent->tick - opponent->lastFireTick >= opponent->fireInterval && !opponent->superActive) {
        projectileSound();
        generateOpponentProjectile(opponentNode);
        ((PLAYER*)getData(opponentNode))->lastFireTick = opponent->tick;
    }
}

void opponentSuperAttack(void) {
    // Only start super attack if superChargeStatus is full
    if (opponent->superChargeStatus >= MAX_SUPER_CHARGE && !opponent->superActive) {
        laserSound();
        ((PLAYER*)getData(opponentNode))->superChargeStatus = 0;
        ((PLAYER*)getData(opponentNode))->superActive = true;
        generateOpponentProjectile(opponentNode);
    }
}

void opponentUpdate(void) {
    if (opponentNode == NULL) {
        return;
    }
    if (getOpponent()->playerStatus != CHARACTER_STATUS::DEAD) {
        GAME_INPUTS* inputs = readInputs();
        // Get joystick input
        // Switched all movement since opponent is mirrored
        if (inputs->opUp) {
            opponentMoveDown();
        } else if (inputs->opDown) {
            opponentMoveUp();
        } else if (inputs->opLeft) {
            opponentMoveRight();
        } else if (inputs->opRight) {
            opponentMoveLeft();
        } else {
            opponentNoMove();
        }
        // Get button inputs
        if (!inputs->opNormalAttack && inputs->opSuperAttack && inputs->opPauseResume) {
            opponentFireProjectile();
        } else if (inputs->opNormalAttack && !inputs->opSuperAttack && inputs->opPauseResume) {
            opponentSuperAttack();
        }
    }
    if (getOpponent()->tick - getOpponent()->lastDamaged > 60 && ((BAR*)getData(getOpponent()->healthBar))->numHearts < ((BAR*)getData(getOpponent()->healthBar))->barType) {
        ((BAR*)getData(getOpponent()->healthBar))->numHearts++;
        getOpponent()->lastDamaged = getOpponent()->tick;
    }
    // Update opponent
    if (((PLAYER*)getData(opponentNode))->playerStatus == CHARACTER_STATUS::DEAD) {
        if (((PLAYER*)getData(opponentNode))->destroyedTick < 20) {
            ((PLAYER*)getData(opponentNode))->playerDisplay = CHARACTER_DISPLAY::DESTROYED;
            ((PLAYER*)getData(opponentNode))->destroyedTick++;
        } else {
            eraseOpponent(opponentNode);
            deleteOpponent();
            return;
        }
    }
    drawOpponent(opponentNode);
    ((PLAYER*)getData(opponentNode))->tick++;
}

void deleteOpponent(void) {
    if (opponentNode == NULL) {
        return;
    }
    deleteOpponentHealthBar();
    free(((PLAYER*)getData(opponentNode))->boundingBox);
    free(opponentNode);
    opponentNode = NULL;
    opponent = NULL;
}
