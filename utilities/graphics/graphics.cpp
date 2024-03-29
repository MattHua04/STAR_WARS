#include "graphics.h"
#include "globals.h"
#include "doublyLinkedList.h"
#include "login.h"
#include "menu.h"
#include "projectile.h"
#include "player.h"
#include "enemy.h"
#include "finalBoss.h"
#include "game.h"
#include "registration.h"
#include "uLCD.hpp"
#include "users.h"

class uLCD lcd(p13, p14, p15, uLCD::B_1500000);

int getHexColor(char color) {
    if (color == 'X' && getMenuSettings()->playerSkin) color = getMenuSettings()->playerSkin;
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

void drawProfileImg(void) {
    int chunckWidth = 32;
    for (int i = 0; i < 128; i += chunckWidth) {
        int chunk[128 * chunckWidth];
        for (int j = 0; j < chunckWidth; j++) {
            for (int k = 0; k < 128; k++) {
                chunk[j * 128 + k] = profileImg[i * 128 + j * 128 + k];
            }
        }
        uLCD.BLIT(0, i, 128, chunckWidth, chunk);
    }
}

void drawImg(int x, int y, int width, int height, const char* object) {
    int img[width*height];
    for (int i = 0; i < width*height; i++) {
        img[i] = (getHexColor(object[i])); //uLCD::get4DGLColor
    }
    uLCD.BLIT(x, y, width, height, img);
    //lcd.BLIT(x, y, width, height, img, false);
}

void drawBox(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, char color) {
    //lcd.drawRectangleFilled(topLeftX, topLeftY, bottomRightX, bottomRightY, uLCD::get4DGLColor(getHexColor(color)));
    uLCD.filled_rectangle(topLeftX, topLeftY, bottomRightX, bottomRightY, getHexColor(color));
}

void drawPlayer(LLNode* player) {
    int pTopLeftX = ((PLAYER*)getData(player))->boundingBox->topLeft.x - (((PLAYER*)getData(player))->x - ((PLAYER*)getData(player))->px);
    int pBottomRightX = ((PLAYER*)getData(player))->boundingBox->bottomRight.x - (((PLAYER*)getData(player))->x - ((PLAYER*)getData(player))->px);
    int pTopLeftY = 127 - (((PLAYER*)getData(player))->boundingBox->topLeft.y - (((PLAYER*)getData(player))->y - ((PLAYER*)getData(player))->py));
    int pBottomRightY = 127 - (((PLAYER*)getData(player))->boundingBox->bottomRight.y - (((PLAYER*)getData(player))->y - ((PLAYER*)getData(player))->py));
    if (((PLAYER*)getData(player))->x > ((PLAYER*)getData(player))->px) { //Moved right
        drawBox(pTopLeftX, pTopLeftY, pTopLeftX + (((PLAYER*)getData(player))->x - ((PLAYER*)getData(player))->px), pBottomRightY, '0');
    } else if (((PLAYER*)getData(player))->x < ((PLAYER*)getData(player))->px) { //Moved left
        drawBox(pBottomRightX - (((PLAYER*)getData(player))->px - ((PLAYER*)getData(player))->x), pTopLeftY, pBottomRightX, pBottomRightY, '0');
    } else if (((PLAYER*)getData(player))->y > ((PLAYER*)getData(player))->py) { //Moved up
        drawBox(pTopLeftX, pBottomRightY + (((PLAYER*)getData(player))->py - ((PLAYER*)getData(player))->y), pBottomRightX, pBottomRightY, '0');
    } else if (((PLAYER*)getData(player))->y < ((PLAYER*)getData(player))->py) { //Moved down
        drawBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((PLAYER*)getData(player))->y - ((PLAYER*)getData(player))->py), '0');
    }
    drawImg(((PLAYER*)getData(player))->boundingBox->topLeft.x, 127 - ((PLAYER*)getData(player))->boundingBox->topLeft.y, 11, 11, PLAYER_IMGS[((PLAYER*)getData(player))->playerDisplay]);
}

