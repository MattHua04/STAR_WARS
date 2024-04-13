#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "globals.hpp"
#include "game.hpp"
#include "doublyLinkedList.hpp"
#include "menu.hpp"
#include "users.hpp"
#include "gameBackground.hpp"
#include "enemySprite.hpp"
#include "finalBossSprite.hpp"
#include "playerSprite.hpp"

#define WIDTH 128
#define HEIGHT 128

/**
 * Returns the hex equivalent of a char color
 */
int getHexColor(char color, bool isOpponent);

/**
 * Load a chunk of pixels from a .raw file
 */
void loadChunk(const char *filename, int *chunk, int startX, int startY, int width, int height);

/**
 * Draws github profile pic
 */
void drawProfileImg(void);

/**
 * Draws game background. Redshift for loss screen
 */
void drawGameBackground(bool redshift);

/**
 * Draws an image in the form of a string of chars with specified dimensions at a cord on screen
 */
void drawImg(const char* object, int x, int y, int width, int height);

/**
 * Specifically meant to draw opponent player since player image needs to be mirrored
 */
void drawImgOpponent(const char* object, int x, int y, int width, int height);

/**
 * Draws a box bounded by a top left and bottom right cord
 */
void drawBox(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, char color);

/**
 * Draws a box bounded by a top left and bottom right cord containing appropriate background pixels
 */
void drawBackgroundBox(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY);

/**
 * Draws the player
 */
void drawPlayer(LLNode* player);

void erasePlayer(LLNode* player);

/**
 * Draws the opponent player
 */
void drawOpponent(LLNode* opponent);

void eraseOpponent(LLNode* opponent);

/**
 * Draw an enemy
 */
void drawEnemy(LLNode* enemy);

void eraseEnemy(LLNode* enemy);

/**
 * Draws a final boss
 */
void drawBoss(LLNode* boss);

void eraseBoss(LLNode* boss);

/**
 * Draws player projectile depending on attack type
 */
void drawPlayerProjectile(LLNode* projectile);

void erasePlayerProjectile(LLNode* projectile);

/**
 * Draws opponent projectile depending on attack type
 */
void drawOpponentProjectile(LLNode* projectile);

void eraseOpponentProjectile(LLNode* projectile);

/**
 * Draws enemy projectile depending on enemy type
 */
void drawEnemyProjectile(LLNode* projectile);

void eraseEnemyProjectile(LLNode* projectile);

/**
 * Draws boss projectile depending on attack type
 */
void drawBossProjectile(LLNode* projectile);

void eraseBossProjectile(LLNode* projectile);

/**
 * Draws a healthBar
 */
void drawHealthBar(LLNode* healthBar);

void eraseHealthBar(LLNode* healthBar);

/**
 * Draws a superAttackStatus
 */
void drawSuperAttackBar(LLNode* player);

void eraseSuperAttackBar(LLNode* player);

/**
 * Draws skin selector button
 */
void drawSSButton(MENU_SETTINGS* menuSettings, BUTTON* skinSelector);

/**
 * Draws game mode selector button
 */
void drawMSButton(MENU_SETTINGS* menuSettings, BUTTON* modeSelector);

/**
 * Draws pause button
 */
void drawPaused(void);

void erasePaused(void);

/**
 * Draws resume button
 */
void drawResumed(void);

void eraseResumed(void);

/**
 * Draws menu play button
 */
void drawPlayButton(MENU_SETTINGS* menuSettings, BUTTON* play);

/**
 * Draws difficulty slider
 */
void drawDiffScale(MENU_SETTINGS* menuSettings, SLIDING_SCALE* difficulty);

/**
 * Draws user stats button
 */
void drawUserStatsButton(USER* user, BUTTON* userStats);

/**
 * Draws delete user button
 */
void drawDeleteUserButton(BUTTON* deleteProfile);

/**
 * Draws login background
 */
void drawLoginBackground(void);

/**
 * Draws registration background
 */
void drawRegistrationBackground(void);

/**
 * Draws registration skin selection page background
 */
void drawRegistrationSkinSelectionBackground(void);

/**
 * Draws menu background
 */
void drawMenuBackground(void);

/**
 * Draws user stats and background
 */
void drawUserStatsBackground(USER* user);

/**
 * Draws login button
 */
void drawLoginButton(BUTTON* button);

/**
 * Draws login button for invalid credentials
 */
void drawInvalidLoginButton(BUTTON* button);

/**
 * Draws register button
 */
void drawRegisterButton(BUTTON* button);

/**
 * Draws username base button
 */
void drawUserBaseButton(USER* userInfo, BUTTON* button);

/**
 * Draws username number button
 */
void drawUserNumButton(USER* userInfo, BUTTON* button);

/**
 * Draws back button
 */
void drawBackButton(BUTTON* button);

/**
 * Draws next button
 */
void drawNextButton(BUTTON* button);

/**
 * Draws invalid next button
 */
void drawInvalidNextButton(BUTTON* button);

/**
 * Draws delete button
 */
void drawDeleteButton(BUTTON* button);

/**
 * Draws zero button
 */
void drawZeroButton(BUTTON* button);

/**
 * Draws one button
 */
void drawOneButton(BUTTON* button);

/**
 * Draws two button
 */
void drawTwoButton(BUTTON* button);

/**
 * Draws three button
 */
void drawThreeButton(BUTTON* button);

/**
 * Draws four button
 */
void drawFourButton(BUTTON* button);

/**
 * Draws five button
 */
void drawFiveButton(BUTTON* button);

/**
 * Draws six button
 */
void drawSixButton(BUTTON* button);

/**
 * Draws seven button
 */
void drawSevenButton(BUTTON* button);

/**
 * Draws eight button
 */
void drawEightButton(BUTTON* button);

/**
 * Draws nine button
 */
void drawNineButton(BUTTON* button);

/**
 * Draws user default skin button
 */
void drawDefaultSkinButton(USER* userInfo, BUTTON* button);

/**
 * Draws password
 */
void drawPassword(char password[5]);

/**
 * Draws invalid password
 */
void drawInvalidPassword(char password[5]);

/**
 * Draws winning screen
 */
void drawGameWon(void);

/**
 * Draws loosing screen
 */
void drawGameLost(void);
 
#endif // GRAPHICS_H
