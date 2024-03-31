#include "gameMusic.hpp"
#include "globals.hpp"
#include "graphics.hpp"
#include "doublyLinkedList.hpp"
#include "hardware.hpp"
#include "player.hpp"
#include "healthBar.hpp"
#include "projectile.hpp"
#include <cstdio>

PLAYER* player = NULL;
LLNode* playerNode = NULL;

PLAYER* getPlayer(void) {
    return player;
}

void playerInit(void) {
    player = (PLAYER*)malloc(sizeof(PLAYER));
    playerNode = createLlnode(player);
    ((PLAYER*)getData(playerNode))->x = PLAYER_XPOS;
    ((PLAYER*)getData(playerNode))->y = PLAYER_YPOS;
    ((PLAYER*)getData(playerNode))->px = PLAYER_XPOS;
    ((PLAYER*)getData(playerNode))->py = PLAYER_YPOS;
    ((PLAYER*)getData(playerNode))->width = PLAYER_WIDTH;
    ((PLAYER*)getData(playerNode))->height = PLAYER_HEIGHT;
    BOUNDINGBOX* boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    boundingBox->topLeft.x = ((PLAYER*)getData(playerNode))->x - (((PLAYER*)getData(playerNode))->width - 1) / 2;
    boundingBox->topLeft.y = ((PLAYER*)getData(playerNode))->y + (((PLAYER*)getData(playerNode))->height - 1) / 2;
    boundingBox->bottomRight.x = ((PLAYER*)getData(playerNode))->x + (((PLAYER*)getData(playerNode))->width - 1) / 2;
    boundingBox->bottomRight.y = ((PLAYER*)getData(playerNode))->y - (((PLAYER*)getData(playerNode))->height - 1) / 2;
    ((PLAYER*)getData(playerNode))->boundingBox = boundingBox;
    // Stats need to be saved and reset after every game
    ((PLAYER*)getData(playerNode))->score = 0;
    ((PLAYER*)getData(playerNode))->sessionPlayTime = 0; // Accumulated in us and converted to seconds for storage
    ((PLAYER*)getData(playerNode))->sessionKills = 0;
    ((PLAYER*)getData(playerNode))->sessionDeaths = 0;

    ((PLAYER*)getData(playerNode))->speed = PLAYER_SPEED;
    ((PLAYER*)getData(playerNode))->tick = 0;
    ((PLAYER*)getData(playerNode))->lastFireTick = -20;
    ((PLAYER*)getData(playerNode))->fireInterval = 20;
    ((PLAYER*)getData(playerNode))->superChargeStatus = 0; // Should start as 0
    ((PLAYER*)getData(playerNode))->superActive = false;
    ((PLAYER*)getData(playerNode))->type = CHARACTER_TYPE::PLAYER_CHARACTER;
    ((PLAYER*)getData(playerNode))->playerStatus = CHARACTER_STATUS::ACTIVE;
    ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::DEFAULT;
    ((PLAYER*)getData(playerNode))->damagedTick = 0; // Lasts for 10 ticks
    ((PLAYER*)getData(playerNode))->lastDamaged = -999; // Player can start healing once its current tick is 60 greater than lastDamaged
    ((PLAYER*)getData(playerNode))->destroyedTick = 0; // Lasts for 20 ticks
    ((PLAYER*)getData(playerNode))->healthBar = createPlayerBar(playerNode);
}

void playerNoMove(void) {
    ((PLAYER*)getData(playerNode))->px = ((PLAYER*)getData(playerNode))->x;
    ((PLAYER*)getData(playerNode))->py = ((PLAYER*)getData(playerNode))->y;
    // Update player display based on whether or not the player is currently taking damage
    if (((PLAYER*)getData(playerNode))->playerDisplay >= 4 && ((PLAYER*)getData(playerNode))->damagedTick < 5) {
        ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::DEFAULTDAMAGED;
        ((PLAYER*)getData(playerNode))->damagedTick++;
    } else {
        ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::DEFAULT;
        ((PLAYER*)getData(playerNode))->damagedTick = 0;
    }
}

