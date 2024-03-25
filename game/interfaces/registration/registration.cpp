#include "registration.h"
#include "graphics.h"
#include "users.h"
#include "hardware.h"

REGISTRATION_PAGE pageR;
USER newUser;
BUTTON defaultSkinSelectorR;
BUTTON usernameBaseSelectorR;
BUTTON usernameNumSelectorR;
BUTTON backStepR;
BUTTON nextStepR;
int currentNumButtonR;
BUTTON deleteNumR;
BUTTON zeroR;
BUTTON oneR;
BUTTON twoR;
BUTTON threeR;
BUTTON fourR;
BUTTON fiveR;
BUTTON sixR;
BUTTON sevenR;
BUTTON eightR;
BUTTON nineR;
PLAYER_SKIN defaultSkins[] = {FIRE_RED, BOLD_ORANGE, STAR_GOLD, EMERALD_GREEN, ICE_BLUE, PLASMA_VIOLET, HOT_PINK};

USER* getNewUser(void) {
    return &newUser;
}

void registrationInit(void) {
    pageR = REGISTRATION_PAGE::USERNAME;
    // Default values for new user
    strcpy(newUser.username, getUsernameList()[0]);
    newUser.userNum = 0;
    for (int i = 0; i < 4; i++) {
        newUser.password[i] = {NULL,};
    }
    newUser.password[4] = '\0';
    newUser.defaultSkin = PLAYER_SKIN::ICE_BLUE;
    newUser.highScore = 0;
    newUser.totalEnemiesKilled = 0;
    newUser.totalPlayerDeaths = 0;
    newUser.totalPoints = 0;
    newUser.totalPlayTime = 0;
    // Default skin selector button
    defaultSkinSelectorR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    defaultSkinSelectorR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    defaultSkinSelectorR.boundingBox->topLeft.x = 0;
    defaultSkinSelectorR.boundingBox->topLeft.y = 127 - 54;
    defaultSkinSelectorR.boundingBox->bottomRight.x = 127;
    defaultSkinSelectorR.boundingBox->bottomRight.y = 127 - 37;
    // Name number selector button
    usernameNumSelectorR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    usernameNumSelectorR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    usernameNumSelectorR.boundingBox->topLeft.x = 93;
    usernameNumSelectorR.boundingBox->topLeft.y = 127 - 54;
    usernameNumSelectorR.boundingBox->bottomRight.x = 127;
    usernameNumSelectorR.boundingBox->bottomRight.y = 127 - 37;
    // Name base button
    usernameBaseSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
    usernameBaseSelectorR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    usernameBaseSelectorR.boundingBox->topLeft.x = 0;
    usernameBaseSelectorR.boundingBox->topLeft.y = 127 - 54;
    usernameBaseSelectorR.boundingBox->bottomRight.x = 93;
    usernameBaseSelectorR.boundingBox->bottomRight.y = 127 - 37;
    // Back to login button
    backStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    backStepR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    backStepR.boundingBox->topLeft.x = 0;
    backStepR.boundingBox->topLeft.y = 0;
    backStepR.boundingBox->bottomRight.x = 25;
    backStepR.boundingBox->bottomRight.y = 25;
    // Next step button
    nextStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    nextStepR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    nextStepR.boundingBox->topLeft.x = 127 - 25;
    nextStepR.boundingBox->topLeft.y = 0;
    nextStepR.boundingBox->bottomRight.x = 127;
    nextStepR.boundingBox->bottomRight.y = 25;
    // Default password num
    currentNumButtonR = 0;
    // Password delete button
    deleteNumR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    deleteNumR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    deleteNumR.boundingBox->topLeft.x = 76;
    deleteNumR.boundingBox->topLeft.y = 127 - 25;
    deleteNumR.boundingBox->bottomRight.x = 101;
    deleteNumR.boundingBox->bottomRight.y = 127;
    // Password zero button
    zeroR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    zeroR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    zeroR.boundingBox->topLeft.x = 51;
    zeroR.boundingBox->topLeft.y = 127 - 25;
    zeroR.boundingBox->bottomRight.x = 76;
    zeroR.boundingBox->bottomRight.y = 127;
    // Password one button
    oneR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    oneR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    oneR.boundingBox->topLeft.x = 26;
    oneR.boundingBox->topLeft.y = 127 - 50;
    oneR.boundingBox->bottomRight.x = 51;
    oneR.boundingBox->bottomRight.y = 127 - 25;
    // Password two button
    twoR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    twoR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    twoR.boundingBox->topLeft.x = 51;
    twoR.boundingBox->topLeft.y = 127 - 50;
    twoR.boundingBox->bottomRight.x = 76;
    twoR.boundingBox->bottomRight.y = 127 - 25;
    // Password three button
    threeR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    threeR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    threeR.boundingBox->topLeft.x = 76;
    threeR.boundingBox->topLeft.y = 127 - 50;
    threeR.boundingBox->bottomRight.x = 101;
    threeR.boundingBox->bottomRight.y = 127 - 25;
    // Password four button
    fourR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    fourR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    fourR.boundingBox->topLeft.x = 26;
    fourR.boundingBox->topLeft.y = 127 - 75;
    fourR.boundingBox->bottomRight.x = 51;
    fourR.boundingBox->bottomRight.y = 127 - 50;
    // Password five button
    fiveR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    fiveR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    fiveR.boundingBox->topLeft.x = 51;
    fiveR.boundingBox->topLeft.y = 127 - 75;
    fiveR.boundingBox->bottomRight.x = 76;
    fiveR.boundingBox->bottomRight.y = 127 - 50;
    // Password six button
    sixR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    sixR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    sixR.boundingBox->topLeft.x = 76;
    sixR.boundingBox->topLeft.y = 127 - 75;
    sixR.boundingBox->bottomRight.x = 101;
    sixR.boundingBox->bottomRight.y = 127 - 50;
    // Password seven button
    sevenR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    sevenR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    sevenR.boundingBox->topLeft.x = 26;
    sevenR.boundingBox->topLeft.y = 127 - 100;
    sevenR.boundingBox->bottomRight.x = 51;
    sevenR.boundingBox->bottomRight.y = 127 - 75;
    // Password eight button
    eightR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    eightR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    eightR.boundingBox->topLeft.x = 51;
    eightR.boundingBox->topLeft.y = 127 - 100;
    eightR.boundingBox->bottomRight.x = 76;
    eightR.boundingBox->bottomRight.y = 127 - 75;
    // Password nine button
    nineR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    nineR.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    nineR.boundingBox->topLeft.x = 76;
    nineR.boundingBox->topLeft.y = 127 - 100;
    nineR.boundingBox->bottomRight.x = 101;
    nineR.boundingBox->bottomRight.y = 127 - 75;
    uLCD.cls();
    drawRegistrationBackground();
    drawUserBaseButton(&newUser, &usernameBaseSelectorR);
    drawUserNumButton(&newUser, &usernameNumSelectorR);
    drawBackButton(&backStepR);
    drawNextButton(&nextStepR);
}

