#include "graphics.hpp"
#include "gameMusic.hpp"
#include "globals.hpp"
#include "doublyLinkedList.hpp"
#include "hardware.hpp"
#include "login.hpp"
#include "menu.hpp"
#include "projectile.hpp"
#include "player.hpp"
#include "opponent.hpp"
#include "enemy.hpp"
#include "finalBoss.hpp"
#include "game.hpp"
#include "registration.hpp"
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

void concatText(const char* message, char* textArr, int scale) {
    int msgLength = strlen(message);
    int overallWidth = 7 * msgLength;
    int width = 7;
    int height = 8;
    for (int i = 0; i < msgLength; i++) {
        char character[57]; // The corresponding character from font array
        switch(message[i]) {
        case '!': strcpy(character, font[0]); break;
        case '.': strcpy(character, font[1]); break;
        case ':': strcpy(character, font[2]); break;
        case '<': strcpy(character, font[3]); break;
        case '>': strcpy(character, font[4]); break;
        case '_': strcpy(character, font[5]); break;
        case '0': strcpy(character, font[6]); break;
        case '1': strcpy(character, font[7]); break;
        case '2': strcpy(character, font[8]); break;
        case '3': strcpy(character, font[9]); break;
        case '4': strcpy(character, font[10]); break;
        case '5': strcpy(character, font[11]); break;
        case '6': strcpy(character, font[12]); break;
        case '7': strcpy(character, font[13]); break;
        case '8': strcpy(character, font[14]); break;
        case '9': strcpy(character, font[15]); break;
        case 'A': strcpy(character, font[16]); break;
        case 'B': strcpy(character, font[17]); break;
        case 'C': strcpy(character, font[18]); break;
        case 'D': strcpy(character, font[19]); break;
        case 'E': strcpy(character, font[20]); break;
        case 'F': strcpy(character, font[21]); break;
        case 'G': strcpy(character, font[22]); break;
        case 'H': strcpy(character, font[23]); break;
        case 'I': strcpy(character, font[24]); break;
        case 'J': strcpy(character, font[25]); break;
        case 'K': strcpy(character, font[26]); break;
        case 'L': strcpy(character, font[27]); break;
        case 'M': strcpy(character, font[28]); break;
        case 'N': strcpy(character, font[29]); break;
        case 'O': strcpy(character, font[30]); break;
        case 'P': strcpy(character, font[31]); break;
        case 'Q': strcpy(character, font[32]); break;
        case 'R': strcpy(character, font[33]); break;
        case 'S': strcpy(character, font[34]); break;
        case 'T': strcpy(character, font[35]); break;
        case 'U': strcpy(character, font[36]); break;
        case 'V': strcpy(character, font[37]); break;
        case 'W': strcpy(character, font[38]); break;
        case 'X': strcpy(character, font[39]); break;
        case 'Y': strcpy(character, font[40]); break;
        case 'Z': strcpy(character, font[41]); break;
        case 'a': strcpy(character, font[42]); break;
        case 'b': strcpy(character, font[43]); break;
        case 'c': strcpy(character, font[44]); break;
        case 'd': strcpy(character, font[45]); break;
        case 'e': strcpy(character, font[46]); break;
        case 'f': strcpy(character, font[47]); break;
        case 'g': strcpy(character, font[48]); break;
        case 'h': strcpy(character, font[49]); break;
        case 'i': strcpy(character, font[50]); break;
        case 'j': strcpy(character, font[51]); break;
        case 'k': strcpy(character, font[52]); break;
        case 'l': strcpy(character, font[53]); break;
        case 'm': strcpy(character, font[54]); break;
        case 'n': strcpy(character, font[55]); break;
        case 'o': strcpy(character, font[56]); break;
        case 'p': strcpy(character, font[57]); break;
        case 'q': strcpy(character, font[58]); break;
        case 'r': strcpy(character, font[59]); break;
        case 's': strcpy(character, font[60]); break;
        case 't': strcpy(character, font[61]); break;
        case 'u': strcpy(character, font[62]); break;
        case 'v': strcpy(character, font[63]); break;
        case 'w': strcpy(character, font[64]); break;
        case 'x': strcpy(character, font[65]); break;
        case 'y': strcpy(character, font[66]); break;
        case 'z': strcpy(character, font[67]); break;
        case '+': strcpy(character, font[68]); break;
        case '-': strcpy(character, font[69]); break;
        case '/': strcpy(character, font[70]); break;
        default:
            // If the requested character doesn't exist then just do a space
            strcpy(character, "00000000000000000000000000000000000000000000000000000000");
            break;
        }
        for (int j = 0; j < height; j++) {
            for (int m = 0; m < scale; m++) {
                for (int k = 0; k < width; k++) {
                    for (int l = 0; l < scale; l++) {
                        textArr[(j * scale + m) * (overallWidth * scale) + i * (scale * width) + (k * scale + l)] = character[j * width + k];
                    }
                }
            }
        }
    }
}

