#include "graphics.h"
#include "hardware.h"
#include "login.h"
#include "menu.h"

MENU_PAGE menuPage;
MENU_SETTINGS menuSettings;
BUTTON skinSelector;
BUTTON modeSelector;
BUTTON play;
SLIDING_SCALE difficulty;
BUTTON userStats;
BUTTON deleteProfile;
PLAYER_SKIN skins[] = {FIRE_RED, BOLD_ORANGE, STAR_GOLD, EMERALD_GREEN, ICE_BLUE, PLASMA_VIOLET, HOT_PINK};

MENU_SETTINGS* getMenuSettings() {
    return &menuSettings;
}

void menuInit(void) {
    drawMenuBackground();
    if (menuSettings.playerSkin && menuSettings.gameMode && menuSettings.difficulty) {
        drawUserStatsButton(getUserInfo(), &userStats);
        drawDiffScale(&menuSettings, &difficulty);
        drawSSButton(&menuSettings, &skinSelector);
        drawMSButton(&menuSettings, &modeSelector);
        drawPlayButton(&menuSettings, &play);
        return;
    }
    menuPage = MENU_PAGE::MENU_HOME;
    menuSettings.gameMode = GAME_MODE::LEVELS;
    menuSettings.scoreCap = 10;
    menuSettings.playerSkin = getUserInfo()->defaultSkin;
    // Skin selector button
    skinSelector.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    skinSelector.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    skinSelector.boundingBox->topLeft.x = 0;
    skinSelector.boundingBox->topLeft.y = 127 - 36;
    skinSelector.boundingBox->bottomRight.x = 36;
    skinSelector.boundingBox->bottomRight.y = 127;
    // Mode selector button
    modeSelector.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    modeSelector.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    modeSelector.boundingBox->topLeft.x = 91;
    modeSelector.boundingBox->topLeft.y = 127 - 36;
    modeSelector.boundingBox->bottomRight.x = 127;
    modeSelector.boundingBox->bottomRight.y = 127;
    // Play button
    play.buttonStatus = BUTTON_STATUS::SELECTED;
    play.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    play.boundingBox->topLeft.x = 36;
    play.boundingBox->topLeft.y = 127 - 36;
    play.boundingBox->bottomRight.x = 90;
    play.boundingBox->bottomRight.y = 127;
    // Difficulty scale
    difficulty.value = 2;
    difficulty.maxVal = 4;
    menuSettings.difficulty = difficulty.value;
    difficulty.sliderStatus = BUTTON_STATUS::NOT_SELECTED;
    difficulty.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    difficulty.boundingBox->topLeft.x = 0;
    difficulty.boundingBox->topLeft.y = 127 - 43;
    difficulty.boundingBox->bottomRight.x = (int)round(127 * (double)difficulty.value / difficulty.maxVal);
    difficulty.boundingBox->bottomRight.y = 127 - 40;
    // User stats button
    userStats.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    userStats.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    userStats.boundingBox->topLeft.x = 0;
    userStats.boundingBox->topLeft.y = 0;
    userStats.boundingBox->bottomRight.x = 127;
    userStats.boundingBox->bottomRight.y = 0 + 10;
    // Delete user button
    deleteProfile.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    deleteProfile.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    deleteProfile.boundingBox->topLeft.x = 0;
    deleteProfile.boundingBox->topLeft.y = 127 - 17;
    deleteProfile.boundingBox->bottomRight.x = 127;
    deleteProfile.boundingBox->bottomRight.y = 120;
    drawUserStatsButton(getUserInfo(), &userStats);
    drawDiffScale(&menuSettings, &difficulty);
    drawSSButton(&menuSettings, &skinSelector);
    drawMSButton(&menuSettings, &modeSelector);
    drawPlayButton(&menuSettings, &play);
}