void playerMoveUp(void) {
    ((PLAYER*)getData(playerNode))->px = ((PLAYER*)getData(playerNode))->x;
    ((PLAYER*)getData(playerNode))->py = ((PLAYER*)getData(playerNode))->y;
    if (((PLAYER*)getData(playerNode))->y + player->speed <= 36 - 10) {
        ((PLAYER*)getData(playerNode))->y += player->speed;
        ((PLAYER*)getData(playerNode))->boundingBox->topLeft.x = ((PLAYER*)getData(playerNode))->x - (((PLAYER*)getData(playerNode))->width - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->topLeft.y = ((PLAYER*)getData(playerNode))->y + (((PLAYER*)getData(playerNode))->height - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->bottomRight.x = ((PLAYER*)getData(playerNode))->x + (((PLAYER*)getData(playerNode))->width - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->bottomRight.y = ((PLAYER*)getData(playerNode))->y - (((PLAYER*)getData(playerNode))->height - 1) / 2;
    }
    // Update player display based on whether or not the player is currently taking damage
    if (((PLAYER*)getData(playerNode))->playerDisplay >= 4 && ((PLAYER*)getData(playerNode))->damagedTick < 5) {
        ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::VERTICALDAMAGED;
        ((PLAYER*)getData(playerNode))->damagedTick++;
    } else {
        ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::VERTICAL;
        ((PLAYER*)getData(playerNode))->damagedTick = 0;
    }
}

void playerMoveDown(void) {
    ((PLAYER*)getData(playerNode))->px = ((PLAYER*)getData(playerNode))->x;
    ((PLAYER*)getData(playerNode))->py = ((PLAYER*)getData(playerNode))->y;
    if (((PLAYER*)getData(playerNode))->y - player->speed >= 0 + 6) {
        ((PLAYER*)getData(playerNode))->y -= player->speed;
        ((PLAYER*)getData(playerNode))->boundingBox->topLeft.x = ((PLAYER*)getData(playerNode))->x - (((PLAYER*)getData(playerNode))->width - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->topLeft.y = ((PLAYER*)getData(playerNode))->y + (((PLAYER*)getData(playerNode))->height - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->bottomRight.x = ((PLAYER*)getData(playerNode))->x + (((PLAYER*)getData(playerNode))->width - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->bottomRight.y = ((PLAYER*)getData(playerNode))->y - (((PLAYER*)getData(playerNode))->height - 1) / 2;
    }
    // Update player display based on whether or not the player is currently taking damage
    if (((PLAYER*)getData(playerNode))->playerDisplay >= 4 && ((PLAYER*)getData(playerNode))->damagedTick < 5) {
        ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::VERTICALDAMAGED;
        ((PLAYER*)getData(playerNode))->damagedTick++;
    } else {
        ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::VERTICAL;
        ((PLAYER*)getData(playerNode))->damagedTick = 0;
    }
}

void playerMoveLeft(void) {
    ((PLAYER*)getData(playerNode))->px = ((PLAYER*)getData(playerNode))->x;
    ((PLAYER*)getData(playerNode))->py = ((PLAYER*)getData(playerNode))->y;
    if (((PLAYER*)getData(playerNode))->x - player->speed >= 0 + 6) {
        ((PLAYER*)getData(playerNode))->x -= player->speed;
        ((PLAYER*)getData(playerNode))->boundingBox->topLeft.x = ((PLAYER*)getData(playerNode))->x - (((PLAYER*)getData(playerNode))->width - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->topLeft.y = ((PLAYER*)getData(playerNode))->y + (((PLAYER*)getData(playerNode))->height - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->bottomRight.x = ((PLAYER*)getData(playerNode))->x + (((PLAYER*)getData(playerNode))->width - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->bottomRight.y = ((PLAYER*)getData(playerNode))->y - (((PLAYER*)getData(playerNode))->height - 1) / 2;
    }
    // Update player display based on whether or not the player is currently taking damage
    if (((PLAYER*)getData(playerNode))->playerDisplay >= 4 && ((PLAYER*)getData(playerNode))->damagedTick < 5) {
        ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::LEFTDAMAGED;
        ((PLAYER*)getData(playerNode))->damagedTick++;
    } else {
        ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::LEFT;
        ((PLAYER*)getData(playerNode))->damagedTick = 0;
    }
}

void playerMoveRight(void) {
    ((PLAYER*)getData(playerNode))->px = ((PLAYER*)getData(playerNode))->x;
    ((PLAYER*)getData(playerNode))->py = ((PLAYER*)getData(playerNode))->y;
    if (((PLAYER*)getData(playerNode))->x + player->speed <= 127 - 6) {
        ((PLAYER*)getData(playerNode))->x += player->speed;
        ((PLAYER*)getData(playerNode))->boundingBox->topLeft.x = ((PLAYER*)getData(playerNode))->x - (((PLAYER*)getData(playerNode))->width - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->topLeft.y = ((PLAYER*)getData(playerNode))->y + (((PLAYER*)getData(playerNode))->height - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->bottomRight.x = ((PLAYER*)getData(playerNode))->x + (((PLAYER*)getData(playerNode))->width - 1) / 2;
        ((PLAYER*)getData(playerNode))->boundingBox->bottomRight.y = ((PLAYER*)getData(playerNode))->y - (((PLAYER*)getData(playerNode))->height - 1) / 2;
    }
    // Update player display based on whether or not the player is currently taking damage
    if (((PLAYER*)getData(playerNode))->playerDisplay >= 4 && ((PLAYER*)getData(playerNode))->damagedTick < 5) {
        ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::RIGHTDAMAGED;
        ((PLAYER*)getData(playerNode))->damagedTick++;
    } else {
        ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::RIGHT;
        ((PLAYER*)getData(playerNode))->damagedTick = 0;
    }
}

void playerFireProjectile(void) {
    // Only fire a projectile if the player has waited for fireInterval and super attack is not active
    if (player->tick - player->lastFireTick >= player->fireInterval && !player->superActive) {
        projectileSound();
        generatePlayerProjectile(playerNode);
        ((PLAYER*)getData(playerNode))->lastFireTick = player->tick;
    }
}

void playerSuperAttack(void) {
    // Only start super attack if superChargeStatus is full
    if (player->superChargeStatus >= MAX_SUPER_CHARGE && !player->superActive) {
        laserSound();
        ((PLAYER*)getData(playerNode))->superChargeStatus = 0;
        ((PLAYER*)getData(playerNode))->superActive = true;
        generatePlayerProjectile(playerNode);
    }
}

void playerUpdate(void) {
    if (playerNode == NULL) {
        return;
    }
    if (getPlayer()->playerStatus != CHARACTER_STATUS::DEAD) {
        GAME_INPUTS* inputs = readInputs();
        // Get joystick input
        if (inputs->up) {
            playerMoveUp();
        } else if (inputs->down) {
            playerMoveDown();
        } else if (inputs->left) {
            playerMoveLeft();
        } else if (inputs->right) {
            playerMoveRight();
        } else {
            playerNoMove();
        }
        // Get button inputs
        if (!inputs->normalAttack && inputs->superAttack && inputs->pauseResume) {
            playerFireProjectile();
        } else if (inputs->normalAttack && !inputs->superAttack && inputs->pauseResume) {
            playerSuperAttack();
        }
    }
    if (getPlayer()->tick - getPlayer()->lastDamaged > 60 && ((BAR*)getData(getPlayer()->healthBar))->numHearts < ((BAR*)getData(getPlayer()->healthBar))->barType) {
        ((BAR*)getData(getPlayer()->healthBar))->numHearts++;
        getPlayer()->lastDamaged = getPlayer()->tick;
    }
    // Update player
    if (((PLAYER*)getData(playerNode))->playerStatus == CHARACTER_STATUS::DEAD) {
        if (((PLAYER*)getData(playerNode))->destroyedTick < 20) {
            ((PLAYER*)getData(playerNode))->playerDisplay = CHARACTER_DISPLAY::DESTROYED;
            ((PLAYER*)getData(playerNode))->destroyedTick++;
        } else {
            erasePlayer(playerNode);
            eraseSuperAttackBar(playerNode);
            return;
        }
    }
    drawPlayer(playerNode);
    drawSuperAttackBar(playerNode);
    ((PLAYER*)getData(playerNode))->tick++;
}

void deletePlayer(void) {
    if (playerNode == NULL) {
        return;
    }
    deletePlayerHealthBar();
    free(((PLAYER*)getData(playerNode))->boundingBox);
    free(playerNode);
    playerNode = NULL;
    player = NULL;
}
