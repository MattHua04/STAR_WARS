#include "graphics.hpp"
#include "gameMusic.hpp"
#include "globals.hpp"
#include "doublyLinkedList.hpp"
#include "login.hpp"
#include "menu.hpp"
#include "projectile.hpp"
#include "player.hpp"
#include "opponent.hpp"
#include "enemy.hpp"
#include "finalBoss.hpp"
#include "game.hpp"
#include "registration.hpp"
#include "uLCD.hpp"
#include "users.hpp"

int getHexColor(char color, bool isOpponent) {
    if (color == 'X' && isOpponent) color = 'R';
    else if (color == 'X' && getMenuSettings()->playerSkin) color = getMenuSettings()->playerSkin;
    else if (color == 'X' && getNewUser()->defaultSkin) color = getNewUser()->defaultSkin;
    else if (color == 'X' && getUserInfo()->defaultSkin) color = getUserInfo()->defaultSkin;
    else if (color == 'X') color = 'B';
    switch (color) {
    case 'R': return RED;
    case 'O': return ORANGE;
    case 'Y': return YELLOW;
    case 'G': return GREEN;
    case 'B': return BLUE;
    case 'V': return VIOLET;
    case 'P': return PINK;
    case 'W': return WHITE;
    case '5': return LGREY;
    case '4': return MGREY;
    case '3': return DGREY;
    case '0': return BLACK;
    default: return BLACK;
    }
}

void loadChunk(FILE *file, int *chunk, int startX, int startY, int width, int height) {
    fseek(file, (startY * 128 + startX) * sizeof(char) * 9, SEEK_SET);
    char buffer[9];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fread(buffer, sizeof(char), 9, file);
            chunk[i * width + j] = strtol(buffer, NULL, 16);
        }
        fseek(file, (128 - width) * sizeof(char) * 9, SEEK_CUR);
    }
}

void drawProfileImg(void) {
    FILE *file = fopen("/sd/profileImg.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    int chunkHeight = 32;
    for (int i = 0; i < 128; i += chunkHeight) {
        int chunk[128 * chunkHeight];
        loadChunk(file, chunk, 0, i, 128, chunkHeight);
        uLCD.BLIT(0, i, 128, chunkHeight, chunk);
    }
    fclose(file);
}

void drawGameBackground(bool redshift) {
    int chunkHeight = 32;
    for (int i = 0; i < 128; i += chunkHeight) {
        int chunk[128 * chunkHeight];
        for (int j = 0; j < chunkHeight; j++) {
            for (int k = 0; k < 128; k++) {
                loadMusic();
                chunk[j * 128 + k] = (redshift) ? gameBackground[i * 128 + j * 128 + k] & 0xFF0000 : gameBackground[i * 128 + j * 128 + k];
            }
        }
        uLCD.BLIT(0, i, 128, chunkHeight, chunk);
    }
}

void drawImg(int x, int y, int width, int height, const char* object) {
    // Ensure coordinates are actually on the screen
    x = max(0, min(x, 127));
    y = max(0, min(y, 127));

    int img[width * height];
    for (int i = 0; i < width * height; i++) {
        int xPos = x + (i % width);
        int yPos = y + (i / width);
        xPos = max(0, min(xPos, 127));
        yPos = max(0, min(yPos, 127));
        if (object[i] == '0') {
            if (getGameLoop() && getGameLoop()->gameStatus == GAMESTATUS::LOST) img[i] = gameBackground[yPos * 128 + xPos] & 0xFF0000;
            else img[i] = gameBackground[yPos * 128 + xPos];
        } else {
            img[i] = getHexColor(object[i], false);
        }
    }
    uLCD.BLIT(x, y, width, height, img);
}


void drawImgOpponent(int x, int y, int width, int height, const char* object) {
    // Ensure coordinates are actually on the screen
    x = max(0, min(x, 127));
    y = max(0, min(y, 127));

    int img[width * height];
    for (int i = width * height - 1; i >= 0; i--) {
        int xPos = x + ((width * height - 1 - i) % width);
        int yPos = y + ((width * height - 1 - i) / width);
        xPos = max(0, min(xPos, 127));
        yPos = max(0, min(yPos, 127));
        if (object[i] == '0') {
            img[width * height - 1 - i] = gameBackground[yPos * 128 + xPos];
        } else {
            img[width * height - 1 - i] = getHexColor(object[i], true);
        }
    }
    uLCD.BLIT(x, y, width, height, img);
}


void drawBox(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, char color) {
    uLCD.filled_rectangle(topLeftX, topLeftY, bottomRightX, bottomRightY, getHexColor(color, false));
}

void drawBackgroundBox(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
    // Ensure coordinates are actually on the screen
    topLeftX = max(0, min(topLeftX, 127));
    topLeftY = max(0, min(topLeftY, 127));
    bottomRightX = max(0, min(bottomRightX, 127));
    bottomRightY = max(0, min(bottomRightY, 127));
    
    int width = bottomRightX - topLeftX + 1;
    int height = bottomRightY - topLeftY + 1;
    int img[width * height];
    int index = 0;
    for (int y = topLeftY; y <= bottomRightY; y++) {
        for (int x = topLeftX; x <= bottomRightX; x++) {
            img[index++] = gameBackground[max(0, min(y, 127)) * 128 + max(0, min(x, 127))];
        }
    }
    uLCD.BLIT(topLeftX, topLeftY, width, height, img);
}

void drawPlayer(LLNode* player) {
    int pTopLeftX = ((PLAYER*)getData(player))->boundingBox->topLeft.x - (((PLAYER*)getData(player))->x - ((PLAYER*)getData(player))->px);
    int pBottomRightX = ((PLAYER*)getData(player))->boundingBox->bottomRight.x - (((PLAYER*)getData(player))->x - ((PLAYER*)getData(player))->px);
    int pTopLeftY = 127 - (((PLAYER*)getData(player))->boundingBox->topLeft.y - (((PLAYER*)getData(player))->y - ((PLAYER*)getData(player))->py));
    int pBottomRightY = 127 - (((PLAYER*)getData(player))->boundingBox->bottomRight.y - (((PLAYER*)getData(player))->y - ((PLAYER*)getData(player))->py));
    if (((PLAYER*)getData(player))->x > ((PLAYER*)getData(player))->px) { // Moved right
        drawBackgroundBox(pTopLeftX, pTopLeftY, pTopLeftX + (((PLAYER*)getData(player))->x - ((PLAYER*)getData(player))->px), pBottomRightY);
    } else if (((PLAYER*)getData(player))->x < ((PLAYER*)getData(player))->px) { // Moved left
        drawBackgroundBox(pBottomRightX - (((PLAYER*)getData(player))->px - ((PLAYER*)getData(player))->x), pTopLeftY, pBottomRightX, pBottomRightY);
    } else if (((PLAYER*)getData(player))->y > ((PLAYER*)getData(player))->py) { // Moved up
        drawBackgroundBox(pTopLeftX, pBottomRightY + (((PLAYER*)getData(player))->py - ((PLAYER*)getData(player))->y), pBottomRightX, pBottomRightY);
    } else if (((PLAYER*)getData(player))->y < ((PLAYER*)getData(player))->py) { // Moved down
        drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((PLAYER*)getData(player))->y - ((PLAYER*)getData(player))->py));
    }
    drawImg(((PLAYER*)getData(player))->boundingBox->topLeft.x, 127 - ((PLAYER*)getData(player))->boundingBox->topLeft.y, 11, 11, PLAYER_IMGS[((PLAYER*)getData(player))->playerDisplay]);
}

void erasePlayer(LLNode* player) {
    drawBackgroundBox(((PLAYER*)getData(player))->boundingBox->topLeft.x, 127 - ((PLAYER*)getData(player))->boundingBox->topLeft.y, ((PLAYER*)getData(player))->boundingBox->bottomRight.x, 127 - ((PLAYER*)getData(player))->boundingBox->bottomRight.y);
}

