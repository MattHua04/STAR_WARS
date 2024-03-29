#include "gameMusic.h"
#include "menu.h"
#include "registration.h"
#include "graphics.h"
#include "users.h"
#include "hardware.h"

REGISTRATION_PAGE page;
USER userInfo;
BUTTON usernameBaseSelector;
BUTTON usernameNumSelector;
BUTTON loginUser;
BUTTON goToRegistration;
BUTTON backStep;
BUTTON nextStep;
int currentNumButton;
BUTTON deleteNum;
BUTTON zero;
BUTTON one;
BUTTON two;
BUTTON three;
BUTTON four;
BUTTON five;
BUTTON six;
BUTTON seven;
BUTTON eight;
BUTTON nine;

USER* getUserInfo() {
    return &userInfo;
}

void loginInit(void) {
    if (getMenuSettings()->playerSkin) {
        deleteMenu();
    }
    page = REGISTRATION_PAGE::USERNAME;
    // Default user values before login
    strcpy(userInfo.username, getUsernameList()[0]);
    userInfo.userNum = 0;
    for (int i = 0; i < 4; i++) {
        userInfo.password[i] = NULL;
    }
    userInfo.password[4] = '\0';
    userInfo.defaultSkin = PLAYER_SKIN::ICE_BLUE;
    // Name number selector button
    usernameNumSelector.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    usernameNumSelector.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    usernameNumSelector.boundingBox->topLeft.x = 93;
    usernameNumSelector.boundingBox->topLeft.y = 127 - 54;
    usernameNumSelector.boundingBox->bottomRight.x = 127;
    usernameNumSelector.boundingBox->bottomRight.y = 127 - 37;
    // Name base button
    usernameBaseSelector.buttonStatus = BUTTON_STATUS::SELECTED;
    usernameBaseSelector.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    usernameBaseSelector.boundingBox->topLeft.x = 0;
    usernameBaseSelector.boundingBox->topLeft.y = 127 - 54;
    usernameBaseSelector.boundingBox->bottomRight.x = 93;
    usernameBaseSelector.boundingBox->bottomRight.y = 127 - 37;
    // Login button
    loginUser.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    loginUser.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    loginUser.boundingBox->topLeft.x = 0;
    loginUser.boundingBox->topLeft.y = 127 - 37;
    loginUser.boundingBox->bottomRight.x = 127;
    loginUser.boundingBox->bottomRight.y = 127 - 20;
    // Register button
    goToRegistration.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    goToRegistration.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    goToRegistration.boundingBox->topLeft.x = 0;
    goToRegistration.boundingBox->topLeft.y = 127 - 20;
    goToRegistration.boundingBox->bottomRight.x = 127;
    goToRegistration.boundingBox->bottomRight.y = 127 - 3;
    // Back to login button
    backStep.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    backStep.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    backStep.boundingBox->topLeft.x = 0;
    backStep.boundingBox->topLeft.y = 0;
    backStep.boundingBox->bottomRight.x = 25;
    backStep.boundingBox->bottomRight.y = 25;
    // Next step button
    nextStep.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    nextStep.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    nextStep.boundingBox->topLeft.x = 127 - 25;
    nextStep.boundingBox->topLeft.y = 0;
    nextStep.boundingBox->bottomRight.x = 127;
    nextStep.boundingBox->bottomRight.y = 25;
    // Default password num
    currentNumButton = 0;
    // Password delete button
    deleteNum.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    deleteNum.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    deleteNum.boundingBox->topLeft.x = 76;
    deleteNum.boundingBox->topLeft.y = 127 - 25;
    deleteNum.boundingBox->bottomRight.x = 101;
    deleteNum.boundingBox->bottomRight.y = 127;
    // Password zero button
    zero.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    zero.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    zero.boundingBox->topLeft.x = 51;
    zero.boundingBox->topLeft.y = 127 - 25;
    zero.boundingBox->bottomRight.x = 76;
    zero.boundingBox->bottomRight.y = 127;
    // Password one button
    one.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    one.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    one.boundingBox->topLeft.x = 26;
    one.boundingBox->topLeft.y = 127 - 50;
    one.boundingBox->bottomRight.x = 51;
    one.boundingBox->bottomRight.y = 127 - 25;
    // Password two button
    two.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    two.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    two.boundingBox->topLeft.x = 51;
    two.boundingBox->topLeft.y = 127 - 50;
    two.boundingBox->bottomRight.x = 76;
    two.boundingBox->bottomRight.y = 127 - 25;
    // Password three button
    three.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    three.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    three.boundingBox->topLeft.x = 76;
    three.boundingBox->topLeft.y = 127 - 50;
    three.boundingBox->bottomRight.x = 101;
    three.boundingBox->bottomRight.y = 127 - 25;
    // Password four button
    four.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    four.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    four.boundingBox->topLeft.x = 26;
    four.boundingBox->topLeft.y = 127 - 75;
    four.boundingBox->bottomRight.x = 51;
    four.boundingBox->bottomRight.y = 127 - 50;
    // Password five button
    five.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    five.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    five.boundingBox->topLeft.x = 51;
    five.boundingBox->topLeft.y = 127 - 75;
    five.boundingBox->bottomRight.x = 76;
    five.boundingBox->bottomRight.y = 127 - 50;
    // Password six button
    six.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    six.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    six.boundingBox->topLeft.x = 76;
    six.boundingBox->topLeft.y = 127 - 75;
    six.boundingBox->bottomRight.x = 101;
    six.boundingBox->bottomRight.y = 127 - 50;
    // Password seven button
    seven.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    seven.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    seven.boundingBox->topLeft.x = 26;
    seven.boundingBox->topLeft.y = 127 - 100;
    seven.boundingBox->bottomRight.x = 51;
    seven.boundingBox->bottomRight.y = 127 - 75;
    // Password eight button
    eight.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    eight.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    eight.boundingBox->topLeft.x = 51;
    eight.boundingBox->topLeft.y = 127 - 100;
    eight.boundingBox->bottomRight.x = 76;
    eight.boundingBox->bottomRight.y = 127 - 75;
    // Password nine button
    nine.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
    nine.boundingBox = (BOUNDINGBOX*)malloc(sizeof(BOUNDINGBOX));
    nine.boundingBox->topLeft.x = 76;
    nine.boundingBox->topLeft.y = 127 - 100;
    nine.boundingBox->bottomRight.x = 101;
    nine.boundingBox->bottomRight.y = 127 - 75;
    uLCD.cls();
    drawLoginBackground();
    drawLoginButton(&loginUser);
    drawRegisterButton(&goToRegistration);
    drawUserNumButton(&userInfo, &usernameNumSelector);
    drawUserBaseButton(&userInfo, &usernameBaseSelector);
}