int menuUpdate(void) {
    loadMusic();
    GAME_INPUTS* inputs = readInputs();
    if (!inputs->quitGame) {
        buttonSound();
        Timer exit;
        bool exiting = true;
        exit.start();
        while (exiting && exit.elapsed_time().count() < 3000000) {
            loadMusic();
            if (readInputs()->quitGame) {
                exiting = false;
            }
        }
        if (exiting) {
            return 999;
        }
    }
    if (menuPage == MENU_PAGE::MENU_HOME) {
        if (play.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (!inputs->normalAttack) {
                buttonSound();
                while (!readInputs()->normalAttack) loadMusic();
                return 1;
            } else if (inputs->right) {
                play.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                modeSelector.buttonStatus = BUTTON_STATUS::SELECTED;
                drawPlayButton(&menuSettings, &play);
                drawMSButton(&menuSettings, &modeSelector);
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (inputs->left) {
                play.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                skinSelector.buttonStatus = BUTTON_STATUS::SELECTED;
                drawPlayButton(&menuSettings, &play);
                drawSSButton(&menuSettings, &skinSelector);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->up) {
                play.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                difficulty.sliderStatus = BUTTON_STATUS::SELECTED;
                drawPlayButton(&menuSettings, &play);
                drawDiffScale(&menuSettings, &difficulty);
                while (readInputs()->up) loadMusic();
                return 0;
            }
        } else if (skinSelector.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                for (int i = sizeof(skins) - 1; i >= 0; i--) {
                    if (skins[i] == menuSettings.playerSkin) {
                        if (i > 0) {
                            menuSettings.playerSkin = skins[i - 1];
                            getUserInfo()->defaultSkin = skins[i - 1];
                            updateUser(getUserInfo());
                            drawSSButton(&menuSettings, &skinSelector);
                            while (readInputs()->up) loadMusic();
                            return 0;
                        } else {
                            menuSettings.playerSkin = skins[sizeof(skins) - 1];
                            getUserInfo()->defaultSkin = skins[sizeof(skins) - 1];
                            updateUser(getUserInfo());
                            drawSSButton(&menuSettings, &skinSelector);
                            while (readInputs()->up) loadMusic();
                            return 0;
                        }
                    }
                }
            } else if (inputs->down) {
                for (int i = 0; i < sizeof(skins); i++) {
                    if (skins[i] == menuSettings.playerSkin) {
                        if (i < sizeof(skins) - 1) {
                            menuSettings.playerSkin = skins[i + 1];
                            getUserInfo()->defaultSkin = skins[i + 1];
                            updateUser(getUserInfo());
                            drawSSButton(&menuSettings, &skinSelector);
                            while (readInputs()->down) loadMusic();
                            return 0;
                        } else {
                            menuSettings.playerSkin = skins[0];
                            getUserInfo()->defaultSkin = skins[0];
                            updateUser(getUserInfo());
                            drawSSButton(&menuSettings, &skinSelector);
                            while (readInputs()->down) loadMusic();
                            return 0;
                        }
                    }
                }
            } else if (inputs->right) {
                skinSelector.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                play.buttonStatus = BUTTON_STATUS::SELECTED;
                drawSSButton(&menuSettings, &skinSelector);
                drawPlayButton(&menuSettings, &play);
                while (readInputs()->right) loadMusic();
                return 0;
            }
        } else if (modeSelector.buttonStatus == BUTTON_STATUS::SELECTED) {
            GAME_MODE gameModeLayout[] = {LEVELS, SCORECAP, INFINITE};
            if (inputs->up) {
                for (int i = 0; i < 3; i++) {
                    if (menuSettings.gameMode == gameModeLayout[i] && i < 2) {
                        menuSettings.gameMode = gameModeLayout[i + 1];
                        break;
                    } else if (menuSettings.gameMode == gameModeLayout[i]) {
                        menuSettings.gameMode = gameModeLayout[0];
                        break;
                    }
                }
                drawMSButton(&menuSettings, &modeSelector);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                for (int i = 2; i >= 0; i--) {
                    if (menuSettings.gameMode == gameModeLayout[i] && i > 0) {
                        menuSettings.gameMode = gameModeLayout[i - 1];
                        break;
                    } else if (menuSettings.gameMode == gameModeLayout[i]) {
                        menuSettings.gameMode = gameModeLayout[2];
                        break;
                    }
                }
                drawMSButton(&menuSettings, &modeSelector);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                modeSelector.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                play.buttonStatus = BUTTON_STATUS::SELECTED;
                drawMSButton(&menuSettings, &modeSelector);
                drawPlayButton(&menuSettings, &play);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (menuSettings.gameMode == GAME_MODE::SCORECAP && !inputs->normalAttack) {
                buttonSound();
                if (menuSettings.scoreCap < 100) {
                    menuSettings.scoreCap += 10;
                } else {
                    menuSettings.scoreCap = 10;
                }
                drawMSButton(&menuSettings, &modeSelector);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
        } else if (difficulty.sliderStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->down) {
                play.buttonStatus = BUTTON_STATUS::SELECTED;
                difficulty.sliderStatus = BUTTON_STATUS::NOT_SELECTED;
                drawDiffScale(&menuSettings, &difficulty);
                drawPlayButton(&menuSettings, &play);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                if (difficulty.value > 1) {
                    difficulty.value--;
                    menuSettings.difficulty = difficulty.value;
                    difficulty.boundingBox->bottomRight.x = (int)round(127 * (double)difficulty.value / difficulty.maxVal);
                    drawDiffScale(&menuSettings, &difficulty);
                }
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                if (difficulty.value < difficulty.maxVal) {
                    difficulty.value++;
                    menuSettings.difficulty = difficulty.value;
                    difficulty.boundingBox->bottomRight.x = (int)round(127 * (double)difficulty.value / difficulty.maxVal);
                    drawDiffScale(&menuSettings, &difficulty);
                }
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (inputs->up) {
                userStats.buttonStatus = BUTTON_STATUS::SELECTED;
                difficulty.sliderStatus = BUTTON_STATUS::NOT_SELECTED;
                drawDiffScale(&menuSettings, &difficulty);
                drawUserStatsButton(getUserInfo(), &userStats);
                while (readInputs()->up) loadMusic();
                return 0;
            }
        } else if (userStats.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->down) {
                userStats.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                difficulty.sliderStatus = BUTTON_STATUS::SELECTED;
                drawUserStatsButton(getUserInfo(), &userStats);
                drawDiffScale(&menuSettings, &difficulty);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                menuPage = MENU_PAGE::USER_STATS;
                uLCD.cls();
                drawUserStatsBackground(getUserInfo());
                drawUserStatsButton(getUserInfo(), &userStats);
                drawDeleteUserButton(&deleteProfile);
                while (!readInputs()->normalAttack) loadMusic();
                return 0;
            }
        }
        return 0;
    } else if (menuPage == MENU_PAGE::USER_STATS) {
        if (userStats.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (!inputs->normalAttack) {
                buttonSound();
                menuPage = MENU_PAGE::MENU_HOME;
                uLCD.cls();
                drawMenuBackground();
                drawUserStatsButton(getUserInfo(), &userStats);
                drawDiffScale(&menuSettings, &difficulty);
                drawSSButton(&menuSettings, &skinSelector);
                drawMSButton(&menuSettings, &modeSelector);
                drawPlayButton(&menuSettings, &play);
                while(!readInputs()->normalAttack) loadMusic();
                return 0;
            } else if (inputs->down) {
                deleteProfile.buttonStatus = BUTTON_STATUS::SELECTED;
                userStats.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawUserStatsButton(getUserInfo(), &userStats);
                drawDeleteUserButton(&deleteProfile);
                while (readInputs()->down) loadMusic();
                return 0;
            }
        } else if (deleteProfile.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (!inputs->normalAttack) {
                buttonSound();
                Timer exit;
                exit.start();
                bool deletingUser = true;
                while (deletingUser && exit.elapsed_time().count() < 5000000) {
                    loadMusic();
                    if (readInputs()->normalAttack) {
                        deletingUser = false;
                    }
                }
                if (deletingUser) {
                    uLCD.cls();
                    deleteUser(getUserInfo());
                    return 999;
                }
                return 0;
            } else if (inputs->up) {
                deleteProfile.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                userStats.buttonStatus = BUTTON_STATUS::SELECTED;
                drawUserStatsButton(getUserInfo(), &userStats);
                drawDeleteUserButton(&deleteProfile);
                while (readInputs()->up) loadMusic();
                return 0;
            }
        }
        return 0;
    }
    return 0;
}

void deleteMenu(void) {
    menuSettings.playerSkin = PLAYER_SKIN::NONE;
    menuSettings.gameMode = GAME_MODE::LEVELS;
    menuSettings.difficulty = 2;
    free(skinSelector.boundingBox);
    free(modeSelector.boundingBox);
    free(play.boundingBox);
    free(difficulty.boundingBox);
}