void erasePlayer(LLNode* player) {
    drawBox(((PLAYER*)getData(player))->boundingBox->topLeft.x, 127 - ((PLAYER*)getData(player))->boundingBox->topLeft.y, ((PLAYER*)getData(player))->boundingBox->bottomRight.x, 127 - ((PLAYER*)getData(player))->boundingBox->bottomRight.y, '0');
}

void drawEnemy(LLNode* enemy) {
    int pTopLeftX = ((ENEMY*)getData(enemy))->boundingBox->topLeft.x - (((ENEMY*)getData(enemy))->x - ((ENEMY*)getData(enemy))->px);
    int pBottomRightX = ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x - (((ENEMY*)getData(enemy))->x - ((ENEMY*)getData(enemy))->px);
    int pTopLeftY = 127 - (((ENEMY*)getData(enemy))->boundingBox->topLeft.y - (((ENEMY*)getData(enemy))->y - ((ENEMY*)getData(enemy))->py));
    int pBottomRightY = 127 - (((ENEMY*)getData(enemy))->boundingBox->bottomRight.y - (((ENEMY*)getData(enemy))->y - ((ENEMY*)getData(enemy))->py));
    if (((ENEMY*)getData(enemy))->x > ((ENEMY*)getData(enemy))->px) { //Moved right
        drawBox(pTopLeftX - 1, pTopLeftY, pTopLeftX + (((ENEMY*)getData(enemy))->x - ((ENEMY*)getData(enemy))->px), pBottomRightY, '0');
    } else if (((ENEMY*)getData(enemy))->x < ((ENEMY*)getData(enemy))->px) { //Moved left
        drawBox(pBottomRightX - (((ENEMY*)getData(enemy))->px - ((ENEMY*)getData(enemy))->x), pTopLeftY, pBottomRightX + 1, pBottomRightY, '0');
    } else if (((ENEMY*)getData(enemy))->y > ((ENEMY*)getData(enemy))->py) { //Moved up
        drawBox(pTopLeftX, pBottomRightY + (((ENEMY*)getData(enemy))->py - ((ENEMY*)getData(enemy))->y), pBottomRightX, pBottomRightY, '0');
    } else if (((ENEMY*)getData(enemy))->y < ((ENEMY*)getData(enemy))->py) { //Moved down
        drawBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((ENEMY*)getData(enemy))->y - ((ENEMY*)getData(enemy))->py), '0');
    }
    drawImg(((ENEMY*)getData(enemy))->boundingBox->topLeft.x, 127 - ((ENEMY*)getData(enemy))->boundingBox->topLeft.y, 11, 11, ENEMY_IMGS[((ENEMY*)getData(enemy))->enemyDisplay]);
}

void eraseEnemy(LLNode* enemy) {
    drawBox(((ENEMY*)getData(enemy))->boundingBox->topLeft.x, 127 - ((ENEMY*)getData(enemy))->boundingBox->topLeft.y, ((ENEMY*)getData(enemy))->boundingBox->bottomRight.x, 127 - ((ENEMY*)getData(enemy))->boundingBox->bottomRight.y, '0');
}