void drawText(const char* message, int x, int y, int textColor, int backgroundColor, double brightness, bool redshift, int scale) {
    int msgWidth = 7 * strlen(message);
    int textHeight = 8;
    char text[scale * msgWidth * scale * textHeight];
    concatText(message, text, scale);

    // Ensure coordinates are actually on the screen
    x = max(0, min(x, 127));
    y = max(0, min(y, 127));

    int textImg[scale * msgWidth * scale * textHeight];
    for (int i = 0; i < scale * msgWidth * scale * textHeight; i++) {
        loadMusic();
        if (text[i] == '0' && backgroundColor == -1) {
            int xPos = x + (i % (scale * msgWidth));
            int yPos = y + (i / (scale * msgWidth));
            xPos = max(0, min(xPos, 127));
            yPos = max(0, min(yPos, 127));
            int pixel = gameBackground[yPos * 128 + xPos];
            // Apply brightness to original background color if not 1
            if (brightness != 1) {
                int red = (pixel >> 16) & 0xFF;
                int green = (pixel >> 8) & 0xFF;
                int blue = pixel & 0xFF;
                red = (double) red * brightness;
                green = (double) green * brightness;
                blue = (double) blue * brightness;
                pixel = (red << 16) | (green << 8) | blue;
            }
            // Redshift if needed
            pixel = (redshift) ? pixel & 0xFF0000 : pixel;
            textImg[i] = pixel;
        } else if (text[i] == '0' && backgroundColor >= 0) {
            textImg[i] = backgroundColor;
        } else {
            textImg[i] = textColor;
        }
    }
    uLCD.BLIT(x, y, scale * msgWidth, scale * textHeight, textImg);
}

void drawProfileImg(void) {
    int chunkHeight = 32;
    for (int i = 0; i < 128; i += chunkHeight) {
        int chunk[128 * chunkHeight];
        for (int j = 0; j < chunkHeight; j++) {
            for (int k = 0; k < 128; k++) {
                loadMusic();
                chunk[j * 128 + k] = profileImg[i * 128 + j * 128 + k];
            }
        }
        uLCD.BLIT(0, i, 128, chunkHeight, chunk);
    }
}

void drawControls(void) {
    uLCD.cls();
    // NavSwitch
    drawText("NAV", 4, 64 - 15 - 10, WHITE, BLACK);
    drawBox(7, 64 - 2, 7 + 15, 64 + 3, 'R');
    drawBox(12, 64 - 7, 12 + 5, 64 + 8, 'R');
    // Action button
    drawText("ACT", 39 - 6, 64 - 15 - 10, WHITE, BLACK);
    uLCD.filled_circle(43, 64, 8, LGREY);
    // Super / vol down button
    drawText("SPR", 38 - 6 + 25, 64 - 15 - 10 - 10, WHITE, BLACK);
    drawText("V-", 43 - 6 + 25, 64 - 15 - 10, WHITE, BLACK);
    uLCD.filled_circle(43 + 25, 64, 8, LGREY);
    // Pause / resume / vol up button
    drawText("P/R", 41 - 6 + 46, 64 - 15 - 10 - 10, WHITE, BLACK);
    drawText("V+", 45 - 6 + 46, 64 - 15 - 10, WHITE, BLACK);
    uLCD.filled_circle(43 - 6 + 55, 64, 8, LGREY);
    // Exit button
    drawText("EXT", 45 - 6 + 67, 64 - 15 - 10, WHITE, BLACK);
    uLCD.filled_circle(43 - 6 + 78, 64, 8, LGREY);
    // Continue message
    drawText("ACT TO CONTINUE", 10, 64 + 15 + 15, WHITE, BLACK);
}