void drawOpponent(LLNode* opponent) {
    int pTopLeftX = ((PLAYER*)getData(opponent))->boundingBox->topLeft.x - (((PLAYER*)getData(opponent))->x - ((PLAYER*)getData(opponent))->px);
    int pBottomRightX = ((PLAYER*)getData(opponent))->boundingBox->bottomRight.x - (((PLAYER*)getData(opponent))->x - ((PLAYER*)getData(opponent))->px);
    int pTopLeftY = 127 - (((PLAYER*)getData(opponent))->boundingBox->topLeft.y - (((PLAYER*)getData(opponent))->y - ((PLAYER*)getData(opponent))->py));
    int pBottomRightY = 127 - (((PLAYER*)getData(opponent))->boundingBox->bottomRight.y - (((PLAYER*)getData(opponent))->y - ((PLAYER*)getData(opponent))->py));
    if (((PLAYER*)getData(opponent))->x > ((PLAYER*)getData(opponent))->px) { // Moved right
        drawBackgroundBox(pTopLeftX, pTopLeftY, pTopLeftX + (((PLAYER*)getData(opponent))->x - ((PLAYER*)getData(opponent))->px), pBottomRightY);
    } else if (((PLAYER*)getData(opponent))->x < ((PLAYER*)getData(opponent))->px) { // Moved left
        drawBackgroundBox(pBottomRightX - (((PLAYER*)getData(opponent))->px - ((PLAYER*)getData(opponent))->x), pTopLeftY, pBottomRightX, pBottomRightY);
    } else if (((PLAYER*)getData(opponent))->y > ((PLAYER*)getData(opponent))->py) { // Moved up
        drawBackgroundBox(pTopLeftX, pBottomRightY + (((PLAYER*)getData(opponent))->py - ((PLAYER*)getData(opponent))->y), pBottomRightX, pBottomRightY);
    } else if (((PLAYER*)getData(opponent))->y < ((PLAYER*)getData(opponent))->py) { // Moved down
        drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((PLAYER*)getData(opponent))->y - ((PLAYER*)getData(opponent))->py));
    }
    drawImgOpponent(((PLAYER*)getData(opponent))->boundingBox->topLeft.x, 127 - ((PLAYER*)getData(opponent))->boundingBox->topLeft.y, 11, 11, PLAYER_IMGS[((PLAYER*)getData(opponent))->playerDisplay]);
}

void eraseOpponent(LLNode* opponent) {
    drawBackgroundBox(((PLAYER*)getData(opponent))->boundingBox->topLeft.x, 127 - ((PLAYER*)getData(opponent))->boundingBox->topLeft.y, ((PLAYER*)getData(opponent))->boundingBox->bottomRight.x, 127 - ((PLAYER*)getData(opponent))->boundingBox->bottomRight.y);
}

void drawEnemy(LLNode* enemy) {
    int pTopLeftX = ((ENEMY*)getData(enemy))->boundingBox->topLeft.x - (((ENEMY*)getData(enemy))->x - ((ENEMY*)getData(enemy))->px);
    int pBottomRightX = ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x - (((ENEMY*)getData(enemy))->x - ((ENEMY*)getData(enemy))->px);
    int pTopLeftY = 127 - (((ENEMY*)getData(enemy))->boundingBox->topLeft.y - (((ENEMY*)getData(enemy))->y - ((ENEMY*)getData(enemy))->py));
    int pBottomRightY = 127 - (((ENEMY*)getData(enemy))->boundingBox->bottomRight.y - (((ENEMY*)getData(enemy))->y - ((ENEMY*)getData(enemy))->py));
    if (((ENEMY*)getData(enemy))->x > ((ENEMY*)getData(enemy))->px) { // Moved right
        drawBackgroundBox(pTopLeftX - 1, pTopLeftY, pTopLeftX + (((ENEMY*)getData(enemy))->x - ((ENEMY*)getData(enemy))->px), pBottomRightY);
    } else if (((ENEMY*)getData(enemy))->x < ((ENEMY*)getData(enemy))->px) { // Moved left
        drawBackgroundBox(pBottomRightX - (((ENEMY*)getData(enemy))->px - ((ENEMY*)getData(enemy))->x), pTopLeftY, pBottomRightX + 1, pBottomRightY);
    } else if (((ENEMY*)getData(enemy))->y > ((ENEMY*)getData(enemy))->py) { // Moved up
        drawBackgroundBox(pTopLeftX, pBottomRightY + (((ENEMY*)getData(enemy))->py - ((ENEMY*)getData(enemy))->y), pBottomRightX, pBottomRightY);
    } else if (((ENEMY*)getData(enemy))->y < ((ENEMY*)getData(enemy))->py) { // Moved down
        drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((ENEMY*)getData(enemy))->y - ((ENEMY*)getData(enemy))->py));
    }
    if (((ENEMY*)getData(enemy))->enemyType == CHARACTER_TYPE::SHORT_RANGE_ENEMY || ((ENEMY*)getData(enemy))->enemyType == CHARACTER_TYPE::LONG_RANGE_ENEMY) {
        drawImg(((ENEMY*)getData(enemy))->boundingBox->topLeft.x, 127 - ((ENEMY*)getData(enemy))->boundingBox->topLeft.y, 11, 11, NORMAL_ENEMY_IMGS[((ENEMY*)getData(enemy))->enemyDisplay]);
    } else {
        drawImg(((ENEMY*)getData(enemy))->boundingBox->topLeft.x, 127 - ((ENEMY*)getData(enemy))->boundingBox->topLeft.y, 11, 11, MISSILE_ENEMY_IMGS[((ENEMY*)getData(enemy))->enemyDisplay]);
    }
}

void eraseEnemy(LLNode* enemy) {
    drawBackgroundBox(((ENEMY*)getData(enemy))->boundingBox->topLeft.x, 127 - ((ENEMY*)getData(enemy))->boundingBox->topLeft.y, ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x, 127 - ((ENEMY*)getData(enemy))->boundingBox->bottomRight.y);
}

void drawBoss(LLNode* boss) {
    int pTopLeftX = ((BOSS*)getData(boss))->boundingBox->topLeft.x - (((BOSS*)getData(boss))->x - ((BOSS*)getData(boss))->px);
    int pBottomRightX = ((BOSS*)getData(boss))->boundingBox->bottomRight.x - (((BOSS*)getData(boss))->x - ((BOSS*)getData(boss))->px);
    int pTopLeftY = 127 - (((BOSS*)getData(boss))->boundingBox->topLeft.y - (((BOSS*)getData(boss))->y - ((BOSS*)getData(boss))->py));
    int pBottomRightY = 127 - (((BOSS*)getData(boss))->boundingBox->bottomRight.y - (((BOSS*)getData(boss))->y - ((BOSS*)getData(boss))->py));
    if (((BOSS*)getData(boss))->x > ((BOSS*)getData(boss))->px) { // Moved right
        drawBackgroundBox(pTopLeftX - 2, pTopLeftY, pTopLeftX + (((BOSS*)getData(boss))->x - ((BOSS*)getData(boss))->px), pBottomRightY);
    } else if (((BOSS*)getData(boss))->x < ((BOSS*)getData(boss))->px) { // Moved left
        drawBackgroundBox(pBottomRightX - (((BOSS*)getData(boss))->px - ((BOSS*)getData(boss))->x), pTopLeftY, pBottomRightX + 2, pBottomRightY);
    } else if (((BOSS*)getData(boss))->y > ((BOSS*)getData(boss))->py) { // Moved up
        drawBackgroundBox(pTopLeftX, pBottomRightY + (((BOSS*)getData(boss))->py - ((BOSS*)getData(boss))->y), pBottomRightX, pBottomRightY);
    } else if (((BOSS*)getData(boss))->y < ((BOSS*)getData(boss))->py) { // Moved down
        drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((BOSS*)getData(boss))->y - ((BOSS*)getData(boss))->py));
    }
    drawImg(((BOSS*)getData(boss))->boundingBox->topLeft.x, 127 - ((BOSS*)getData(boss))->boundingBox->topLeft.y, 19, 19, BOSS_IMGS[((BOSS*)getData(boss))->bossDisplay]);
}