/** Update the login screen. Returns -1 to go back to login page 
 */
int registrationUpdate(void) {
    GAME_INPUTS* inputs = readInputs();
    if (!inputs->quitGame) {
        Timer exit;
        bool exiting = true;
        exit.start();
        while (exiting && exit.elapsed_time().count() < 3000000) {
            if (readInputs()->quitGame) {
                exiting = false;
            }
        }
        if (exiting) {
            return 999;
        }
    }
    switch (pageR) {
    // Username page
    case REGISTRATION_PAGE::USERNAME:
        if (backStepR.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (!inputs->normalAttack) {
                usernameBaseSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
                backStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                // Reset new user data
                strcpy(newUser.username, getUsernameList()[0]);
                newUser.userNum = 0;
                for (int i = 0; i < 4; i++) {
                    newUser.password[i] = {NULL,};
                }
                newUser.defaultSkin = PLAYER_SKIN::ICE_BLUE;
                while (!readInputs()->normalAttack);
                uLCD.cls();
                return -1;
            } else if (inputs->right) {
                usernameBaseSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
                backStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawUserBaseButton(&newUser, &usernameBaseSelectorR);
                drawBackButton(&backStepR);
                while (readInputs()->right);
                return 0;
            }
        } else if (usernameBaseSelectorR.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                for (int i = 15 - 1; i >= 0; i--) {
                    bool matchingNames = true;
                    int k = 0;
                    while (getUsernameList()[i][k] && newUser.username[k]) {
                        if (getUsernameList()[i][k] != newUser.username[k]) {
                            matchingNames = false;
                            break;
                        }
                        k++;
                    }
                    if (matchingNames) {
                        if (i > 0) {
                            strcpy(newUser.username, getUsernameList()[i - 1]);
                            drawUserBaseButton(&newUser, &usernameBaseSelectorR);
                            while (readInputs()->up);
                            return 0;
                        } else {
                            strcpy(newUser.username, getUsernameList()[15 - 1]);
                            drawUserBaseButton(&newUser, &usernameBaseSelectorR);
                            while (readInputs()->up);
                            return 0;
                        }
                    }
                }
            } else if (inputs->down) {
                for (int i = 0; i < 15; i++) {
                    bool matchingNames = true;
                    int k = 0;
                    while (getUsernameList()[i][k] && newUser.username[k]) {
                        if (getUsernameList()[i][k] != newUser.username[k]) {
                            matchingNames = false;
                            break;
                        }
                        k++;
                    }
                    if (matchingNames) {
                        if (i < 15 - 1) {
                            strcpy(newUser.username, getUsernameList()[i + 1]);
                            drawUserBaseButton(&newUser, &usernameBaseSelectorR);
                            while (readInputs()->down);
                            return 0;
                        } else {
                            strcpy(newUser.username, getUsernameList()[0]);
                            drawUserBaseButton(&newUser, &usernameBaseSelectorR);
                            while (readInputs()->down);
                            return 0;
                        }
                    }
                }
            } else if (inputs->left) {
                usernameBaseSelectorR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                backStepR.buttonStatus = BUTTON_STATUS::SELECTED;
                drawUserBaseButton(&newUser, &usernameBaseSelectorR);
                drawBackButton(&backStepR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                usernameBaseSelectorR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                usernameNumSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
                drawUserBaseButton(&newUser, &usernameBaseSelectorR);
                drawUserNumButton(&newUser, &usernameNumSelectorR);
                while (readInputs()->right);
                return 0;
            }
        } else if (usernameNumSelectorR.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                if (newUser.userNum < 99) newUser.userNum++;
                else newUser.userNum = 0;
                drawUserNumButton(&newUser, &usernameNumSelectorR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                if (newUser.userNum > 0) newUser.userNum--;
                else newUser.userNum = 99;
                drawUserNumButton(&newUser, &usernameNumSelectorR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                usernameBaseSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
                usernameNumSelectorR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawUserBaseButton(&newUser, &usernameBaseSelectorR);
                drawUserNumButton(&newUser, &usernameNumSelectorR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                nextStepR.buttonStatus = BUTTON_STATUS::SELECTED;
                usernameNumSelectorR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNextButton(&nextStepR);
                drawUserNumButton(&newUser, &usernameNumSelectorR);
                while (readInputs()->right);
                return 0;
            }
        } else if (nextStepR.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->left) {
                nextStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                usernameNumSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
                drawNextButton(&nextStepR);
                drawUserNumButton(&newUser, &usernameNumSelectorR);
                while (readInputs()->left);
                return 0;
            } else if (!inputs->normalAttack) {
                // Only procede to password setup if username is not already taken
                if (!userExists(newUser.username, newUser.userNum)) {
                    drawInvalidNextButton(&nextStepR);
                    while (!readInputs()->normalAttack);
                    return 0;
                }
                nextStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                zeroR.buttonStatus = BUTTON_STATUS::SELECTED;
                currentNumButtonR = 0;
                pageR = REGISTRATION_PAGE::PASSWORD;
                // Draw all the buttons on the next page
                uLCD.cls();
                uLCD.filled_rectangle(0, 26, 127, 127, DGREY);
                drawPassword(newUser.password);
                drawBackButton(&backStepR);
                drawNextButton(&nextStepR);
                drawDeleteButton(&deleteNumR);
                drawZeroButton(&zeroR);
                drawOneButton(&oneR);
                drawTwoButton(&twoR);
                drawThreeButton(&threeR);
                drawFourButton(&fourR);
                drawFiveButton(&fiveR);
                drawSixButton(&sixR);
                drawSevenButton(&sevenR);
                drawEightButton(&eightR);
                drawNineButton(&nineR);
                while (!readInputs()->normalAttack);
                return 0;
            }
        }
        break;
    // Password page
    case REGISTRATION_PAGE::PASSWORD:
        if (backStepR.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = 7;
                sevenR.buttonStatus = BUTTON_STATUS::SELECTED;
                backStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawBackButton(&backStepR);
                drawSevenButton(&sevenR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                return 0;
            } else if (inputs->right) {
                nextStepR.buttonStatus = BUTTON_STATUS::SELECTED;
                backStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawBackButton(&backStepR);
                drawNextButton(&nextStepR);
                while (readInputs()->right);
                return 0;
            } else if (!inputs->normalAttack) {
                pageR = REGISTRATION_PAGE::USERNAME;
                backStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                usernameBaseSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
                // Draw all the buttons on the prev page
                uLCD.cls();
                drawRegistrationBackground();
                drawUserBaseButton(&newUser, &usernameBaseSelectorR);
                drawUserNumButton(&newUser, &usernameNumSelectorR);
                drawBackButton(&backStepR);
                drawNextButton(&nextStepR);
                while (!readInputs()->normalAttack);
                return 0;
            }
        } else if (nextStepR.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = 9;
                nineR.buttonStatus = BUTTON_STATUS::SELECTED;
                nextStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNextButton(&nextStepR);
                drawNineButton(&nineR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                backStepR.buttonStatus = BUTTON_STATUS::SELECTED;
                nextStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNextButton(&nextStepR);
                drawBackButton(&backStepR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                return 0;
            } else if (!inputs->normalAttack) {
                // Need to check whether password is 4 numbers long before going to next page
                if (!newUser.password[3]) {
                    drawInvalidNextButton(&nextStepR);
                    drawInvalidPassword(newUser.password);
                    while (!readInputs()->normalAttack);
                    return 0;
                }
                pageR = REGISTRATION_PAGE::DEFAULT_SKIN;
                nextStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                defaultSkinSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
                // Draw all the buttons on the next page
                uLCD.cls();
                drawDefaultSkinButton(&newUser, &defaultSkinSelectorR);
                drawBackButton(&backStepR);
                drawNextButton(&nextStepR);
                while (!readInputs()->normalAttack);
                return 0;
            }
        } else if (deleteNumR.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                currentNumButtonR = 3;
                threeR.buttonStatus = BUTTON_STATUS::SELECTED;
                deleteNumR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawDeleteButton(&deleteNumR);
                drawThreeButton(&threeR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                return 0;
            } else if (inputs->left) {
                currentNumButtonR = 0;
                zeroR.buttonStatus = BUTTON_STATUS::SELECTED;
                deleteNumR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawDeleteButton(&deleteNumR);
                drawZeroButton(&zeroR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 4 - 1; i >= 0; i--) {
                    if (newUser.password[i] != NULL) {
                        newUser.password[i] = NULL;
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
        }
        // Password numbers
        switch(currentNumButtonR) {
        case 0:
            if (inputs->up) {
                currentNumButtonR = 2;
                twoR.buttonStatus = BUTTON_STATUS::SELECTED;
                zeroR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawZeroButton(&zeroR);
                drawTwoButton(&twoR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                return 0;
            } else if (inputs->left) {
                currentNumButtonR = 1;
                oneR.buttonStatus = BUTTON_STATUS::SELECTED;
                zeroR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawZeroButton(&zeroR);
                drawOneButton(&oneR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                currentNumButtonR = -1;
                deleteNumR.buttonStatus = BUTTON_STATUS::SELECTED;
                zeroR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawZeroButton(&zeroR);
                drawDeleteButton(&deleteNumR);
                while (readInputs()->right);
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 0; i < 4; i++) {
                    if (newUser.password[i] == NULL) {
                        newUser.password[i] = '0';
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
            break;
        case 1:
            if (inputs->up) {
                currentNumButtonR = 4;
                fourR.buttonStatus = BUTTON_STATUS::SELECTED;
                oneR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawOneButton(&oneR);
                drawFourButton(&fourR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = 0;
                zeroR.buttonStatus = BUTTON_STATUS::SELECTED;
                oneR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawOneButton(&oneR);
                drawZeroButton(&zeroR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                return 0;
            } else if (inputs->right) {
                currentNumButtonR = 2;
                twoR.buttonStatus = BUTTON_STATUS::SELECTED;
                oneR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawOneButton(&oneR);
                drawTwoButton(&twoR);
                while (readInputs()->right);
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 0; i < 4; i++) {
                    if (newUser.password[i] == NULL) {
                        newUser.password[i] = '1';
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
            break;
        case 2:
            if (inputs->up) {
                currentNumButtonR = 5;
                fiveR.buttonStatus = BUTTON_STATUS::SELECTED;
                twoR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawTwoButton(&twoR);
                drawFiveButton(&fiveR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = 0;
                zeroR.buttonStatus = BUTTON_STATUS::SELECTED;
                twoR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawTwoButton(&twoR);
                drawZeroButton(&zeroR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                currentNumButtonR = 1;
                oneR.buttonStatus = BUTTON_STATUS::SELECTED;
                twoR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawTwoButton(&twoR);
                drawOneButton(&oneR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                currentNumButtonR = 3;
                threeR.buttonStatus = BUTTON_STATUS::SELECTED;
                twoR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawTwoButton(&twoR);
                drawThreeButton(&threeR);
                while (readInputs()->right);
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 0; i < 4; i++) {
                    if (newUser.password[i] == NULL) {
                        newUser.password[i] = '2';
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
            break;
        case 3:
            if (inputs->up) {
                currentNumButtonR = 6;
                sixR.buttonStatus = BUTTON_STATUS::SELECTED;
                threeR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawThreeButton(&threeR);
                drawSixButton(&sixR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = -1;
                deleteNumR.buttonStatus = BUTTON_STATUS::SELECTED;
                threeR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawThreeButton(&threeR);
                drawDeleteButton(&deleteNumR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                currentNumButtonR = 2;
                twoR.buttonStatus = BUTTON_STATUS::SELECTED;
                threeR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawThreeButton(&threeR);
                drawTwoButton(&twoR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 0; i < 4; i++) {
                    if (newUser.password[i] == NULL) {
                        newUser.password[i] = '3';
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
            break;
        case 4:
            if (inputs->up) {
                currentNumButtonR = 7;
                sevenR.buttonStatus = BUTTON_STATUS::SELECTED;
                fourR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFourButton(&fourR);
                drawSevenButton(&sevenR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = 1;
                oneR.buttonStatus = BUTTON_STATUS::SELECTED;
                fourR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFourButton(&fourR);
                drawOneButton(&oneR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                return 0;
            } else if (inputs->right) {
                currentNumButtonR = 5;
                fiveR.buttonStatus = BUTTON_STATUS::SELECTED;
                fourR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFourButton(&fourR);
                drawFiveButton(&fiveR);
                while (readInputs()->right);
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 0; i < 4; i++) {
                    if (newUser.password[i] == NULL) {
                        newUser.password[i] = '4';
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
            break;
        case 5:
            if (inputs->up) {
                currentNumButtonR = 8;
                eightR.buttonStatus = BUTTON_STATUS::SELECTED;
                fiveR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFiveButton(&fiveR);
                drawEightButton(&eightR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = 2;
                twoR.buttonStatus = BUTTON_STATUS::SELECTED;
                fiveR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFiveButton(&fiveR);
                drawTwoButton(&twoR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                currentNumButtonR = 4;
                fourR.buttonStatus = BUTTON_STATUS::SELECTED;
                fiveR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFiveButton(&fiveR);
                drawFourButton(&fourR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                currentNumButtonR = 6;
                sixR.buttonStatus = BUTTON_STATUS::SELECTED;
                fiveR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFiveButton(&fiveR);
                drawSixButton(&sixR);
                while (readInputs()->right);
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 0; i < 4; i++) {
                    if (newUser.password[i] == NULL) {
                        newUser.password[i] = '5';
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
            break;
        case 6:
            if (inputs->up) {
                currentNumButtonR = 9;
                nineR.buttonStatus = BUTTON_STATUS::SELECTED;
                sixR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSixButton(&sixR);
                drawNineButton(&nineR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = 3;
                threeR.buttonStatus = BUTTON_STATUS::SELECTED;
                sixR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSixButton(&sixR);
                drawThreeButton(&threeR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                currentNumButtonR = 5;
                fiveR.buttonStatus = BUTTON_STATUS::SELECTED;
                sixR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSixButton(&sixR);
                drawFiveButton(&fiveR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 0; i < 4; i++) {
                    if (newUser.password[i] == NULL) {
                        newUser.password[i] = '6';
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
            break;
        case 7:
            if (inputs->up) {
                currentNumButtonR = -1;
                backStepR.buttonStatus = BUTTON_STATUS::SELECTED;
                sevenR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSevenButton(&sevenR);
                drawBackButton(&backStepR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = 4;
                fourR.buttonStatus = BUTTON_STATUS::SELECTED;
                sevenR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSevenButton(&sevenR);
                drawFourButton(&fourR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                currentNumButtonR = -1;
                backStepR.buttonStatus = BUTTON_STATUS::SELECTED;
                sevenR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSevenButton(&sevenR);
                drawBackButton(&backStepR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                currentNumButtonR = 8;
                eightR.buttonStatus = BUTTON_STATUS::SELECTED;
                sevenR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSevenButton(&sevenR);
                drawEightButton(&eightR);
                while (readInputs()->right);
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 0; i < 4; i++) {
                    if (newUser.password[i] == NULL) {
                        newUser.password[i] = '7';
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
            break;
        case 8:
            if (inputs->up) {
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = 5;
                fiveR.buttonStatus = BUTTON_STATUS::SELECTED;
                eightR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawEightButton(&eightR);
                drawFiveButton(&fiveR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                currentNumButtonR = 7;
                sevenR.buttonStatus = BUTTON_STATUS::SELECTED;
                eightR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawEightButton(&eightR);
                drawSevenButton(&sevenR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                currentNumButtonR = 9;
                nineR.buttonStatus = BUTTON_STATUS::SELECTED;
                eightR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawEightButton(&eightR);
                drawNineButton(&nineR);
                while (readInputs()->right);
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 0; i < 4; i++) {
                    if (newUser.password[i] == NULL) {
                        newUser.password[i] = '8';
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
            break;
        case 9:
            if (inputs->up) {
                currentNumButtonR = -1;
                nextStepR.buttonStatus = BUTTON_STATUS::SELECTED;
                nineR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNineButton(&nineR);
                drawNextButton(&nextStepR);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                currentNumButtonR = 6;
                sixR.buttonStatus = BUTTON_STATUS::SELECTED;
                nineR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNineButton(&nineR);
                drawSixButton(&sixR);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                currentNumButtonR = 8;
                eightR.buttonStatus = BUTTON_STATUS::SELECTED;
                nineR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNineButton(&nineR);
                drawEightButton(&eightR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                currentNumButtonR = -1;
                nextStepR.buttonStatus = BUTTON_STATUS::SELECTED;
                nineR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNineButton(&nineR);
                drawNextButton(&nextStepR);
                while (readInputs()->right);
                return 0;
            } else if (!inputs->normalAttack) {
                for (int i = 0; i < 4; i++) {
                    if (newUser.password[i] == NULL) {
                        newUser.password[i] = '9';
                        break;
                    }
                }
                drawPassword(newUser.password);
                while (!readInputs()->normalAttack);
                wait_us(250000);
                return 0;
            }
            break;
        }
        break;
    // Default skin selection
    case REGISTRATION_PAGE::DEFAULT_SKIN:
        if (defaultSkinSelectorR.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                for (int i = sizeof(defaultSkins) - 1; i >= 0; i--) {
                    if (defaultSkins[i] == newUser.defaultSkin) {
                        if (i > 0) {
                            newUser.defaultSkin = defaultSkins[--i];
                            drawDefaultSkinButton(&newUser, &defaultSkinSelectorR);
                            while (readInputs()->up);
                            return 0;
                        } else {
                            newUser.defaultSkin = defaultSkins[sizeof(defaultSkins) - 1];
                            drawDefaultSkinButton(&newUser, &defaultSkinSelectorR);
                            while (readInputs()->up);
                            return 0;
                        }
                    }
                }
            } else if (inputs->down) {
                for (int i = 0; i < sizeof(defaultSkins); i++) {
                    if (defaultSkins[i] == newUser.defaultSkin) {
                        if (i < sizeof(defaultSkins) - 1) {
                            newUser.defaultSkin = defaultSkins[++i];
                            drawDefaultSkinButton(&newUser, &defaultSkinSelectorR);
                            while (readInputs()->down);
                            return 0;
                        } else {
                            newUser.defaultSkin = defaultSkins[0];
                            drawDefaultSkinButton(&newUser, &defaultSkinSelectorR);
                            while (readInputs()->down);
                            return 0;
                        }
                    }
                }
            } else if (inputs->left) {
                defaultSkinSelectorR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                backStepR.buttonStatus = BUTTON_STATUS::SELECTED;
                drawDefaultSkinButton(&newUser, &defaultSkinSelectorR);
                drawBackButton(&backStepR);
                while (readInputs()->left);
                return 0;
            } else if (inputs->right) {
                defaultSkinSelectorR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                nextStepR.buttonStatus = BUTTON_STATUS::SELECTED;
                drawDefaultSkinButton(&newUser, &defaultSkinSelectorR);
                drawNextButton(&nextStepR);
                while (readInputs()->right);
                return 0;
            }
        } else if (backStepR.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->right) {
                defaultSkinSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
                backStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawBackButton(&backStepR);
                drawDefaultSkinButton(&newUser, &defaultSkinSelectorR);
                while (readInputs()->right);
                return 0;
            } else if (!inputs->normalAttack) {
                pageR = REGISTRATION_PAGE::PASSWORD;
                backStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                zeroR.buttonStatus = BUTTON_STATUS::SELECTED;
                currentNumButtonR = 0;
                // Draw all buttons on prev page
                uLCD.cls();
                uLCD.filled_rectangle(0, 26, 127, 127, DGREY);
                drawPassword(newUser.password);
                drawBackButton(&backStepR);
                drawNextButton(&nextStepR);
                drawDeleteButton(&deleteNumR);
                drawZeroButton(&zeroR);
                drawOneButton(&oneR);
                drawTwoButton(&twoR);
                drawThreeButton(&threeR);
                drawFourButton(&fourR);
                drawFiveButton(&fiveR);
                drawSixButton(&sixR);
                drawSevenButton(&sevenR);
                drawEightButton(&eightR);
                drawNineButton(&nineR);
                while (!readInputs()->normalAttack);
                return 0;
            }
        } else if (nextStepR.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->left) {
                defaultSkinSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
                nextStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNextButton(&nextStepR);
                drawDefaultSkinButton(&newUser, &defaultSkinSelectorR);
                while (readInputs()->left);
                return 0;
            } else if (!inputs->normalAttack) {
                registerNewUser(&newUser);
                pageR = REGISTRATION_PAGE::USERNAME;
                nextStepR.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                usernameBaseSelectorR.buttonStatus = BUTTON_STATUS::SELECTED;
                // Go back to the login page
                while (!readInputs()->normalAttack);
                uLCD.cls();
                return -1;
            }
        }
        break;
    }
    return 0;
}

void deleteRegistration(void) {
    pageR = REGISTRATION_PAGE::USERNAME;
    // Default values for new user
    strcpy(newUser.username, getUsernameList()[0]);
    newUser.userNum = 0;
    for (int i = 0; i < 4; i++) {
        newUser.password[i] = {NULL,};
    }
    newUser.password[4] = '\0';
    newUser.defaultSkin = PLAYER_SKIN::ICE_BLUE;
    newUser.highScore = 0;
    newUser.totalEnemiesKilled = 0;
    newUser.totalPlayerDeaths = 0;
    newUser.totalPoints = 0;
    newUser.totalPlayTime = 0;
    free(defaultSkinSelectorR.boundingBox);
    free(usernameBaseSelectorR.boundingBox);
    free(usernameNumSelectorR.boundingBox);
    free(backStepR.boundingBox);
    free(nextStepR.boundingBox);
    free(deleteNumR.boundingBox);
    free(zeroR.boundingBox);
    free(oneR.boundingBox);
    free(twoR.boundingBox);
    free(threeR.boundingBox);
    free(fourR.boundingBox);
    free(fiveR.boundingBox);
    free(sixR.boundingBox);
    free(sevenR.boundingBox);
    free(eightR.boundingBox);
    free(nineR.boundingBox);
    uLCD.cls();
}

void registerNewUser(USER* newUser) {
    // Create a new txt file to store user info
    // File name format: uName_uNum_pWord.txt
    char newUserFileName[100] = "/sd/Users/";
    strcat(newUserFileName, newUser->username);
    strcat(newUserFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", newUser->userNum);
    strcat(newUserFileName, strUserNum);
    strcat(newUserFileName, "_");
    strcat(newUserFileName, newUser->password);
    strcat(newUserFileName, ".txt");
    FILE* newUserFile = fopen(newUserFileName, "w");
    if (newUserFile == NULL) {
        printf("Error, could not open file.\n");
        return;
    }
    // User data format: dSkin, hScore, eKilled, pDeaths, tPoints, tTime
    fprintf(newUserFile, "%c, %d, %d, %d, %d, %d", newUser->defaultSkin, newUser->highScore, newUser->totalEnemiesKilled, newUser->totalPlayerDeaths, newUser->totalPoints, newUser->totalPlayTime);
    fclose(newUserFile);
}