void drawBoss(LLNode* boss) {
    int pTopLeftX = ((BOSS*)getData(boss))->boundingBox->topLeft.x - (((BOSS*)getData(boss))->x - ((BOSS*)getData(boss))->px);
    int pBottomRightX = ((BOSS*)getData(boss))->boundingBox->bottomRight.x - (((BOSS*)getData(boss))->x - ((BOSS*)getData(boss))->px);
    int pTopLeftY = 127 - (((BOSS*)getData(boss))->boundingBox->topLeft.y - (((BOSS*)getData(boss))->y - ((BOSS*)getData(boss))->py));
    int pBottomRightY = 127 - (((BOSS*)getData(boss))->boundingBox->bottomRight.y - (((BOSS*)getData(boss))->y - ((BOSS*)getData(boss))->py));
    if (((BOSS*)getData(boss))->x > ((BOSS*)getData(boss))->px) { //Moved right
        drawBox(pTopLeftX - 2, pTopLeftY, pTopLeftX + (((BOSS*)getData(boss))->x - ((BOSS*)getData(boss))->px), pBottomRightY, '0');
    } else if (((BOSS*)getData(boss))->x < ((BOSS*)getData(boss))->px) { //Moved left
        drawBox(pBottomRightX - (((BOSS*)getData(boss))->px - ((BOSS*)getData(boss))->x), pTopLeftY, pBottomRightX + 2, pBottomRightY, '0');
    } else if (((BOSS*)getData(boss))->y > ((BOSS*)getData(boss))->py) { //Moved up
        drawBox(pTopLeftX, pBottomRightY + (((BOSS*)getData(boss))->py - ((BOSS*)getData(boss))->y), pBottomRightX, pBottomRightY, '0');
    } else if (((BOSS*)getData(boss))->y < ((BOSS*)getData(boss))->py) { //Moved down
        drawBox(pTopLeftX, pTopLeftY, pBottomRightX, pTopLeftY - (((BOSS*)getData(boss))->y - ((BOSS*)getData(boss))->py), '0');
    }
    drawImg(((BOSS*)getData(boss))->boundingBox->topLeft.x, 127 - ((BOSS*)getData(boss))->boundingBox->topLeft.y, 19, 19, BOSS_IMGS[((BOSS*)getData(boss))->bossDisplay]);
}

void eraseBoss(LLNode* boss) {
    drawBox(((BOSS*)getData(boss))->boundingBox->topLeft.x, 127 - ((BOSS*)getData(boss))->boundingBox->topLeft.y, ((BOSS*)getData(boss))->boundingBox->bottomRight.x, 127 - ((BOSS*)getData(boss))->boundingBox->bottomRight.y, '0');
}

void drawPlayerProjectile(LLNode* projectile) {
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
        drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, '0');
    } else {
        drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, '0');
    }
}

void drawEnemyProjectile(LLNode* projectile) {
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
            drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x + 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, '0');
        } else if ((int)((PROJECTILE*)getData(projectile))->projectileDirection->pdx > 0 || (int)((PROJECTILE*)getData(projectile))->projectileDirection->dx > 0) { // Was or is going right
            drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, '0');
        } else { // Is going straight
            drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x , 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, '0');
        }
    } else {
        drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, '0');
    }
}

void drawBossProjectile(LLNode* projectile) {
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
            drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x + 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, '0');
        } else if ((int)((PROJECTILE*)getData(projectile))->projectileDirection->pdx > 0 || (int)((PROJECTILE*)getData(projectile))->projectileDirection->dx > 0) { // Was or is going right
            drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, '0');
        } else { // Is going straight
            drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x , 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, '0');
        }
    } else {
        drawBox(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->bottomRight.y, '0');
    }
}