void drawGameBackground(bool redshift, double brightness) {
    int chunkHeight = 32;
    for (int i = 0; i < 128; i += chunkHeight) {
        int chunk[128 * chunkHeight];
        for (int j = 0; j < chunkHeight; j++) {
            for (int k = 0; k < 128; k++) {
                loadMusic();
                int pixel = gameBackground[i * 128 + j * 128 + k];
                // Apply brightness to original background if not 1
                if (brightness != 1) {
                    int red = (pixel >> 16) & 0xFF;
                    int green = (pixel >> 8) & 0xFF;
                    int blue = pixel & 0xFF;
                    red = (double) red * brightness;
                    green = (double) green * brightness;
                    blue = (double) blue * brightness;
                    pixel = (red << 16) | (green << 8) | blue;
                }
                // Redshift if needed
                chunk[j * 128 + k] = (redshift) ? pixel & 0xFF0000 : pixel;
            }
        }
        uLCD.BLIT(0, i, 128, chunkHeight, chunk);
    }
}

void drawImg(int x, int y, int width, int height, const char* object, double brightness) {
    // Ensure coordinates are actually on the screen
    x = max(0, min(x, 127));
    y = max(0, min(y, 127));

    int img[width * height];
    for (int i = 0; i < width * height; i++) {
        loadMusic();
        int xPos = x + (i % width);
        int yPos = y + (i / width);
        xPos = max(0, min(xPos, 127));
        yPos = max(0, min(yPos, 127));
        if (object[i] == '0') {
            int pixel = gameBackground[yPos * 128 + xPos];
            // Apply brightness to original background color if not 1
            if (brightness != 1) {
                int red = (pixel >> 16) & 0xFF;
                int green = (pixel >> 8) & 0xFF;
                int blue = pixel & 0xFF;
                red = (double) red * brightness;
                green = (double) green * brightness;
                blue = (double) blue * brightness;
                pixel = (red << 16) | (green << 8) | blue;
            }
            // Redshift if needed
            if (getGameLoop() && getGameLoop()->gameStatus == GAMESTATUS::LOST) img[i] = pixel & 0xFF0000;
            else img[i] = pixel;
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
        loadMusic();
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
            loadMusic();
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
            drawImg(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, 6, 7, "0000000000Y0000O0Y000RO00WR0005WW000550000");
        } else if (((PROJECTILE*)getData(projectile))->projectileDirection->dx > 0) { // right
            drawImg(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, 6, 7, "0000000Y0000Y0O0000OR000000RW0000WW5000055");
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
            drawImg(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, 6, 7, "0000000000Y0000O0Y000RO00WR0005WW000550000");
        } else if (((PROJECTILE*)getData(projectile))->projectileDirection->dx > 0) { // right
            drawImg(((PROJECTILE*)getData(projectile))->boundingBox->topLeft.x - 3, 127 - ((PROJECTILE*)getData(projectile))->boundingBox->topLeft.y, 6, 7, "0000000Y0000Y0O0000OR000000RW0000WW5000055");
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
    // Account for numHearts
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
    int color = YELLOW;
    double brightness = 0.5;
    if (((PLAYER*)getData(player))->superChargeStatus == MAX_SUPER_CHARGE) {
        // Creates pulsing effect for superCharge bar
        brightness = 0.25 * sin((double) ((PLAYER*)getData(player))->tick * 3.14 / 10) + 0.75;
    }
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;
    r = r * brightness;
    g = g * brightness;
    b = b * brightness;
    color = ((r << 16) | (g << 8) | b);
    if (((PLAYER*)getData(player))->superChargeStatus == 0) {
        uLCD.filled_rectangle(1, 127 - 5, 126, 127 - 3, MGREY);
        uLCD.pixel(0, 127 - 4, MGREY);
        uLCD.pixel(127, 127 - 4, MGREY);
    } else {
        if (((PLAYER*)getData(player))->superChargeStatus < MAX_SUPER_CHARGE) {
            uLCD.filled_rectangle((int)round(125 * (double)((PLAYER*)getData(player))->superChargeStatus / MAX_SUPER_CHARGE) + 1, 127 - 5, 126, 127 - 3, MGREY);
        }
        uLCD.pixel(0, 127 - 4, color);
        uLCD.filled_rectangle(1, 127 - 5, (int)round(126 * (double)((PLAYER*)getData(player))->superChargeStatus / MAX_SUPER_CHARGE), 127 - 3, color);
        uLCD.pixel((int)round(126 * (double)((PLAYER*)getData(player))->superChargeStatus / MAX_SUPER_CHARGE) + 1, 127 - 4, color);
    }
}

void eraseSuperAttackBar(LLNode* player) {
    drawBackgroundBox(0, 127 - 7, 127, 127 - 3);
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
        drawText("INF", 109 - 11, 104, color, textBackground);
    } else if (menuSettings->gameMode == GAME_MODE::LEVELS) {
        drawText("123", 109 - 11, 104, color, textBackground);
    } else if (menuSettings->gameMode == GAME_MODE::SCORECAP) {
        drawText("___", 109 - 11, 104 - 8, color, textBackground);
        char message[5];
        sprintf(message, "%03d", menuSettings->scoreCap);
        drawText(message, 109 - 11, 104, color, textBackground);
    } else if (menuSettings->gameMode == GAME_MODE::PVP) {
        drawText("PVP", 109 - 11, 104, color, textBackground);
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
    drawText("PLAY", 64 - 14, 109 - 4, color, textBackground);
}

void drawDiffScale(MENU_SETTINGS* menuSettings, SLIDING_SCALE* difficulty) {
    drawBackgroundBox(0, difficulty->boundingBox->topLeft.y - 1, 127, difficulty->boundingBox->bottomRight.y + 1);
    uLCD.filled_rectangle(127, difficulty->boundingBox->topLeft.y + 1, 127, difficulty->boundingBox->topLeft.y + 2, DGREY);
    drawBox(1, difficulty->boundingBox->topLeft.y, 126, difficulty->boundingBox->bottomRight.y, '3');
    drawBackgroundBox(39 - 5, 127 - 116, 89 - 5 + 11, 127 - 76);
    if (menuSettings->gameMode != GAME_MODE::PVP) {
        if (difficulty->value == difficulty->maxVal) {
            uLCD.filled_rectangle(0, difficulty->boundingBox->topLeft.y + 1, 0, difficulty->boundingBox->topLeft.y + 2, RED);
            drawBox(difficulty->boundingBox->topLeft.x + 1, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x - 1, difficulty->boundingBox->bottomRight.y, 'R');
            drawImg(64 - 9, 127 - 116, 19, 19, BOSS_IMGS[3]);
            drawImg(39 - 5, 127 - 101, 11, 11, MISSILE_ENEMY_IMGS[3]);
            drawImg(89 - 5, 127 - 101, 11, 11, MISSILE_ENEMY_IMGS[3]);
            drawImg(64 - 5, 127 - 88, 11, 11, NORMAL_ENEMY_IMGS[3]);
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.75) {
            uLCD.filled_rectangle(0, difficulty->boundingBox->topLeft.y + 1, 0, difficulty->boundingBox->topLeft.y + 2, ORANGE);
            drawBox(difficulty->boundingBox->topLeft.x + 1, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x - 1, difficulty->boundingBox->bottomRight.y, 'O');
            drawImg(39 - 5, 127 - 101, 11, 11, MISSILE_ENEMY_IMGS[3]);
            drawImg(89 - 5, 127 - 101, 11, 11, MISSILE_ENEMY_IMGS[3]);
            drawImg(64 - 5, 127 - 88, 11, 11, NORMAL_ENEMY_IMGS[3]);
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.50) {
            uLCD.filled_rectangle(0, difficulty->boundingBox->topLeft.y + 1, 0, difficulty->boundingBox->topLeft.y + 2, YELLOW);
            drawBox(difficulty->boundingBox->topLeft.x + 1, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x - 1, difficulty->boundingBox->bottomRight.y, 'Y');
            drawImg(46 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
            drawImg(82 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.25) {
            uLCD.filled_rectangle(0, difficulty->boundingBox->topLeft.y + 1, 0, difficulty->boundingBox->topLeft.y + 2, GREEN);
            drawBox(difficulty->boundingBox->topLeft.x + 1, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x - 1, difficulty->boundingBox->bottomRight.y, 'G');
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
            uLCD.filled_rectangle(0, difficulty->boundingBox->topLeft.y + 1, 0, difficulty->boundingBox->topLeft.y + 2, RED);
            drawBox(difficulty->boundingBox->topLeft.x + 1, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x - 1, difficulty->boundingBox->bottomRight.y, 'R');
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.75) {
            uLCD.filled_rectangle(0, difficulty->boundingBox->topLeft.y + 1, 0, difficulty->boundingBox->topLeft.y + 2, ORANGE);
            drawBox(difficulty->boundingBox->topLeft.x + 1, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x - 1, difficulty->boundingBox->bottomRight.y, 'O');
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.50) {
            uLCD.filled_rectangle(0, difficulty->boundingBox->topLeft.y + 1, 0, difficulty->boundingBox->topLeft.y + 2, YELLOW);
            drawBox(difficulty->boundingBox->topLeft.x + 1, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x - 1, difficulty->boundingBox->bottomRight.y, 'Y');
        } else if ((double)difficulty->value / difficulty->maxVal >= 0.25) {
            uLCD.filled_rectangle(0, difficulty->boundingBox->topLeft.y + 1, 0, difficulty->boundingBox->topLeft.y + 2, GREEN);
            drawBox(difficulty->boundingBox->topLeft.x + 1, difficulty->boundingBox->topLeft.y, difficulty->boundingBox->bottomRight.x - 1, difficulty->boundingBox->bottomRight.y, 'G');
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
    int nameLength = strlen(message);
    drawText(message, 63 - (int)round(7 * (double)nameLength / 2), 2, color, textBackground);
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
    int msgLength = strlen(message);
    drawText("DELETE PROFILE", 63 - (int)round(7 * (double) msgLength / 2), 115 - 3, color, textBackground);
}

void drawLoginBackground(void) {
    // Sun
    uLCD.filled_circle(127, 0, 30, YELLOW);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 29, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 0, 15, LGREY);
    uLCD.filled_circle(8, 10, 2, DGREY);
    uLCD.filled_circle(0, 9, 3, MGREY);
    uLCD.filled_circle(-1, 5, 2, DGREY);
    uLCD.filled_circle(5, -1, 3, MGREY);
    uLCD.filled_circle(9, 2, 2, DGREY);
    drawImg(64 - 9, 127 - 113, 19, 19, BOSS_IMGS[3]);
    drawImg(39 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
    drawImg(89 - 5, 127 - 98, 11, 11, NORMAL_ENEMY_IMGS[3]);
    drawImg(64 - 5, 127 - 80, 11, 11, PLAYER_IMGS[3]);
    drawText("STAR WARS", 31, 1, WHITE);
}

void drawRegistrationBackground(void) {
    // Sun
    uLCD.filled_circle(127, 20, 30, YELLOW);
    uLCD.circle(127, 20, 30, WHITE);
    uLCD.circle(127, 20, 29, WHITE);
    uLCD.circle(127, 20, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 32, 15, LGREY);
    uLCD.filled_circle(8, 42, 2, DGREY);
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
    uLCD.circle(127, 20, 30, WHITE);
    uLCD.circle(127, 20, 29, WHITE);
    uLCD.circle(127, 20, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 32, 15, LGREY);
    uLCD.filled_circle(8, 42, 2, DGREY);
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
    uLCD.filled_circle(8, 17, 2, DGREY);
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
    double brightness = 0.5;
    drawGameBackground(false, brightness);
    // Sun
    uLCD.filled_circle(127, 0, 30, YELLOW);
    uLCD.circle(127, 0, 30, WHITE);
    uLCD.circle(127, 0, 29, WHITE);
    uLCD.circle(127, 0, 28, WHITE);
    // Moon
    uLCD.filled_circle(0, 7, 15, LGREY);
    uLCD.filled_circle(8, 17, 2, DGREY);
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
    drawText(hScoreStat, 14, 21, WHITE, -1, brightness);
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
    drawText(eKilledStat, 14, 37, WHITE, -1, brightness);
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
    drawText(pDeathsStat, 14, 52, WHITE, -1, brightness);
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
    drawText(tPointsStat, 14, 68, WHITE, -1, brightness);
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
    drawText(tTimeStat, 14, 84, WHITE, -1, brightness);
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
    drawText("LOGIN", 64 - 17, 98 - 3, color, textBackground);
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
    drawText("LOGIN", 64 - 17, 98 - 3, color, textBackground);
}

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
    drawText("REGISTER", 64 - 28, 115 - 3, color, textBackground);
}

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
    int nameLength = strlen(userInfo->username);
    drawText(userInfo->username, 93 / 2 - (int)round(7 * (double)nameLength / 2), 81 - 3, color, textBackground);
}

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
    drawText(strNum, 110 - 7, 81 - 3, color, textBackground);
}

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
    drawText("<", 12 - 4, 12 - 3, color, textBackground);
}

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
    drawText(">", 114 - 2, 12 - 3, color, textBackground);
}

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
    drawText(">", 114 - 2, 12 - 3, color, textBackground);
}

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
    drawText("X", 88 - 3, 114 - 3, color, textBackground);
}

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
    drawText("0", 63 - 3, 114 - 3, color, textBackground);
}

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
    drawText("1", 38 - 3, 89 - 3, color, textBackground);
}

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
    drawText("2", 63 - 3, 89 - 3, color, textBackground);
}

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
    drawText("3", 88 - 3, 89 - 3, color, textBackground);
}

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
    drawText("4", 38 - 3, 64 - 3, color, textBackground);
}

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
    drawText("5", 63 - 3, 64 - 3, color, textBackground);
}

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
    drawText("6", 88 - 3, 64 - 3, color, textBackground);
}

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
    drawText("7", 38 - 3, 39 - 3, color, textBackground);
}

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
    drawText("8", 63 - 3, 39 - 3, color, textBackground);
}

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
    drawText("9", 88 - 3, 39 - 3, color, textBackground);
}

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

    int skinTypeNameLength = strlen(skinType);
    drawText(skinType, 64 - (int)round(7 * (double) skinTypeNameLength / 2), 81 - 3, color, textBackground);
    drawImg(64 - 5, 127 - 80, 11, 11, PLAYER_IMGS[3]);
}