/** Update the login screen
 */
int loginUpdate(void) {
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
    } else if (!inputs->superAttack) {
        buttonSound();
        getMenuSettings()->volume -= (getMenuSettings()->volume > 0) ? 0.05 : 0;
        while (!readInputs()->superAttack) loadMusic();
        return 0;
    } else if (!inputs->pauseResume) {
        buttonSound();
        getMenuSettings()->volume += (getMenuSettings()->volume < 1) ? 0.05 : 0;
        while (!readInputs()->pauseResume) loadMusic();
        return 0;
    }
    switch (page) {
    // Username page
    case REGISTRATION_PAGE::USERNAME:
        if (usernameBaseSelector.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                for (int i = 15 - 1; i >= 0; i--) {
                    bool matchingNames = true;
                    int k = 0;
                    while (getUsernameList()[i][k] && userInfo.username[k]) {
                        if (getUsernameList()[i][k] != userInfo.username[k]) {
                            matchingNames = false;
                            break;
                        }
                        k++;
                    }
                    if (matchingNames) {
                        if (i > 0) {
                            strcpy(userInfo.username, getUsernameList()[i - 1]);
                            drawUserBaseButton(&userInfo, &usernameBaseSelector);
                            while (readInputs()->up) loadMusic();
                            return 0;
                        } else {
                            strcpy(userInfo.username, getUsernameList()[15 - 1]);
                            drawUserBaseButton(&userInfo, &usernameBaseSelector);
                            while (readInputs()->up) loadMusic();
                            return 0;
                        }
                    }
                }
            } else if (inputs->down) {
                for (int i = 0; i < 15; i++) {
                    bool matchingNames = true;
                    int k = 0;
                    while (getUsernameList()[i][k] && userInfo.username[k]) {
                        if (getUsernameList()[i][k] != userInfo.username[k]) {
                            matchingNames = false;
                            break;
                        }
                        k++;
                    }
                    if (matchingNames) {
                        if (i < 15 - 1) {
                            strcpy(userInfo.username, getUsernameList()[i + 1]);
                            drawUserBaseButton(&userInfo, &usernameBaseSelector);
                            while (readInputs()->down) loadMusic();
                            return 0;
                        } else {
                            strcpy(userInfo.username, getUsernameList()[0]);
                            drawUserBaseButton(&userInfo, &usernameBaseSelector);
                            while (readInputs()->down) loadMusic();
                            return 0;
                        }
                    }
                }
            } else if (inputs->right) {
                usernameBaseSelector.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                usernameNumSelector.buttonStatus = BUTTON_STATUS::SELECTED;
                drawUserBaseButton(&userInfo, &usernameBaseSelector);
                drawUserNumButton(&userInfo, &usernameNumSelector);
                while (readInputs()->right) loadMusic();
                return 0;
            }
        } else if (usernameNumSelector.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                if (userInfo.userNum < 99) userInfo.userNum++;
                else userInfo.userNum = 0;
                drawUserNumButton(&userInfo, &usernameNumSelector);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                if (userInfo.userNum > 0) userInfo.userNum--;
                else userInfo.userNum = 99;
                drawUserNumButton(&userInfo, &usernameNumSelector);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                usernameBaseSelector.buttonStatus = BUTTON_STATUS::SELECTED;
                usernameNumSelector.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawUserNumButton(&userInfo, &usernameNumSelector);
                drawUserBaseButton(&userInfo, &usernameBaseSelector);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                loginUser.buttonStatus = BUTTON_STATUS::SELECTED;
                usernameNumSelector.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawUserNumButton(&userInfo, &usernameNumSelector);
                drawLoginButton(&loginUser);
                while (readInputs()->right) loadMusic();
                return 0;
            }
        } else if (loginUser.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->left || inputs->up) {
                loginUser.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                usernameNumSelector.buttonStatus = BUTTON_STATUS::SELECTED;
                drawLoginButton(&loginUser);
                drawUserNumButton(&userInfo, &usernameNumSelector);
                while (readInputs()->left || readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->right || inputs->down) {
                loginUser.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                goToRegistration.buttonStatus = BUTTON_STATUS::SELECTED;
                drawLoginButton(&loginUser);
                drawRegisterButton(&goToRegistration);
                while (readInputs()->right || readInputs()->down) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                if (userExists(userInfo.username, userInfo.userNum)) {
                    printf("User doesn't exists. Please register new user.\n");
                    drawInvalidLoginButton(&loginUser);
                    while (!readInputs()->normalAttack) loadMusic();
                    return 0;
                }
                loginUser.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                zero.buttonStatus = BUTTON_STATUS::SELECTED;
                page = REGISTRATION_PAGE::PASSWORD;
                currentNumButton = 0;
                // Draw all the buttons on the next page
                uLCD.cls();
                uLCD.filled_rectangle(0, 26, 127, 127, DGREY);
                drawPassword(userInfo.password);
                drawBackButton(&backStep);
                drawNextButton(&nextStep);
                drawDeleteButton(&deleteNum);
                drawZeroButton(&zero);
                drawOneButton(&one);
                drawTwoButton(&two);
                drawThreeButton(&three);
                drawFourButton(&four);
                drawFiveButton(&five);
                drawSixButton(&six);
                drawSevenButton(&seven);
                drawEightButton(&eight);
                drawNineButton(&nine);
                while (!readInputs()->normalAttack) loadMusic();
                return 0;
            }
        } else if (goToRegistration.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->left || inputs->up) {
                goToRegistration.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                loginUser.buttonStatus = BUTTON_STATUS::SELECTED;
                drawRegisterButton(&goToRegistration);
                drawLoginButton(&loginUser);
                while (readInputs()->left || readInputs()->up) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                // Go to registration page
                while (!readInputs()->normalAttack) loadMusic();
                return 111;
            }
        }
        break;
    // Password page
    case REGISTRATION_PAGE::PASSWORD:
        if (backStep.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                return 0;
            } else if (inputs->down) {
                currentNumButton = 7;
                seven.buttonStatus = BUTTON_STATUS::SELECTED;
                backStep.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawBackButton(&backStep);
                drawSevenButton(&seven);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                return 0;
            } else if (inputs->right) {
                nextStep.buttonStatus = BUTTON_STATUS::SELECTED;
                backStep.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawBackButton(&backStep);
                drawNextButton(&nextStep);
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                page = REGISTRATION_PAGE::USERNAME;
                backStep.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                usernameBaseSelector.buttonStatus = BUTTON_STATUS::SELECTED;
                // Reset password data for privacy
                for (int i = 0; i < 4; i++) {
                    userInfo.password[i] = NULL;
                }
                // Draw all the buttons on the prev page
                uLCD.cls();
                drawLoginBackground();
                drawLoginButton(&loginUser);
                drawRegisterButton(&goToRegistration);
                drawUserNumButton(&userInfo, &usernameNumSelector);
                drawUserBaseButton(&userInfo, &usernameBaseSelector);
                while (!readInputs()->normalAttack) loadMusic();
                return 0;
            }
        } else if (nextStep.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                return 0;
            } else if (inputs->down) {
                currentNumButton = 9;
                nine.buttonStatus = BUTTON_STATUS::SELECTED;
                nextStep.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNextButton(&nextStep);
                drawNineButton(&nine);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                backStep.buttonStatus = BUTTON_STATUS::SELECTED;
                nextStep.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNextButton(&nextStep);
                drawBackButton(&backStep);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                // Need to check whether password is 4 numbers long and correct
                if (!userInfo.password[3] || !checkPassword(&userInfo)) {
                    drawInvalidNextButton(&nextStep);
                    drawInvalidPassword(userInfo.password);
                    while (!readInputs()->normalAttack) loadMusic();
                    return 0;
                }
                page = REGISTRATION_PAGE::USERNAME;
                userInfo.defaultSkin = getSavedSkin(&userInfo);
                userInfo.highScore = getSavedHighScore(&userInfo);
                userInfo.totalEnemiesKilled = getSavedEnemiesKilled(&userInfo);
                userInfo.totalPlayerDeaths = getSavedPlayerDeaths(&userInfo);
                userInfo.totalPoints = getSavedTotalPoints(&userInfo);
                userInfo.totalPlayTime = getSavedPlayTime(&userInfo);
                // Draw all the buttons on the next page
                while (!readInputs()->normalAttack) loadMusic();
                uLCD.cls();
                return 1;
            }
        } else if (deleteNum.buttonStatus == BUTTON_STATUS::SELECTED) {
            if (inputs->up) {
                currentNumButton = 3;
                three.buttonStatus = BUTTON_STATUS::SELECTED;
                deleteNum.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawDeleteButton(&deleteNum);
                drawThreeButton(&three);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                return 0;
            } else if (inputs->left) {
                currentNumButton = 0;
                zero.buttonStatus = BUTTON_STATUS::SELECTED;
                deleteNum.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawDeleteButton(&deleteNum);
                drawZeroButton(&zero);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 4 - 1; i >= 0; i--) {
                    if (userInfo.password[i] != NULL) {
                        userInfo.password[i] = NULL;
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
        }
        // Password numbers
        switch(currentNumButton) {
        case 0:
            if (inputs->up) {
                currentNumButton = 2;
                two.buttonStatus = BUTTON_STATUS::SELECTED;
                zero.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawZeroButton(&zero);
                drawTwoButton(&two);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                return 0;
            } else if (inputs->left) {
                currentNumButton = 1;
                one.buttonStatus = BUTTON_STATUS::SELECTED;
                zero.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawZeroButton(&zero);
                drawOneButton(&one);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                currentNumButton = -1;
                deleteNum.buttonStatus = BUTTON_STATUS::SELECTED;
                zero.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawZeroButton(&zero);
                drawDeleteButton(&deleteNum);
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 0; i < 4; i++) {
                    if (userInfo.password[i] == NULL) {
                        userInfo.password[i] = '0';
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
            break;
        case 1:
            if (inputs->up) {
                currentNumButton = 4;
                four.buttonStatus = BUTTON_STATUS::SELECTED;
                one.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawOneButton(&one);
                drawFourButton(&four);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                currentNumButton = 0;
                zero.buttonStatus = BUTTON_STATUS::SELECTED;
                one.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawOneButton(&one);
                drawZeroButton(&zero);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                return 0;
            } else if (inputs->right) {
                currentNumButton = 2;
                two.buttonStatus = BUTTON_STATUS::SELECTED;
                one.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawOneButton(&one);
                drawTwoButton(&two);
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 0; i < 4; i++) {
                    if (userInfo.password[i] == NULL) {
                        userInfo.password[i] = '1';
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
            break;
        case 2:
            if (inputs->up) {
                currentNumButton = 5;
                five.buttonStatus = BUTTON_STATUS::SELECTED;
                two.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawTwoButton(&two);
                drawFiveButton(&five);
                while (readInputs()->up);
                return 0;
            } else if (inputs->down) {
                currentNumButton = 0;
                zero.buttonStatus = BUTTON_STATUS::SELECTED;
                two.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawTwoButton(&two);
                drawZeroButton(&zero);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                currentNumButton = 1;
                one.buttonStatus = BUTTON_STATUS::SELECTED;
                two.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawTwoButton(&two);
                drawOneButton(&one);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                currentNumButton = 3;
                three.buttonStatus = BUTTON_STATUS::SELECTED;
                two.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawTwoButton(&two);
                drawThreeButton(&three);
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 0; i < 4; i++) {
                    if (userInfo.password[i] == NULL) {
                        userInfo.password[i] = '2';
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
            break;
        case 3:
            if (inputs->up) {
                currentNumButton = 6;
                six.buttonStatus = BUTTON_STATUS::SELECTED;
                three.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawThreeButton(&three);
                drawSixButton(&six);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                currentNumButton = -1;
                deleteNum.buttonStatus = BUTTON_STATUS::SELECTED;
                three.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawThreeButton(&three);
                drawDeleteButton(&deleteNum);
                while (readInputs()->down);
                return 0;
            } else if (inputs->left) {
                currentNumButton = 2;
                two.buttonStatus = BUTTON_STATUS::SELECTED;
                three.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawThreeButton(&three);
                drawTwoButton(&two);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 0; i < 4; i++) {
                    if (userInfo.password[i] == NULL) {
                        userInfo.password[i] = '3';
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
            break;
        case 4:
            if (inputs->up) {
                currentNumButton = 7;
                seven.buttonStatus = BUTTON_STATUS::SELECTED;
                four.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFourButton(&four);
                drawSevenButton(&seven);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                currentNumButton = 1;
                one.buttonStatus = BUTTON_STATUS::SELECTED;
                four.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFourButton(&four);
                drawOneButton(&one);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                return 0;
            } else if (inputs->right) {
                currentNumButton = 5;
                five.buttonStatus = BUTTON_STATUS::SELECTED;
                four.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFourButton(&four);
                drawFiveButton(&five);
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 0; i < 4; i++) {
                    if (userInfo.password[i] == NULL) {
                        userInfo.password[i] = '4';
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
            break;
        case 5:
            if (inputs->up) {
                currentNumButton = 8;
                eight.buttonStatus = BUTTON_STATUS::SELECTED;
                five.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFiveButton(&five);
                drawEightButton(&eight);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                currentNumButton = 2;
                two.buttonStatus = BUTTON_STATUS::SELECTED;
                five.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFiveButton(&five);
                drawTwoButton(&two);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                currentNumButton = 4;
                four.buttonStatus = BUTTON_STATUS::SELECTED;
                five.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFiveButton(&five);
                drawFourButton(&four);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                currentNumButton = 6;
                six.buttonStatus = BUTTON_STATUS::SELECTED;
                five.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawFiveButton(&five);
                drawSixButton(&six);
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 0; i < 4; i++) {
                    if (userInfo.password[i] == NULL) {
                        userInfo.password[i] = '5';
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
            break;
        case 6:
            if (inputs->up) {
                currentNumButton = 9;
                nine.buttonStatus = BUTTON_STATUS::SELECTED;
                six.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSixButton(&six);
                drawNineButton(&nine);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                currentNumButton = 3;
                three.buttonStatus = BUTTON_STATUS::SELECTED;
                six.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSixButton(&six);
                drawThreeButton(&three);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                currentNumButton = 5;
                five.buttonStatus = BUTTON_STATUS::SELECTED;
                six.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSixButton(&six);
                drawFiveButton(&five);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 0; i < 4; i++) {
                    if (userInfo.password[i] == NULL) {
                        userInfo.password[i] = '6';
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
            break;
        case 7:
            if (inputs->up) {
                currentNumButton = -1;
                backStep.buttonStatus = BUTTON_STATUS::SELECTED;
                seven.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSevenButton(&seven);
                drawBackButton(&backStep);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                currentNumButton = 4;
                four.buttonStatus = BUTTON_STATUS::SELECTED;
                seven.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSevenButton(&seven);
                drawFourButton(&four);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                currentNumButton = -1;
                backStep.buttonStatus = BUTTON_STATUS::SELECTED;
                seven.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSevenButton(&seven);
                drawBackButton(&backStep);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                currentNumButton = 8;
                eight.buttonStatus = BUTTON_STATUS::SELECTED;
                seven.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawSevenButton(&seven);
                drawEightButton(&eight);
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 0; i < 4; i++) {
                    if (userInfo.password[i] == NULL) {
                        userInfo.password[i] = '7';
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
            break;
        case 8:
            if (inputs->up) {
                return 0;
            } else if (inputs->down) {
                currentNumButton = 5;
                five.buttonStatus = BUTTON_STATUS::SELECTED;
                eight.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawEightButton(&eight);
                drawFiveButton(&five);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                currentNumButton = 7;
                seven.buttonStatus = BUTTON_STATUS::SELECTED;
                eight.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawEightButton(&eight);
                drawSevenButton(&seven);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                currentNumButton = 9;
                nine.buttonStatus = BUTTON_STATUS::SELECTED;
                eight.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawEightButton(&eight);
                drawNineButton(&nine);
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 0; i < 4; i++) {
                    if (userInfo.password[i] == NULL) {
                        userInfo.password[i] = '8';
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
            break;
        case 9:
            if (inputs->up) {
                currentNumButton = -1;
                nextStep.buttonStatus = BUTTON_STATUS::SELECTED;
                nine.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNineButton(&nine);
                drawNextButton(&nextStep);
                while (readInputs()->up) loadMusic();
                return 0;
            } else if (inputs->down) {
                currentNumButton = 6;
                six.buttonStatus = BUTTON_STATUS::SELECTED;
                nine.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNineButton(&nine);
                drawSixButton(&six);
                while (readInputs()->down) loadMusic();
                return 0;
            } else if (inputs->left) {
                currentNumButton = 8;
                eight.buttonStatus = BUTTON_STATUS::SELECTED;
                nine.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNineButton(&nine);
                drawEightButton(&eight);
                while (readInputs()->left) loadMusic();
                return 0;
            } else if (inputs->right) {
                currentNumButton = -1;
                nextStep.buttonStatus = BUTTON_STATUS::SELECTED;
                nine.buttonStatus = BUTTON_STATUS::NOT_SELECTED;
                drawNineButton(&nine);
                drawNextButton(&nextStep);
                while (readInputs()->right) loadMusic();
                return 0;
            } else if (!inputs->normalAttack) {
                buttonSound();
                for (int i = 0; i < 4; i++) {
                    if (userInfo.password[i] == NULL) {
                        userInfo.password[i] = '9';
                        break;
                    }
                }
                drawPassword(userInfo.password);
                while (!readInputs()->normalAttack) loadMusic();
                wait_us(250000);
                return 0;
            }
            break;
        }
        break;
    }
    return 0;
}

void deleteLogin(void) {
    free(usernameBaseSelector.boundingBox);
    free(usernameNumSelector.boundingBox);
    free(loginUser.boundingBox);
    free(goToRegistration.boundingBox);
    free(backStep.boundingBox);
    free(nextStep.boundingBox);
    free(deleteNum.boundingBox);
    free(zero.boundingBox);
    free(one.boundingBox);
    free(two.boundingBox);
    free(three.boundingBox);
    free(four.boundingBox);
    free(five.boundingBox);
    free(six.boundingBox);
    free(seven.boundingBox);
    free(eight.boundingBox);
    free(nine.boundingBox);
    uLCD.cls();
}