void eraseBoss(LLNode* boss) {
    drawBackgroundBox(((BOSS*)getData(boss))->boundingBox->topLeft.x, 127 - ((BOSS*)getData(boss))->boundingBox->topLeft.y, ((BOSS*)getData(boss))->boundingBox->bottomRight.x, 127 - ((BOSS*)getData(boss))->boundingBox->bottomRight.y);
}

void drawPlayerProjectile(LLNode* projectile) {
    // Revert old pixels to background
    int pTopLeftX = ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px);
    int pBottomRightX = ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x - (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px);
    int pTopLeftY = 127 - (((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    int pBottomRightY = 127 - (((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    if (((PROJECTILE*)getData(projectile))->x > ((PROJECTILE*)getData(projectile))->px) { // Moved right
        drawBackgroundBox(pTopLeftX, pTopLeftY, pTopLeftX + (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px), pBottomRightY);
    } else if (((PROJECTILE*)getData(projectile))->x < ((PROJECTILE*)getData(projectile))->px) { // Moved left
        drawBackgroundBox(pBottomRightX - (((PROJECTILE*)getData(projectile))->px - ((PROJECTILE*)getData(projectile))->x), pTopLeftY, pBottomRightX, pBottomRightY);
    } else if (((PROJECTILE*)getData(projectile))->y > ((PROJECTILE*)getData(projectile))->py) { // Moved up
        drawBackgroundBox(pTopLeftX, pBottomRightY + (((PROJECTILE*)getData(projectile))->py - ((PROJECTILE*)getData(projectile))->y), pBottomRightX, pBottomRightY);
    } else if (((PROJECTILE*)getData(projectile))->y < ((PROJECTILE*)getData(projectile))->py) { // Moved down
        drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    }
    // Draw the new projectile position
    if (getPlayer()->superActive) {
        drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, 'R');
    } else {
        if (getMenuSettings()->playerSkin != NULL){
            drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, getMenuSettings()->playerSkin);
        } else {
            drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, 'B');
        }
    }
}

void erasePlayerProjectile(LLNode* projectile) {
    if (getPlayer()->superActive) {
        drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
    } else {
        drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
    }
}

void drawOpponentProjectile(LLNode* projectile) {
    // Revert old pixels to background
    int pTopLeftX = ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px);
    int pBottomRightX = ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x - (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px);
    int pTopLeftY = 127 - (((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    int pBottomRightY = 127 - (((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    if (((PROJECTILE*)getData(projectile))->x > ((PROJECTILE*)getData(projectile))->px) { // Moved right
        drawBackgroundBox(pTopLeftX, pTopLeftY, pTopLeftX + (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px), pBottomRightY);
    } else if (((PROJECTILE*)getData(projectile))->x < ((PROJECTILE*)getData(projectile))->px) { // Moved left
        drawBackgroundBox(pBottomRightX - (((PROJECTILE*)getData(projectile))->px - ((PROJECTILE*)getData(projectile))->x), pTopLeftY, pBottomRightX, pBottomRightY);
    } else if (((PROJECTILE*)getData(projectile))->y > ((PROJECTILE*)getData(projectile))->py) { // Moved up
        drawBackgroundBox(pTopLeftX, pBottomRightY + (((PROJECTILE*)getData(projectile))->py - ((PROJECTILE*)getData(projectile))->y), pBottomRightX, pBottomRightY);
    } else if (((PROJECTILE*)getData(projectile))->y < ((PROJECTILE*)getData(projectile))->py) { // Moved down
        drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    }
    // Draw the new projectile position
    if (getOpponent()->superActive) {
        drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, 'R');
    } else {
        drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, 'R');
    }
}

void eraseOpponentProjectile(LLNode* projectile) {
    drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
}

void drawEnemyProjectile(LLNode* projectile) {
    // Revert old pixels to background
    int pTopLeftX = ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px);
    int pBottomRightX = ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x - (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px);
    int pTopLeftY = 127 - (((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    int pBottomRightY = 127 - (((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    if (((PROJECTILE*)getData(projectile))->projectileType == PROJECTILE_TYPE::MISSILE) {
        if ((int)((PROJECTILE*)getData(projectile))->projectileDirection->pdx < 0 || (int)((PROJECTILE*)getData(projectile))->projectileDirection->dx < 0) { // Was or is going left
            drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX + 3, pBottomRightY);
        } else if ((int)((PROJECTILE*)getData(projectile))->projectileDirection->pdx > 0 || (int)((PROJECTILE*)getData(projectile))->projectileDirection->dx > 0) { // Was or is going right
            drawBackgroundBox(pTopLeftX - 3, pTopLeftY, pBottomRightX, pBottomRightY);
        } else { // Is going straight
            drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX , pBottomRightY);
        }
    } else {
        drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX, pBottomRightY);
    }
    // Draw the new projectile position
    if (((PROJECTILE*)getData(projectile))->projectileType == PROJECTILE_TYPE::MISSILE) {
        if (((PROJECTILE*)getData(projectile))->projectileDirection->dx == 0) { //straight
            drawImg(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, 3, 7, "Y0YORO0R0RRRWRW5W5050");
        } else if (((PROJECTILE*)getData(projectile))->projectileDirection->dx < 0) { // left
            drawImg(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, 6, 7, "0000000000Y0000O0Y00RRO00WRR005WW000550000");
        } else if (((PROJECTILE*)getData(projectile))->projectileDirection->dx > 0) { // right
            drawImg(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, 6, 7, "0000000Y0000Y0O0000ORR00000RW0000WW5000055");
        }
    } else {
        drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, 'R');
    }
}

void eraseEnemyProjectile(LLNode* projectile) {
    if (((PROJECTILE*)getData(projectile))->projectileType == PROJECTILE_TYPE::MISSILE) {
        if ((int)((PROJECTILE*)getData(projectile))->projectileDirection->pdx < 0 || (int)((PROJECTILE*)getData(projectile))->projectileDirection->dx < 0) { // Was or is going left
            drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x + 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
        } else if ((int)((PROJECTILE*)getData(projectile))->projectileDirection->pdx > 0 || (int)((PROJECTILE*)getData(projectile))->projectileDirection->dx > 0) { // Was or is going right
            drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
        } else { // Is going straight
            drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x , 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
        }
    } else {
        drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
    }
}

void drawBossProjectile(LLNode* projectile) {
    // Revert old pixels to background
    int pTopLeftX = ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px);
    int pBottomRightX = ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x - (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px);
    int pTopLeftY = 127 - (((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    int pBottomRightY = 127 - (((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    if (((PROJECTILE*)getData(projectile))->x > ((PROJECTILE*)getData(projectile))->px) { // Moved right
        drawBackgroundBox(pTopLeftX, pTopLeftY, pTopLeftX + (((PROJECTILE*)getData(projectile))->x - ((PROJECTILE*)getData(projectile))->px), pBottomRightY);
    } else if (((PROJECTILE*)getData(projectile))->x < ((PROJECTILE*)getData(projectile))->px) { // Moved left
        drawBackgroundBox(pBottomRightX - (((PROJECTILE*)getData(projectile))->px - ((PROJECTILE*)getData(projectile))->x), pTopLeftY, pBottomRightX, pBottomRightY);
    } else if (((PROJECTILE*)getData(projectile))->y > ((PROJECTILE*)getData(projectile))->py) { // Moved up
        drawBackgroundBox(pTopLeftX, pBottomRightY + (((PROJECTILE*)getData(projectile))->py - ((PROJECTILE*)getData(projectile))->y), pBottomRightX, pBottomRightY);
    } else if (((PROJECTILE*)getData(projectile))->y < ((PROJECTILE*)getData(projectile))->py) { // Moved down
        drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((PROJECTILE*)getData(projectile))->y - ((PROJECTILE*)getData(projectile))->py));
    }
    // Draw the new projectile position
    if (((PROJECTILE*)getData(projectile))->projectileType == PROJECTILE_TYPE::MISSILE) {
        if (((PROJECTILE*)getData(projectile))->projectileDirection->dx == 0) { //straight
            drawImg(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, 3, 7, "Y0YORO0R0RRRWRW5W5050");
        } else if (((PROJECTILE*)getData(projectile))->projectileDirection->dx < 0) { // left
            drawImg(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, 6, 7, "0000000000Y0000O0Y00RRO00WRR005WW000550000");
        } else if (((PROJECTILE*)getData(projectile))->projectileDirection->dx > 0) { // right
            drawImg(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, 6, 7, "0000000Y0000Y0O0000ORR00000RW0000WW5000055");
        }
    } else {
        drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, 'R');
    }
}

void eraseBossProjectile(LLNode* projectile) {
    if (((PROJECTILE*)getData(projectile))->projectileType == PROJECTILE_TYPE::MISSILE) {
        if ((int)((PROJECTILE*)getData(projectile))->projectileDirection->pdx < 0 || (int)((PROJECTILE*)getData(projectile))->projectileDirection->dx < 0) { // Was or is going left
            drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x + 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
        } else if ((int)((PROJECTILE*)getData(projectile))->projectileDirection->pdx > 0 || (int)((PROJECTILE*)getData(projectile))->projectileDirection->dx > 0) { // Was or is going right
            drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
        } else { // Is going straight
            drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x , 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
        }
    } else {
        drawBackgroundBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y);
    }
}

void drawHealthBar(LLNode* healthBar) {
    // Revert old pixels to background
    int pTopLeftX = ((BAR*)getData(healthBar))->boundingBox->topLeft.x - (((BAR*)getData(healthBar))->x - ((BAR*)getData(healthBar))->px);
    int pBottomRightX = ((BAR*)getData(healthBar))->boundingBox->bottomRight.x - (((BAR*)getData(healthBar))->x - ((BAR*)getData(healthBar))->px);
    int pTopLeftY = 127 - (((BAR*)getData(healthBar))->boundingBox->topLeft.y - (((BAR*)getData(healthBar))->y - ((BAR*)getData(healthBar))->py));
    int pBottomRightY = 127 - (((BAR*)getData(healthBar))->boundingBox->bottomRight.y - (((BAR*)getData(healthBar))->y - ((BAR*)getData(healthBar))->py));
    if (((BAR*)getData(healthBar))->x > ((BAR*)getData(healthBar))->px) { // Moved right
        drawBackgroundBox(pTopLeftX, pTopLeftY, pTopLeftX + (((BAR*)getData(healthBar))->x - ((BAR*)getData(healthBar))->px), pBottomRightY);
    } else if (((BAR*)getData(healthBar))->x < ((BAR*)getData(healthBar))->px) { // Moved left
        drawBackgroundBox(pBottomRightX - (((BAR*)getData(healthBar))->px - ((BAR*)getData(healthBar))->x), pTopLeftY, pBottomRightX, pBottomRightY);
    } else if (((BAR*)getData(healthBar))->y > ((BAR*)getData(healthBar))->py) { // Moved up
        drawBackgroundBox(pTopLeftX, pBottomRightY + (((BAR*)getData(healthBar))->py - ((BAR*)getData(healthBar))->y), pBottomRightX, pBottomRightY - 2);
    } else if (((BAR*)getData(healthBar))->y < ((BAR*)getData(healthBar))->py) { // Moved down
        drawBackgroundBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((BAR*)getData(healthBar))->y - ((BAR*)getData(healthBar))->py));
    }
    // Draw the new healthbar position
    // Need to account for numHearts
    drawBox(((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2, 127 - ((BAR*)getData(healthBar))->y, ((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2 + (BAR_WIDTH - 1), 127 - ((BAR*)getData(healthBar))->y - 1, '4');
    if (((BAR*)getData(healthBar))->numHearts == 0) {
        eraseHealthBar(healthBar);
    } else if ((double)((BAR*)getData(healthBar))->numHearts / ((BAR*)getData(healthBar))->barType > 0.75) {
        drawBox(((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2, 127 - ((BAR*)getData(healthBar))->y, ((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2 + (int)round((BAR_WIDTH - 1) * (double)((BAR*)getData(healthBar))->numHearts / ((BAR*)getData(healthBar))->barType), 127 - ((BAR*)getData(healthBar))->y - 1, 'G');
    } else if ((double)((BAR*)getData(healthBar))->numHearts / ((BAR*)getData(healthBar))->barType > 0.50) {
        drawBox(((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2, 127 - ((BAR*)getData(healthBar))->y, ((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2 + (int)round((BAR_WIDTH - 1) * (double)((BAR*)getData(healthBar))->numHearts / ((BAR*)getData(healthBar))->barType), 127 - ((BAR*)getData(healthBar))->y - 1, 'Y');
    } else if ((double)((BAR*)getData(healthBar))->numHearts / ((BAR*)getData(healthBar))->barType > 0.25) {
        drawBox(((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2, 127 - ((BAR*)getData(healthBar))->y, ((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2 + (int)round((BAR_WIDTH - 1) * (double)((BAR*)getData(healthBar))->numHearts / ((BAR*)getData(healthBar))->barType), 127 - ((BAR*)getData(healthBar))->y - 1, 'O');
    } else {
        drawBox(((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2, 127 - ((BAR*)getData(healthBar))->y, ((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2 + (int)round((BAR_WIDTH - 1) * (double)((BAR*)getData(healthBar))->numHearts / ((BAR*)getData(healthBar))->barType), 127 - ((BAR*)getData(healthBar))->y - 1, 'R');
    }
}

void eraseHealthBar(LLNode* healthBar) {
    drawBackgroundBox(((BAR*)getData(healthBar))->boundingBox->topLeft.x, 127 - ((BAR*)getData(healthBar))->boundingBox->topLeft.y, ((BAR*)getData(healthBar))->boundingBox->bottomRight.x, 127 - ((BAR*)getData(healthBar))->boundingBox->bottomRight.y);
}

void drawSuperAttackBar(LLNode* player) {
    if (((PLAYER*)getData(player))->superChargeStatus == 0) {
        drawBox(0, 127 - 5, 127, 127 - 3, '4');
    } else {
        drawBox(0, 127 - 5, 127, 127 - 3, '4');
        drawBox(0, 127 - 5, (int)round(127 * (double)((PLAYER*)getData(player))->superChargeStatus / MAX_SUPER_CHARGE), 127 - 3, 'Y');
    }
}

void eraseSuperAttackBar(LLNode* player) {
    drawBox(0, 127 - 7, 127, 127 - 3, '0');
}

void drawSSButton(MENU_SETTINGS* menuSettings, BUTTON* skinSelector) {
    if (skinSelector->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(skinSelector->boundingBox->topLeft.x, skinSelector->boundingBox->topLeft.y, skinSelector->boundingBox->bottomRight.x, skinSelector->boundingBox->bottomRight.y, '5');
        uLCD.circle(18, 127 - 18, 9, WHITE);
        uLCD.rectangle(skinSelector->boundingBox->topLeft.x, skinSelector->boundingBox->topLeft.y, skinSelector->boundingBox->bottomRight.x, skinSelector->boundingBox->bottomRight.y, WHITE);
    } else {
        drawBox(skinSelector->boundingBox->topLeft.x, skinSelector->boundingBox->topLeft.y, skinSelector->boundingBox->bottomRight.x, skinSelector->boundingBox->bottomRight.y, '3');
        uLCD.circle(18, 127 - 18, 9, MGREY);
        uLCD.rectangle(skinSelector->boundingBox->topLeft.x, skinSelector->boundingBox->topLeft.y, skinSelector->boundingBox->bottomRight.x, skinSelector->boundingBox->bottomRight.y, MGREY);
    }
    switch (menuSettings->playerSkin) {
    case 'R': uLCD.filled_circle(18, 127 - 18, 8, RED); break;
    case 'O': uLCD.filled_circle(18, 127 - 18, 8, ORANGE); break;
    case 'Y': uLCD.filled_circle(18, 127 - 18, 8, YELLOW); break;
    case 'G': uLCD.filled_circle(18, 127 - 18, 8, GREEN); break;
    case 'B': uLCD.filled_circle(18, 127 - 18, 8, BLUE); break;
    case 'V': uLCD.filled_circle(18, 127 - 18, 8, VIOLET); break;
    case 'P': uLCD.filled_circle(18, 127 - 18, 8, PINK); break;
    }
    drawImg(64 - 5, 127 - 68, 11, 11, PLAYER_IMGS[3]);
}

void drawMSButton(MENU_SETTINGS* menuSettings, BUTTON* modeSelector) {
    int color;
    int textBackground;
    if (modeSelector->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(modeSelector->boundingBox->topLeft.x, modeSelector->boundingBox->topLeft.y, modeSelector->boundingBox->bottomRight.x, modeSelector->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(modeSelector->boundingBox->topLeft.x, modeSelector->boundingBox->topLeft.y, modeSelector->boundingBox->bottomRight.x, modeSelector->boundingBox->bottomRight.y, WHITE);
        color = RED;
        textBackground = LGREY;
    } else {
        drawBox(modeSelector->boundingBox->topLeft.x, modeSelector->boundingBox->topLeft.y, modeSelector->boundingBox->bottomRight.x, modeSelector->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(modeSelector->boundingBox->topLeft.x, modeSelector->boundingBox->topLeft.y, modeSelector->boundingBox->bottomRight.x, modeSelector->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    if (menuSettings->gameMode == GAME_MODE::INFINITE) {
        char message[] = "INF";
        int i = 0;
        while (message[i]) {
            loadMusic();
            uLCD.text_bold(ON);
            uLCD.set_font(FONT_7X8);
            uLCD.locate(14 + i, 13);
            uLCD.color(color);
            uLCD.textbackground_color(textBackground);
            uLCD.printf("%c", message[i]);
            i++;
        }
    } else if (menuSettings->gameMode == GAME_MODE::LEVELS) {
        char message[] = "123";
        int i = 0;
        while (message[i]) {
            loadMusic();
            uLCD.text_bold(ON);
            uLCD.set_font(FONT_7X8);
            uLCD.locate(14 + i, 13);
            uLCD.color(color);
            uLCD.textbackground_color(textBackground);
            uLCD.printf("%c", message[i]);
            i++;
        }
    } else if (menuSettings->gameMode == GAME_MODE::SCORECAP) {
        uLCD.set_font(FONT_7X8);
        uLCD.locate(14, 12);
        uLCD.color(color);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("___");
        char message[5];
        sprintf(message, "%03d", menuSettings->scoreCap);
        int i = 0;
        while (message[i]) {
            loadMusic();
            uLCD.text_bold(ON);
            uLCD.set_font(FONT_7X8);
            uLCD.locate(14 + i, 13);
            uLCD.color(color);
            uLCD.textbackground_color(textBackground);
            uLCD.printf("%c", message[i]);
            i++;
        }
    } else if (menuSettings->gameMode == GAME_MODE::PVP) {
        char message[] = "PVP";
        int i = 0;
        while (message[i]) {
            loadMusic();
            uLCD.text_bold(ON);
            uLCD.set_font(FONT_7X8);
            uLCD.locate(14 + i, 13);
            uLCD.color(color);
            uLCD.textbackground_color(textBackground);
            uLCD.printf("%c", message[i]);
            i++;
        }
    }
}

void drawPaused(void) {
    drawBox(64 - 10, 64 - 10, 64 - 5, 64 + 10, 'W');
    drawBox(64 + 5, 64 - 10, 64 + 10, 64 + 10, 'W');
}

void erasePaused(void) {
    drawBackgroundBox(64 - 10, 64 - 10, 64 - 5, 64 + 10);
    drawBackgroundBox(64 + 5, 64 - 10, 64 + 10, 64 + 10);
}

void drawResumed(void) {
    for (int i = 0; i <= 10; i++) {
        uLCD.triangle(64 - i, 64 - i, 64 - i, 64 + i, 64 + i, 64, WHITE);
    }
}

void eraseResumed(void) {
    drawBackgroundBox(64 - 10, 64 - 10, 64 + 10, 64 + 10);
}

void drawPlayButton(MENU_SETTINGS* menuSettings, BUTTON* play) {
    int color;
    int textBackground;
    if (play->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(play->boundingBox->topLeft.x, play->boundingBox->topLeft.y, play->boundingBox->bottomRight.x, play->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(play->boundingBox->topLeft.x, play->boundingBox->topLeft.y, play->boundingBox->bottomRight.x, play->boundingBox->bottomRight.y, WHITE);
        color = GREEN;
        textBackground = LGREY;
    } else {
        drawBox(play->boundingBox->topLeft.x, play->boundingBox->topLeft.y, play->boundingBox->bottomRight.x, play->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(play->boundingBox->topLeft.x, play->boundingBox->topLeft.y, play->boundingBox->bottomRight.x, play->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "PLAY";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(7 + i, 13);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

void drawDiffScale(MENU_SETTINGS* menuSettings, SLIDING_SCALE* difficulty) {
    drawBackgroundBox(0, difficulty->boundingBox->topLeft.y - 1, 127, difficulty->boundingBox->bottomRight.y + 1);
    drawBox(0, difficulty->boundingBox->topLeft.y, 127, difficulty->boundingBox->bottomRight.y, '3');
    drawBackgroundBox(39 - 5, 127 - 116, 89 - 5 + 11, 127 - 76);
    if (menuSettings->gameMode != GAME_MODE::PVP) {
        if (difficulty->value == difficulty->maxVal) {
            drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'R');
            drawImg(64 - 9, 127 - 116, 19, 19, BOSS_IMGS[3]);
            drawImg(39 - 5, 127 - 101, 11, 11, MISSILE_ENEMY_IMGS[3]);
            drawImg(89 - 5, 127 - 101, 11, 11, MISSILE_ENEMY_IMGS[3]);
            drawImg(64 - 5, 127 - 88, 11, 11, NORMAL_ENEMY_IMGS[3]);
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.75) {
            drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'O');
            drawImg(39 - 5, 127 - 101, 11, 11, MISSILE_ENEMY_IMGS[3]);
            drawImg(89 - 5, 127 - 101, 11, 11, MISSILE_ENEMY_IMGS[3]);
            drawImg(64 - 5, 127 - 88, 11, 11, NORMAL_ENEMY_IMGS[3]);
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.50) {
            drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'Y');
            drawImg(46 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
            drawImg(82 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.25) {
            drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'G');
            drawImg(64 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
        }
        if (difficulty->sliderStatus == BUTTON_STATUS::SELECTED) {
            uLCD.filled_circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, LGREY);
            uLCD.circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, WHITE);
        } else {
            uLCD.filled_circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, DGREY);
            uLCD.circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, MGREY);
        }
    } else { // Draw the opponent if the gamemode is pvp
        if (difficulty->value == difficulty->maxVal) {
            drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'R');
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.75) {
            drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'O');
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.50) {
            drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'Y');
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.25) {
            drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'G');
        }
        if (difficulty->sliderStatus == BUTTON_STATUS::SELECTED) {
            uLCD.filled_circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, LGREY);
            uLCD.circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, WHITE);
        } else {
            uLCD.filled_circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, DGREY);
            uLCD.circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, MGREY);
        }
        drawImgOpponent(64 - 5, 127 - 100, 11, 11, PLAYER_IMGS[3]);
    }
}

void drawUserStatsButton(USER* user, BUTTON* userStats) {
    int color;
    int textBackground;
    if (userStats->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(userStats->boundingBox->topLeft.x, userStats->boundingBox->topLeft.y, userStats->boundingBox->bottomRight.x, userStats->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(userStats->boundingBox->topLeft.x, userStats->boundingBox->topLeft.y, userStats->boundingBox->bottomRight.x, userStats->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(userStats->boundingBox->topLeft.x, userStats->boundingBox->topLeft.y, userStats->boundingBox->bottomRight.x, userStats->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(userStats->boundingBox->topLeft.x, userStats->boundingBox->topLeft.y, userStats->boundingBox->bottomRight.x, userStats->boundingBox->bottomRight.y, MGREY);
        color = WHITE;
        textBackground = DGREY;
    }
    char message[20];
    strcpy(message, user->username);
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(message, strUserNum);
    int nameLength = 0;
    int i = 0;
    while (message[nameLength]) {
        nameLength++;
    }
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate((9 - (int)round((double)nameLength / 2)) + i, 0);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

void drawDeleteUserButton(BUTTON* deleteProfile) {
    int color;
    int textBackground;
    if (deleteProfile->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(deleteProfile->boundingBox->topLeft.x, deleteProfile->boundingBox->topLeft.y, deleteProfile->boundingBox->bottomRight.x, deleteProfile->boundingBox->bottomRight.y, 'R');
        uLCD.rectangle(deleteProfile->boundingBox->topLeft.x, deleteProfile->boundingBox->topLeft.y, deleteProfile->boundingBox->bottomRight.x, deleteProfile->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = RED;
    } else {
        drawBox(deleteProfile->boundingBox->topLeft.x, deleteProfile->boundingBox->topLeft.y, deleteProfile->boundingBox->bottomRight.x, deleteProfile->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(deleteProfile->boundingBox->topLeft.x, deleteProfile->boundingBox->topLeft.y, deleteProfile->boundingBox->bottomRight.x, deleteProfile->boundingBox->bottomRight.y, MGREY);
        color = RED;
        textBackground = DGREY;
    }
    char message[20] = "DELETE PROFILE";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(2 + i, 14);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

void drawLoginBackground(void) {
    // Sun
    uLCD.filled_circle(127, 0, 30, YELLOW);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 29, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 0, 15, LGREY);
    uLCD.filled_circle(8, 12, 1, DGREY);
    uLCD.filled_circle(0, 9, 3, MGREY);
    uLCD.filled_circle(-1, 5, 2, DGREY);
    uLCD.filled_circle(5, -1, 3, MGREY);
    uLCD.filled_circle(9, 2, 2, DGREY);
    drawImg(64 - 9, 127 - 113, 19, 19, BOSS_IMGS[3]);
    drawImg(39 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
    drawImg(89 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
    drawImg(64 - 5, 127 - 80, 11, 11, PLAYER_IMGS[3]);
    char message[30] = "STAR WARS";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(WHITE);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(4 + i, 0);
        uLCD.textbackground_color(BLACK);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

void drawRegistrationBackground(void) {
    // Sun
    uLCD.filled_circle(127, 20, 30, YELLOW);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 29, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 32, 15, LGREY);
    uLCD.filled_circle(8, 44, 1, DGREY);
    uLCD.filled_circle(0, 41, 3, MGREY);
    uLCD.filled_circle(-1, 37, 2, DGREY);
    uLCD.filled_circle(5, 31, 3, MGREY);
    uLCD.filled_circle(9, 34, 2, DGREY);
    // Earth
    uLCD.filled_circle(127, 115, 25, BLUE);
    uLCD.filled_circle(120, 105, 9, GREEN);
    uLCD.filled_circle(110, 115, 4, GREEN);
    uLCD.filled_circle(112, 110, 3, GREEN);
    uLCD.filled_circle(108, 110, 2, DGREEN);
    uLCD.filled_circle(120, 102, 3, DGREEN);
    uLCD.filled_circle(126, 108, 5, DGREEN);
    // Mars
    uLCD.filled_circle(0, 110, 15, ORANGE);
    uLCD.filled_circle(0, 100, 3, BROWN);
    uLCD.filled_circle(-1, 104, 2, MGREY);
    uLCD.filled_circle(7, 111, 4, BROWN);
    uLCD.filled_circle(11, 107, 2, MGREY);
    drawImg(64 - 9, 127 - 113, 19, 19, BOSS_IMGS[3]);
    drawImg(39 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
    drawImg(89 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
    drawImg(64 - 5, 127 - 80, 11, 11, PLAYER_IMGS[3]);
}

void drawRegistrationSkinSelectionBackground(void) {
    // Sun
    uLCD.filled_circle(127, 20, 30, YELLOW);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 29, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 32, 15, LGREY);
    uLCD.filled_circle(8, 44, 1, DGREY);
    uLCD.filled_circle(0, 41, 3, MGREY);
    uLCD.filled_circle(-1, 37, 2, DGREY);
    uLCD.filled_circle(5, 31, 3, MGREY);
    uLCD.filled_circle(9, 34, 2, DGREY);
    // Earth
    uLCD.filled_circle(127, 115, 25, BLUE);
    uLCD.filled_circle(120, 105, 9, GREEN);
    uLCD.filled_circle(110, 115, 4, GREEN);
    uLCD.filled_circle(112, 110, 3, GREEN);
    uLCD.filled_circle(108, 110, 2, DGREEN);
    uLCD.filled_circle(120, 102, 3, DGREEN);
    uLCD.filled_circle(126, 108, 5, DGREEN);
    // Mars
    uLCD.filled_circle(0, 110, 15, ORANGE);
    uLCD.filled_circle(0, 100, 3, BROWN);
    uLCD.filled_circle(-1, 104, 2, MGREY);
    uLCD.filled_circle(7, 111, 4, BROWN);
    uLCD.filled_circle(11, 107, 2, MGREY);
}

void drawMenuBackground(void) {
    // Sun
    uLCD.filled_circle(127, 0, 30, YELLOW);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 29, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 7, 15, LGREY);
    uLCD.filled_circle(8, 19, 1, DGREY);
    uLCD.filled_circle(0, 16, 3, MGREY);
    uLCD.filled_circle(-1, 12, 2, DGREY);
    uLCD.filled_circle(5, 6, 3, MGREY);
    uLCD.filled_circle(9, 9, 2, DGREY);
    // Earth
    uLCD.filled_circle(127, 70, 25, BLUE);
    uLCD.filled_circle(120, 60, 9, GREEN);
    uLCD.filled_circle(110, 70, 4, GREEN);
    uLCD.filled_circle(112, 65, 3, GREEN);
    uLCD.filled_circle(108, 65, 2, DGREEN);
    uLCD.filled_circle(120, 57, 3, DGREEN);
    uLCD.filled_circle(126, 63, 5, DGREEN);
    // Mars
    uLCD.filled_circle(0, 70, 15, ORANGE);
    uLCD.filled_circle(0, 60, 3, BROWN);
    uLCD.filled_circle(-1, 64, 2, MGREY);
    uLCD.filled_circle(7, 71, 4, BROWN);
    uLCD.filled_circle(11, 67, 2, MGREY);
}

void drawUserStatsBackground(USER* user) {
    // Sun
    uLCD.filled_circle(127, 0, 30, YELLOW);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 29, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 7, 15, LGREY);
    uLCD.filled_circle(8, 19, 1, DGREY);
    uLCD.filled_circle(0, 16, 3, MGREY);
    uLCD.filled_circle(-1, 12, 2, DGREY);
    uLCD.filled_circle(5, 6, 3, MGREY);
    uLCD.filled_circle(9, 9, 2, DGREY);
    // Earth
    uLCD.filled_circle(127, 100, 25, BLUE);
    uLCD.filled_circle(120, 90, 9, GREEN);
    uLCD.filled_circle(110, 100, 4, GREEN);
    uLCD.filled_circle(112, 95, 3, GREEN);
    uLCD.filled_circle(108, 95, 2, DGREEN);
    uLCD.filled_circle(120, 87, 3, DGREEN);
    uLCD.filled_circle(126, 93, 5, DGREEN);
    // Mars
    uLCD.filled_circle(0, 100, 15, ORANGE);
    uLCD.filled_circle(0, 90, 3, BROWN);
    uLCD.filled_circle(-1, 94, 2, MGREY);
    uLCD.filled_circle(7, 101, 4, BROWN);
    uLCD.filled_circle(11, 97, 2, MGREY);
    // User stats
    // User highscore
    char hScoreStat[30] = "HI SCORE: ";
    char strHighScore[10];
    double highScore = (double)getSavedHighScore(user);
    if (highScore >= 1000) {
        highScore /= 1000;
        sprintf(strHighScore, "%.1fK", highScore);
    } else {
        sprintf(strHighScore, "%d", (int)highScore);
    }
    strcat(hScoreStat, strHighScore);
    int i = 0;
    while (hScoreStat[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(WHITE);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(2 + i, 3);
        uLCD.textbackground_color(BLACK);
        if (hScoreStat[i] == ' ') uLCD.locate(uLCD.current_col + 1, uLCD.current_row);
        else uLCD.printf("%c", hScoreStat[i]);
        i++;
    }
    // User enemies killed
    char eKilledStat[30] = "KILLS: ";
    char strEnemiesKilled[10];
    double totalKills = (double)getSavedEnemiesKilled(user);
    if (totalKills >= 1000) {
        totalKills /= 1000;
        sprintf(strEnemiesKilled, "%.1fK", totalKills);
    } else {
        sprintf(strEnemiesKilled, "%d", (int)totalKills);
    }
    strcat(eKilledStat, strEnemiesKilled);
    i = 0;
    while (eKilledStat[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(WHITE);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(2 + i, 5);
        uLCD.textbackground_color(BLACK);
        if (eKilledStat[i] == ' ') uLCD.locate(uLCD.current_col + 1, uLCD.current_row);
        else uLCD.printf("%c", eKilledStat[i]);
        i++;
    }
    // User player deaths
    char pDeathsStat[30] = "DEATHS: ";
    char strPlayerDeaths[10];
    double totalDeaths = (double)getSavedPlayerDeaths(user);
    if (totalDeaths >= 1000) {
        totalDeaths /= 1000;
        sprintf(strPlayerDeaths, "%.1fK", totalDeaths);
    } else {
        sprintf(strPlayerDeaths, "%d", (int)totalDeaths);
    }
    strcat(pDeathsStat, strPlayerDeaths);
    i = 0;
    while (pDeathsStat[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(WHITE);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(2 + i, 7);
        uLCD.textbackground_color(BLACK);
        if (pDeathsStat[i] == ' ') uLCD.locate(uLCD.current_col + 1, uLCD.current_row);
        else uLCD.printf("%c", pDeathsStat[i]);
        i++;
    }
    // User total points
    char tPointsStat[30] = "TOT PTS: ";
    char strTotalPoints[10];
    double totalPoints = (double)getSavedTotalPoints(user);
    if (totalPoints >= 1000) {
        totalPoints /= 1000;
        sprintf(strTotalPoints, "%.1fK", totalPoints);
    } else {
        sprintf(strTotalPoints, "%d", (int)totalPoints);
    }
    strcat(tPointsStat, strTotalPoints);
    i = 0;
    while (tPointsStat[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(WHITE);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(2 + i, 9);
        uLCD.textbackground_color(BLACK);
        if (tPointsStat[i] == ' ') uLCD.locate(uLCD.current_col + 1, uLCD.current_row);
        else uLCD.printf("%c", tPointsStat[i]);
        i++;
    }
    // User total play time
    char tTimeStat[30] = "PLY TME: ";
    char strHours[10];
    char strMins[10];
    int hours, minutes, seconds;
    seconds = getSavedPlayTime(user);
    hours = seconds / 3600;
    seconds = seconds % 3600;
    minutes = seconds / 60;
    sprintf(strHours, "%dH ", hours);
    sprintf(strMins, "%dM", minutes);
    strcat(tTimeStat, strHours);
    strcat(tTimeStat, strMins);
    i = 0;
    while (tTimeStat[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(WHITE);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(2 + i, 11);
        uLCD.textbackground_color(BLACK);
        if (tTimeStat[i] == ' ') uLCD.locate(uLCD.current_col + 1, uLCD.current_row);
        else uLCD.printf("%c", tTimeStat[i]);
        i++;
    }
}

/**
 * Draws login button
 */
void drawLoginButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "LOGIN";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(6 + i, 12);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

void drawInvalidLoginButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = RED;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "LOGIN";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(6 + i, 12);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws register button
 */
void drawRegisterButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "REGISTER";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(5 + i, 14);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws username base button
 */
void drawUserBaseButton(USER* userInfo, BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    int nameLength = 0;
    int i = 0;
    while (userInfo->username[nameLength]) {
        nameLength++;
    }
    while (userInfo->username[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate((7 - (int)round((double)nameLength / 2)) + i, 10);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", userInfo->username[i]);
        i++;
    }
}

/**
 * Draws username number button
 */
void drawUserNumButton(USER* userInfo, BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char strNum[3];
    snprintf(strNum, 3, "%02d", userInfo->userNum);
    int i = 0;
    while (strNum[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        if (strNum[1]) uLCD.locate(15 + i, 10);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", strNum[i]);
        i++;
    }
}

/**
 * Draws back button
 */
void drawBackButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = RED;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "<";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(1 + i, 1);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws next button
 */
void drawNextButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = GREEN;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = ">";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(16 + i, 1);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws invalid next button
 */
void drawInvalidNextButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = RED;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = ">";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(16 + i, 1);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws delete button
 */
void drawDeleteButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = RED;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "X";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_7X8);
        uLCD.locate(12 + i, 14);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws zero button
 */
void drawZeroButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "0";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(8 + i, 14);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws one button
 */
void drawOneButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "1";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(5 + i, 11);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws two button
 */
void drawTwoButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "2";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(8 + i, 11);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws three button
 */
void drawThreeButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "3";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(12 + i, 11);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws four button
 */
void drawFourButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "4";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(5 + i, 8);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws five button
 */
void drawFiveButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "5";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(8 + i, 8);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws six button
 */
void drawSixButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "6";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(12 + i, 8);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws seven button
 */
void drawSevenButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "7";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(5 + i, 5);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws eight button
 */
void drawEightButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "8";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(8 + i, 5);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws nine button
 */
void drawNineButton(BUTTON* button) {
    int color;
    int textBackground;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
        textBackground = LGREY;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
        textBackground = DGREY;
    }
    char message[] = "9";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(12 + i, 5);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws user default skin button
 */
void drawDefaultSkinButton(USER* userInfo, BUTTON* button) {
    int color;
    int textBackground;
    char skinType[20];
    switch (userInfo->defaultSkin) {
    case 'R': uLCD.filled_circle(64, 127 - 18, 8, RED); strcpy(skinType, "FIRE RED"); color = RED; break;
    case 'O': uLCD.filled_circle(64, 127 - 18, 8, ORANGE); strcpy(skinType, "BOLD ORANGE"); color = ORANGE; break;
    case 'Y': uLCD.filled_circle(64, 127 - 18, 8, YELLOW); strcpy(skinType, "STAR GOLD"); color = YELLOW; break;
    case 'G': uLCD.filled_circle(64, 127 - 18, 8, GREEN); strcpy(skinType, "EMERALD GREEN"); color = GREEN; break;
    case 'B': uLCD.filled_circle(64, 127 - 18, 8, BLUE); strcpy(skinType, "ICE BLUE"); color = BLUE; break;
    case 'V': uLCD.filled_circle(64, 127 - 18, 8, VIOLET); strcpy(skinType, "PLASMA VIOLET"); color = VIOLET; break;
    case 'P': uLCD.filled_circle(64, 127 - 18, 8, PINK); strcpy(skinType, "HOT PINK"); color = PINK; break;
    }

    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        textBackground = LGREY;
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.circle(64, 127 - 18, 9, WHITE);
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
    } else {
        textBackground = DGREY;
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.circle(64, 127 - 18, 9, MGREY);
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }

    int skinTypeNameLength = 0;
    while (skinType[skinTypeNameLength]) {
        skinTypeNameLength++;
    }
    int i = 0;
    while (skinType[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(9 + i - (int)round((double)skinTypeNameLength / 2), 10);
        uLCD.textbackground_color(textBackground);
        uLCD.printf("%c", skinType[i]);
        i++;
    }
    drawImg(64 - 5, 127 - 80, 11, 11, PLAYER_IMGS[3]);
}

/**
 * Draws password
 */
void drawPassword(char password[5]) {
    uLCD.filled_rectangle(26, 0, 127 - 26, 0 + 25, BLACK);
    int pwLength = 0;
    while (password[pwLength]) {
        pwLength++;
    }
    int i = 0;
    while (password[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.textbackground_color(BLACK);
        uLCD.color(GREEN);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(9 + i - (int)round((double)pwLength / 2), 1);
        uLCD.printf("%c", password[i]);
        i++;
    }
}

/**
 * Draws invalid password
 */
void drawInvalidPassword(char password[5]) {
    uLCD.filled_rectangle(26, 0, 127 - 26, 0 + 25, BLACK);
    int pwLength = 0;
    while (password[pwLength]) {
        pwLength++;
    }
    int i = 0;
    while (password[i]) {
        uLCD.text_bold(ON);
        uLCD.textbackground_color(BLACK);
        uLCD.color(RED);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(9 + i - (int)round((double)pwLength / 2), 1);
        uLCD.printf("%c", password[i]);
        i++;
    }
}

void drawGameWon(void) {
    if (getMenuSettings()->gameMode == GAME_MODE::LEVELS && getGameLoop()->level < 3) {
        uLCD.cls();
        char message[30] = "LEVEL ";
        char strLevel[10];
        sprintf(strLevel, "%d PASSED", getGameLoop()->level);
        strcat(message, strLevel);
        int msgLength = 0;
        while (message[msgLength]) {
            msgLength++;
        }
        int i = 0;
        while (message[i]) {
            loadMusic();
            uLCD.text_bold(ON);
            uLCD.color(WHITE);
            uLCD.set_font(FONT_8X8);
            uLCD.locate(9 + i - (int)round((double)msgLength / 2), 5);
            uLCD.textbackground_color(BLACK);
            uLCD.printf("%c", message[i]);
            i++;
        }
    } else {
        drawGameBackground(false);
        // Sun
        uLCD.filled_circle(127, 0, 30, YELLOW);
        uLCD.circle(127, 0, 30, WHITE);
        uLCD.circle(127, 0, 29, WHITE);
        uLCD.circle(127, 0, 28, WHITE);
        // Moon
        uLCD.filled_circle(0, 7, 15, LGREY);
        uLCD.filled_circle(8, 19, 1, DGREY);
        uLCD.filled_circle(0, 16, 3, MGREY);
        uLCD.filled_circle(-1, 12, 2, DGREY);
        uLCD.filled_circle(5, 6, 3, MGREY);
        uLCD.filled_circle(9, 9, 2, DGREY);
        // Earth
        uLCD.filled_circle(127, 110, 25, BLUE);
        uLCD.filled_circle(120, 100, 9, GREEN);
        uLCD.filled_circle(110, 110, 4, GREEN);
        uLCD.filled_circle(112, 105, 3, GREEN);
        uLCD.filled_circle(108, 105, 2, DGREEN);
        uLCD.filled_circle(120, 97, 3, DGREEN);
        uLCD.filled_circle(126, 103, 5, DGREEN);
        // Mars
        uLCD.filled_circle(0, 105, 15, ORANGE);
        uLCD.filled_circle(0, 95, 3, BROWN);
        uLCD.filled_circle(-1, 99, 2, MGREY);
        uLCD.filled_circle(7, 106, 4, BROWN);
        uLCD.filled_circle(11, 112, 2, MGREY);

        char message[30] = "YOU WON!";
        int i = 0;
        while (message[i]) {
            loadMusic();
            uLCD.text_bold(ON);
            uLCD.color(WHITE);
            uLCD.set_font(FONT_8X8);
            uLCD.locate(5 + i, 5);
            uLCD.textbackground_color(BLACK);
            if (message[i] == ' ') uLCD.locate(uLCD.current_col + 1, uLCD.current_row);
            else uLCD.printf("%c", message[i]);
            i++;
        }
        drawImg(64 - 7, 127 - 60, 11, 11, PLAYER_IMGS[3]);
    }
}

void drawGameLost(void) {
    drawGameBackground(true);
    // Sun
    uLCD.filled_circle(127, 0, 30, RED);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 29, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 7, 15, RED);
    uLCD.filled_circle(8, 19, 1, DRED);
    uLCD.filled_circle(0, 16, 3, MRED);
    uLCD.filled_circle(-1, 12, 2, DRED);
    uLCD.filled_circle(5, 6, 3, MRED);
    uLCD.filled_circle(9, 9, 2, DRED);
    // Earth
    uLCD.filled_circle(127, 110, 25, RED);
    uLCD.filled_circle(120, 100, 9, MRED);
    uLCD.filled_circle(110, 110, 4, MRED);
    uLCD.filled_circle(112, 105, 3, MRED);
    uLCD.filled_circle(108, 105, 2, DRED);
    uLCD.filled_circle(120, 97, 3, DRED);
    uLCD.filled_circle(126, 103, 5, DRED);
    // Mars
    uLCD.filled_circle(0, 105, 15, RED);
    uLCD.filled_circle(0, 95, 3, DRED);
    uLCD.filled_circle(-1, 99, 2, MRED);
    uLCD.filled_circle(7, 106, 4, DRED);
    uLCD.filled_circle(11, 112, 2, MRED);

    drawImg(64 - 9, 127 - 105, 19, 19, BOSS_IMGS[3]);
    drawImg(39 - 5, 127 - 85, 11, 11, MISSILE_ENEMY_IMGS[3]);
    drawImg(89 - 5, 127 - 85, 11, 11, MISSILE_ENEMY_IMGS[3]);
    drawImg(64 - 5, 127 - 68, 11, 11, NORMAL_ENEMY_IMGS[3]);
    char game[30] = "GAME";
    int i = 0;
    while (game[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(RED);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(3 + i, 8);
        uLCD.textbackground_color(BLACK);
        uLCD.printf("%c", game[i]);
        i++;
    }
    char over[30] = "OVER";
    i = 0;
    while (game[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(RED);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(11 + i, 8);
        uLCD.textbackground_color(BLACK);
        uLCD.printf("%c", over[i]);
        i++;
    }
}