void drawPassword(char password[5]) {
    uLCD.filled_rectangle(26, 0, 127 - 26, 0 + 25, BLACK);
    int pwLength = 0;
    while (password[pwLength] != ' ' && password[pwLength]) {
        pwLength++;
    }
    drawText(password, 63 - (int)round(7 * (double) pwLength / 2), 12 - 3, GREEN, BLACK);
}

void drawInvalidPassword(char password[5]) {
    uLCD.filled_rectangle(26, 0, 127 - 26, 0 + 25, BLACK);
    int pwLength = 0;
    while (password[pwLength] != ' ' && password[pwLength]) {
        pwLength++;
    }
    drawText(password, 63 - (int)round(7 * (double) pwLength / 2), 12 - 3, RED, BLACK);
}

void drawGameWon(void) {
    if (getMenuSettings()->gameMode == GAME_MODE::LEVELS && getGameLoop()->level < 3) {
        uLCD.cls();
        char message[30] = "LEVEL ";
        char strLevel[10];
        sprintf(strLevel, "%d PASSED", getGameLoop()->level);
        strcat(message, strLevel);
        int msgLength = strlen(message);
        drawText(message, 63 - (int)round(7 * (double) msgLength / 2), 40, WHITE, BLACK);
    } else {
        drawGameBackground(false);
        // Sun
        uLCD.filled_circle(127, 0, 30, YELLOW);
        uLCD.circle(127, 0, 30, WHITE);
        uLCD.circle(127, 0, 29, WHITE);
        uLCD.circle(127, 0, 28, WHITE);
        // Moon
        uLCD.filled_circle(0, 7, 15, LGREY);
        uLCD.filled_circle(8, 17, 2, DGREY);
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
        int msgLength = strlen(message);
        drawText(message, 63 - (int)round(7 * (double) msgLength / 2), 40, WHITE);
        drawImg(64 - 7, 127 - 60, 11, 11, PLAYER_IMGS[3]);
    }
}

void drawGameLost(void) {
    double brightness = 0.5;
    drawGameBackground(true, brightness);
    // Sun
    uLCD.filled_circle(127, 0, 30, RED);
    uLCD.circle(127, 0, 30, MRED);
    uLCD.circle(127, 0, 29, MRED);
    uLCD.circle(127, 0, 28, MRED);
    // Moon
    uLCD.filled_circle(0, 7, 15, RED);
    uLCD.filled_circle(8, 17, 2, DRED);
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

    drawImg(64 - 9, 127 - 105, 19, 19, BOSS_IMGS[3], brightness);
    drawImg(39 - 5, 127 - 85, 11, 11, MISSILE_ENEMY_IMGS[3], brightness);
    drawImg(89 - 5, 127 - 85, 11, 11, MISSILE_ENEMY_IMGS[3], brightness);
    drawImg(64 - 5, 127 - 68, 11, 11, NORMAL_ENEMY_IMGS[3], brightness);
    drawText("GAME", 21, 56, RED, -1, brightness, true);
    drawText("OVER", 77, 56, RED, -1, brightness, true);
}