void drawHealthBar(LLNode* healthBar) {
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
    drawBox(((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2, 127 - ((BAR*)getData(healthBar))->y, ((BAR*)getData(healthBar))->x - (BAR_WIDTH - 1) / 2 + (BAR_WIDTH - 1), 127 - ((BAR*)getData(healthBar))->y - 1, '0');
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
    if (modeSelector->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(modeSelector->boundingBox->topLeft.x, modeSelector->boundingBox->topLeft.y, modeSelector->boundingBox->bottomRight.x, modeSelector->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(modeSelector->boundingBox->topLeft.x, modeSelector->boundingBox->topLeft.y, modeSelector->boundingBox->bottomRight.x, modeSelector->boundingBox->bottomRight.y, WHITE);
        color = RED;
    } else {
        drawBox(modeSelector->boundingBox->topLeft.x, modeSelector->boundingBox->topLeft.y, modeSelector->boundingBox->bottomRight.x, modeSelector->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(modeSelector->boundingBox->topLeft.x, modeSelector->boundingBox->topLeft.y, modeSelector->boundingBox->bottomRight.x, modeSelector->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
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
            uLCD.text_mode(TRANSPARENT);
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
            uLCD.text_mode(TRANSPARENT);
            uLCD.printf("%c", message[i]);
            i++;
        }
    } else if (menuSettings->gameMode == GAME_MODE::SCORECAP) {
        uLCD.text_bold(ON);
        uLCD.set_font(FONT_7X8);
        uLCD.locate(14, 12);
        uLCD.color(color);
        uLCD.text_mode(TRANSPARENT);
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
            uLCD.text_mode(TRANSPARENT);
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
    drawBox(64 - 10, 64 - 10, 64 - 5, 64 + 10, '0');
    drawBox(64 + 5, 64 - 10, 64 + 10, 64 + 10, '0');
}

void drawResumed(void) {
    for (int i = 0; i <= 10; i++) {
        uLCD.triangle(64 - i, 64 - i, 64 - i, 64 + i, 64 + i, 64, WHITE);
    }
}

void eraseResumed(void) {
    for (int i = 10; i >= 0; i--) {
        uLCD.triangle(64 - i, 64 - i, 64 - i, 64 + i, 64 + i, 64, BLACK);
    }
}

void drawPlayButton(MENU_SETTINGS* menuSettings, BUTTON* play) {
    int color;
    if (play->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(play->boundingBox->topLeft.x, play->boundingBox->topLeft.y, play->boundingBox->bottomRight.x, play->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(play->boundingBox->topLeft.x, play->boundingBox->topLeft.y, play->boundingBox->bottomRight.x, play->boundingBox->bottomRight.y, WHITE);
        color = GREEN;
    } else {
        drawBox(play->boundingBox->topLeft.x, play->boundingBox->topLeft.y, play->boundingBox->bottomRight.x, play->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(play->boundingBox->topLeft.x, play->boundingBox->topLeft.y, play->boundingBox->bottomRight.x, play->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "PLAY";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(7 + i, 13);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

void drawDiffScale(MENU_SETTINGS* menuSettings, SLIDING_SCALE* difficulty) {
    drawBox(0, 127 - 47, 127, 127 - 37, '0');
    drawBox(0, difficulty->boundingBox->topLeft.y, 127, difficulty->boundingBox->bottomRight.y, '3');
    drawBox(39 - 5, 127 - 118, 89 - 5 + 11, 127 - 76, '0');
    if (difficulty->value == difficulty->maxVal) {
        drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'R');
        drawImg(64 - 9, 127 - 118, 19, 19, BOSS_IMGS[3]);
        drawImg(39 - 5, 127 - 103, 11, 11, ENEMY_IMGS[3]);
        drawImg(89 - 5, 127 - 103, 11, 11, ENEMY_IMGS[3]);
        drawImg(64 - 5, 127 - 90, 11, 11, ENEMY_IMGS[3]);
    } else if ((double)difficulty->value / difficulty->maxVal >= 0.75) {
        drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'O');
        drawImg(39 - 5, 127 - 103, 11, 11, ENEMY_IMGS[3]);
        drawImg(89 - 5, 127 - 103, 11, 11, ENEMY_IMGS[3]);
        drawImg(64 - 5, 127 - 90, 11, 11, ENEMY_IMGS[3]);
    } else if ((double)difficulty->value / difficulty->maxVal >= 0.50) {
        drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'Y');
        drawImg(46 - 5, 127 - 100, 11, 11, ENEMY_IMGS[3]);
        drawImg(82 - 5, 127 - 100, 11, 11, ENEMY_IMGS[3]);
    } else if ((double)difficulty->value / difficulty->maxVal >= 0.25) {
        drawBox(difficulty->boundingBox->topLeft.x, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y, 'G');
        drawImg(64 - 5, 127 - 100, 11, 11, ENEMY_IMGS[3]);
    }
    if (difficulty->sliderStatus == BUTTON_STATUS::SELECTED) {
        uLCD.filled_circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, LGREY);
        uLCD.circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, WHITE);
    } else {
        uLCD.filled_circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, DGREY);
        uLCD.circle(difficulty->boundingBox->bottomRight.x, difficulty->boundingBox->bottomRight.y - 2, 3, MGREY);
    }
}

void drawUserStatsButton(USER* user, BUTTON* userStats) {
    int color;
    if (userStats->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(userStats->boundingBox->topLeft.x, userStats->boundingBox->topLeft.y, userStats->boundingBox->bottomRight.x, userStats->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(userStats->boundingBox->topLeft.x, userStats->boundingBox->topLeft.y, userStats->boundingBox->bottomRight.x, userStats->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(userStats->boundingBox->topLeft.x, userStats->boundingBox->topLeft.y, userStats->boundingBox->bottomRight.x, userStats->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(userStats->boundingBox->topLeft.x, userStats->boundingBox->topLeft.y, userStats->boundingBox->bottomRight.x, userStats->boundingBox->bottomRight.y, MGREY);
        color = WHITE;
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
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

void drawDeleteUserButton(BUTTON* deleteProfile) {
    int color;
    if (deleteProfile->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(deleteProfile->boundingBox->topLeft.x, deleteProfile->boundingBox->topLeft.y, deleteProfile->boundingBox->bottomRight.x, deleteProfile->boundingBox->bottomRight.y, 'R');
        uLCD.rectangle(deleteProfile->boundingBox->topLeft.x, deleteProfile->boundingBox->topLeft.y, deleteProfile->boundingBox->bottomRight.x, deleteProfile->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(deleteProfile->boundingBox->topLeft.x, deleteProfile->boundingBox->topLeft.y, deleteProfile->boundingBox->bottomRight.x, deleteProfile->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(deleteProfile->boundingBox->topLeft.x, deleteProfile->boundingBox->topLeft.y, deleteProfile->boundingBox->bottomRight.x, deleteProfile->boundingBox->bottomRight.y, MGREY);
        color = RED;
    }
    char message[20] = "DELETE PROFILE";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(2 + i, 14);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

void drawLoginBackground(void) {
    // Sun
    uLCD.filled_circle(127, 0, 30, YELLOW);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 0, 15, LGREY);
    uLCD.filled_circle(8, 12, 2, BLACK);
    uLCD.filled_circle(0, 9, 3, MGREY);
    uLCD.filled_circle(-1, 5, 2, DGREY);
    uLCD.filled_circle(5, -1, 3, MGREY);
    uLCD.filled_circle(9, 2, 2, DGREY);
    drawImg(64 - 9, 127 - 113, 19, 19, BOSS_IMGS[3]);
    drawImg(39 - 5, 127 - 98, 11, 11, ENEMY_IMGS[3]);
    drawImg(89 - 5, 127 - 98, 11, 11, ENEMY_IMGS[3]);
    drawImg(64 - 5, 127 - 80, 11, 11, PLAYER_IMGS[3]);
    char message[30] = "STAR WARS";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(WHITE);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(4 + i, 0);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

void drawRegistrationBackground(void) {
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
    drawImg(64 - 9, 127 - 113, 19, 19, BOSS_IMGS[3]);
    drawImg(39 - 5, 127 - 98, 11, 11, ENEMY_IMGS[3]);
    drawImg(89 - 5, 127 - 98, 11, 11, ENEMY_IMGS[3]);
    drawImg(64 - 5, 127 - 80, 11, 11, PLAYER_IMGS[3]);
    uLCD.filled_rectangle(0, 127 - 39, 127, 127, DGREY);
}

void drawMenuBackground(void) {
    // Sun
    uLCD.filled_circle(127, 0, 30, YELLOW);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 7, 15, LGREY);
    uLCD.filled_circle(8, 19, 2, BLACK);
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
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 7, 15, LGREY);
    uLCD.filled_circle(8, 19, 2, BLACK);
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
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", hScoreStat[i]);
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
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", eKilledStat[i]);
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
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", pDeathsStat[i]);
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
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", tPointsStat[i]);
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
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", tTimeStat[i]);
        i++;
    }
}

/**
 * Draws login button
 */
void drawLoginButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "LOGIN";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(6 + i, 12);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

void drawInvalidLoginButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = RED;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "LOGIN";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(6 + i, 12);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws register button
 */
void drawRegisterButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "REGISTER";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(5 + i, 14);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws username base button
 */
void drawUserBaseButton(USER* userInfo, BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
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
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", userInfo->username[i]);
        i++;
    }
}

/**
 * Draws username number button
 */
void drawUserNumButton(USER* userInfo, BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
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
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", strNum[i]);
        i++;
    }
}

/**
 * Draws back button
 */
void drawBackButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = RED;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "<";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(1 + i, 1);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws next button
 */
void drawNextButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = GREEN;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = ">";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(16 + i, 1);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws invalid next button
 */
void drawInvalidNextButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = RED;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = ">";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(16 + i, 1);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws delete button
 */
void drawDeleteButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = RED;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "X";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_7X8);
        uLCD.locate(12 + i, 14);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws zero button
 */
void drawZeroButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "0";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(8 + i, 14);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws one button
 */
void drawOneButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "1";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(5 + i, 11);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws two button
 */
void drawTwoButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "2";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(8 + i, 11);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws three button
 */
void drawThreeButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "3";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(12 + i, 11);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws four button
 */
void drawFourButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "4";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(5 + i, 8);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws five button
 */
void drawFiveButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "5";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(8 + i, 8);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws six button
 */
void drawSixButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "6";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(12 + i, 8);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws seven button
 */
void drawSevenButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "7";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(5 + i, 5);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws eight button
 */
void drawEightButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "8";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(8 + i, 5);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws nine button
 */
void drawNineButton(BUTTON* button) {
    int color;
    if (button->buttonStatus == BUTTON_STATUS::SELECTED) {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
        color = BLACK;
    } else {
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '3');
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, MGREY);
        color = MGREY;
    }
    char message[] = "9";
    int i = 0;
    while (message[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(color);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(12 + i, 5);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", message[i]);
        i++;
    }
}

/**
 * Draws user default skin button
 */
void drawDefaultSkinButton(USER* userInfo, BUTTON* button) {
    int color;
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
        drawBox(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, '5');
        uLCD.circle(64, 127 - 18, 9, WHITE);
        uLCD.rectangle(button->boundingBox->topLeft.x, button->boundingBox->topLeft.y, button->boundingBox->bottomRight.x, button->boundingBox->bottomRight.y, WHITE);
    } else {
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
        uLCD.text_mode(TRANSPARENT);
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
            uLCD.text_mode(TRANSPARENT);
            uLCD.printf("%c", message[i]);
            i++;
        }
    } else {
        uLCD.cls();
        // Sun
        uLCD.filled_circle(127, 0, 30, YELLOW);
        uLCD.circle(127, 0, 30, WHITE);
        uLCD.circle(127, 0, 28, WHITE);
        // Moon
        uLCD.filled_circle(0, 7, 15, LGREY);
        uLCD.filled_circle(8, 19, 2, BLACK);
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
            uLCD.text_mode(TRANSPARENT);
            uLCD.printf("%c", message[i]);
            i++;
        }
        drawImg(64 - 7, 127 - 60, 11, 11, PLAYER_IMGS[3]);
    }
}

void drawGameLost(void) {
    uLCD.cls();
    // Sun
    uLCD.filled_circle(127, 0, 30, RED);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 29, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 7, 15, RED);
    uLCD.filled_circle(8, 19, 2, BLACK);
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
    drawImg(39 - 5, 127 - 85, 11, 11, ENEMY_IMGS[3]);
    drawImg(89 - 5, 127 - 85, 11, 11, ENEMY_IMGS[3]);
    drawImg(64 - 5, 127 - 68, 11, 11, ENEMY_IMGS[3]);
    char game[30] = "GAME    OVER";
    int i = 0;
    while (game[i]) {
        loadMusic();
        uLCD.text_bold(ON);
        uLCD.color(RED);
        uLCD.set_font(FONT_8X8);
        uLCD.locate(3 + i, 8);
        uLCD.text_mode(TRANSPARENT);
        uLCD.printf("%c", game[i]);
        i++;
    }
}
