#include "gameMusic.hpp"
#include "globals.hpp"
#include "doublyLinkedList.hpp"
#include "graphics.hpp"
#include "healthBar.hpp"
#include "menu.hpp"
#include "opponent.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "finalBoss.hpp"
#include "hardware.hpp"
#include "game.hpp"
#include "projectile.hpp"
#include <cstdio>

Timer randNumSeed;
int inGame = 0;

GAMELOOP* gameLoop = NULL;

GAMELOOP* getGameLoop(void) {
    return gameLoop;
}

int randomNum(void) {
    srand(randNumSeed.elapsed_time().count());
    return rand();
}

void generateGameLoop(int level) {
    randNumSeed.start();
    if (!gameLoop) gameLoop = (GAMELOOP*)malloc(sizeof(GAMELOOP));
    gameLoop->gameStatus = GAMESTATUS::RUNNING;
    gameLoop->level = level;
    switch (gameLoop->level) {
    case 0:
        testLevel();
        if (gameLoop->gameStatus == GAMESTATUS::LOST) break;
        gameLoop->level++;
    case 1:
        levelOne();
        if (gameLoop->gameStatus == GAMESTATUS::LOST) break;
        gameLoop->level++;
    case 2:
        levelTwo();
        if (gameLoop->gameStatus == GAMESTATUS::LOST) break;
        gameLoop->level++;
    case 3:
        levelThree();
        break;
    case 4:
        infiniteDuration();
        break;
    case 5:
        scoreCap();
        break;
    case 6:
        pvp();
        break;
    }
    randNumSeed.reset();
}

void levelOne(void) {
    drawGameBackground(false);
    enemyInit();
    projectileInit();
    healthBarInit();
    for (int i = 0; i < getMenuSettings()->difficulty; i++) {
        enemyGenerator(CHARACTER_TYPE::SHORT_RANGE_ENEMY);
    }
    start(false, false, false);
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        Timer won;
        won.start();
        while (won.elapsed_time().count() < 3000000) loadMusic();
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        Timer lost;
        lost.start();
        while (lost.elapsed_time().count() < 3000000) loadMusic();
    }
    getPlayer()->superActive = false;
    deallocateEnemies();
}

void levelTwo(void) {
    drawGameBackground(false);
    enemyInit();
    projectileInit();
    healthBarInit();
    for (int i = 0; i < (int)round((double)getMenuSettings()->difficulty / 2); i++) {
        enemyGenerator(CHARACTER_TYPE::SHORT_RANGE_ENEMY);
    }
    for (int i = 0; i < (int)round((double)getMenuSettings()->difficulty / 2); i++) {
        enemyGenerator(CHARACTER_TYPE::LONG_RANGE_ENEMY);
    }
    enemyGenerator(CHARACTER_TYPE::MISSILE_ENEMY);
    start(false, false, false);
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        Timer won;
        won.start();
        while (won.elapsed_time().count() < 3000000) loadMusic();
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        Timer lost;
        lost.start();
        while (lost.elapsed_time().count() < 3000000) loadMusic();
    }
    getPlayer()->superActive = false;
    deallocateEnemies();
}

void levelThree(void) {
    drawGameBackground(false);
    enemyInit();
    bossInit();
    projectileInit();
    healthBarInit();
    for (int i = 0; i < getMenuSettings()->difficulty - 1; i++) {
        enemyGenerator(CHARACTER_TYPE::SHORT_RANGE_ENEMY);
    }
    for (int i = 0; i < (int)round((double)(getMenuSettings()->difficulty - 1) / 2); i++) {
        enemyGenerator(CHARACTER_TYPE::LONG_RANGE_ENEMY);
    }
    start(false, false, false);
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        Timer won;
        won.start();
        while (won.elapsed_time().count() < 3000000) loadMusic();
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        Timer lost;
        lost.start();
        while (lost.elapsed_time().count() < 3000000) loadMusic();
    }
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteBoss();
}

void infiniteDuration(void) {
    drawGameBackground(false);
    enemyInit();
    projectileInit();
    healthBarInit();
    // Stop loop once player score has reached 10
    while (1) {
        switch(rand() % 3) {
        case 0:
            for (int i = 0; i < getMenuSettings()->difficulty; i++) {
                enemyGenerator(CHARACTER_TYPE::SHORT_RANGE_ENEMY);
            }
            break;
        case 1:
            for (int i = 0; i < (int)round((double)getMenuSettings()->difficulty / 2); i++) {
                enemyGenerator(CHARACTER_TYPE::SHORT_RANGE_ENEMY);
            }
            for (int i = 0; i < (int)round((double)getMenuSettings()->difficulty / 2); i++) {
                enemyGenerator(CHARACTER_TYPE::LONG_RANGE_ENEMY);
            }
            enemyGenerator(CHARACTER_TYPE::MISSILE_ENEMY);
            break;
        case 2:
            bossInit();
            for (int i = 0; i < getMenuSettings()->difficulty - 1; i++) {
                enemyGenerator(CHARACTER_TYPE::SHORT_RANGE_ENEMY);
            }
            for (int i = 0; i < (int)round((double)(getMenuSettings()->difficulty - 1) / 2); i++) {
                enemyGenerator(CHARACTER_TYPE::LONG_RANGE_ENEMY);
            }
        }
        start(true, false, false);
        if (gameLoop->gameStatus == GAMESTATUS::WON || gameLoop->gameStatus == GAMESTATUS::LOST) {
            break;
        }
    }
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        Timer won;
        won.start();
        while (won.elapsed_time().count() < 3000000) loadMusic();
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        Timer lost;
        lost.start();
        while (lost.elapsed_time().count() < 3000000) loadMusic();
    }
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteBoss();
}

void scoreCap(void) {
    drawGameBackground(false);
    enemyInit();
    projectileInit();
    healthBarInit();
    // Stop loop once player score has reached 10
    while (1) {
        switch(rand() % 3) {
        case 0:
            for (int i = 0; i < getMenuSettings()->difficulty; i++) {
                enemyGenerator(CHARACTER_TYPE::SHORT_RANGE_ENEMY);
            }
            break;
        case 1:
            for (int i = 0; i < (int)round((double)getMenuSettings()->difficulty / 2); i++) {
                enemyGenerator(CHARACTER_TYPE::SHORT_RANGE_ENEMY);
            }
            for (int i = 0; i < (int)round((double)getMenuSettings()->difficulty / 2); i++) {
                enemyGenerator(CHARACTER_TYPE::LONG_RANGE_ENEMY);
            }
            enemyGenerator(CHARACTER_TYPE::MISSILE_ENEMY);
            break;
        case 2:
            bossInit();
            for (int i = 0; i < getMenuSettings()->difficulty - 1; i++) {
                enemyGenerator(CHARACTER_TYPE::SHORT_RANGE_ENEMY);
            }
            for (int i = 0; i < (int)round((double)(getMenuSettings()->difficulty - 1) / 2); i++) {
                enemyGenerator(CHARACTER_TYPE::LONG_RANGE_ENEMY);
            }
        }
        start(false, true, false);
        if (gameLoop->gameStatus == GAMESTATUS::WON || gameLoop->gameStatus == GAMESTATUS::LOST) {
            break;
        }
    }
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        Timer won;
        won.start();
        while (won.elapsed_time().count() < 3000000) loadMusic();
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        Timer lost;
        lost.start();
        while (lost.elapsed_time().count() < 3000000) loadMusic();
    }
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteBoss();
}

void pvp(void) {
    drawGameBackground(false);
    enemyInit();
    opponentInit();
    projectileInit();
    healthBarInit();
    start(false, false, true);
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        Timer won;
        won.start();
        while (won.elapsed_time().count() < 3000000) loadMusic();
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        Timer lost;
        lost.start();
        while (lost.elapsed_time().count() < 3000000) loadMusic();
    }
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteOpponent();
}

void testLevel(void) {
    drawGameBackground(false);
    enemyInit();
    bossInit();
    projectileInit();
    healthBarInit();
    start(false, false, false);
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        Timer won;
        won.start();
        while (won.elapsed_time().count() < 3000000) loadMusic();
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        Timer lost;
        lost.start();
        while (lost.elapsed_time().count() < 3000000) loadMusic();
    }
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteBoss();
}

void start(bool infinite, bool scoreCap, bool pvp) {
    Timer t;
    // Count down for pvp
    if (getMenuSettings()->gameMode == GAME_MODE::PVP) {
        updatePlayerProjectiles();
        updateOpponentProjectiles();
        updateBossProjectiles();
        updateEnemyProjectiles();
        playerUpdate();
        opponentUpdate();
        bossUpdate();
        enemiesUpdate();
        updateBars();
        t.start();
        while (t.elapsed_time().count() < 1000000) loadMusic();
        // Sync countdown with opponent
        syncDevices();
        t.reset();
        char countDownColors[4] = {'G', 'Y', 'O', 'R'};
        for (int i = 3; i >= 0; i--) {
            char message[10];
            if (i == 0) {
                winSound();
                strcpy(message, "FIGHT!");
            } else {
                buttonSound();
                sprintf(message, "%d", i);
            }
            int msgLength = strlen(message);
            int color = getHexColor(countDownColors[i], false);
            drawText(message, 64 + 1 - (int) round(7 * (double) msgLength / 2), 64 - 4, color);
            while (t.elapsed_time().count() < 1000000) loadMusic();
            // Sync countdown with opponent
            syncDevices();
            t.reset();
        }
        drawBackgroundBox(0, 40, 127, 70);
    }
    int run = 0;
    while (1) {
        loadMusic();
        t.start();
        if (pvp) syncDevices(); // For pvp, need to sync game ticks for both devices
        GAME_INPUTS* gameInputs = readMyInputs();
        if (pvp) {
            // For pvp, need to sync game ticks for both devices
            syncDevices();
            gameInputs = readOpponentInputs();
        }
        // Update everything
        updatePlayerProjectiles();
        updateOpponentProjectiles();
        updateBossProjectiles();
        updateEnemyProjectiles();
        playerUpdate();
        opponentUpdate();
        bossUpdate();
        enemiesUpdate();
        updateBars();
        // Print game info
        uLCD.max_col = 18;
        if (getMenuSettings()->gameMode != GAME_MODE::PVP) {
            int color = WHITE;
            char message[20];
            if (getPlayer()->score >= 1000) {
                sprintf(message, "SCORE:%.1fK", (double) getPlayer()->score / 1000);
                drawText(message, 0, 1, color);
            } else {
                sprintf(message, "SCORE:%d", getPlayer()->score);
                drawText(message, 0, 1, color);
            }
            if (getMenuSettings()->gameMode == GAME_MODE::INFINITE) {
                strcpy(message, "INFINITE");
                drawText(message, 127 - 1 - 7 * strlen(message), 1, color);
            } else if (getMenuSettings()->gameMode == GAME_MODE::LEVELS) {
                sprintf(message, "LEVEL:%d", gameLoop->level);
                drawText(message, 127 - 7 * strlen(message), 1, color);
            } else if (getMenuSettings()->gameMode == GAME_MODE::SCORECAP) {
                if (getMenuSettings()->scoreCap == 100) {
                    strcpy(message, "GOAL:100");
                    drawText(message, 127 - 7 * strlen(message), 1, color);
                } else {
                    sprintf(message, "GOAL:%d", getMenuSettings()->scoreCap);
                    drawText(message, 127 - 7 * strlen(message), 1, color);
                }
            }
        } else if (getOpponent() != NULL) { // If gamemode is pvp then display score ratio
            int color;
            char message[10];
            if (getPlayer()->score > getOpponent()->score) {
                color = GREEN;
            } else if (getPlayer()->score < getOpponent()->score){
                color = RED;
            } else {
                color = WHITE;
            }
            if (getPlayer()->score >= 1000) {
                sprintf(message, "%dK", getPlayer()->score / 1000);
                drawText(message, 64 - 7 - 7 * strlen(message), 1, color);
            } else {
                sprintf(message, "%d", getPlayer()->score);
                drawText(message, 64 - 7 - 7 * strlen(message), 1, color);
            }
            if (getOpponent()->score >= 1000) {
                sprintf(message, "%dK", getOpponent()->score / 1000);
                drawText(message, 64 + 7, 1, color);
            } else {
                sprintf(message, "%d", getOpponent()->score);
                drawText(message, 64 + 7, 1, color);
            }
        }
        if (pvp) syncDevices(); // For pvp, need to sync game ticks for both devices
        // Check for quit
        if (!gameInputs->quitGame || (getMenuSettings()->gameMode == GAME_MODE::PVP && !gameInputs->opQuitGame)) {
            Timer quit;
            bool quitting = true;
            quit.start();
            while ((gameInputs = readInputs()) && quitting && quit.elapsed_time().count() < 3000000) {
                loadMusic();
                if ((gameInputs->quitGame && getMenuSettings()->gameMode == GAME_MODE::PVP && gameInputs->opQuitGame) || (gameInputs->quitGame && getMenuSettings()->gameMode != GAME_MODE::PVP)) {
                    quitting = false;
                }
            }
            if (quitting) {
                gameLoop->gameStatus = GAMESTATUS::LOST;
                
                break;
            }
        }
        // Check for pause
        if ((gameInputs->normalAttack && gameInputs->superAttack && !gameInputs->pauseResume) || (getMenuSettings()->gameMode == GAME_MODE::PVP && gameInputs->opNormalAttack && gameInputs->opSuperAttack && !gameInputs->opPauseResume)) {
            printf("Game Paused\n");
            drawPaused();
            getGameLoop()->gameStatus = GAMESTATUS::PAUSED;
            while ((gameInputs = readInputs()) && gameInputs->normalAttack && gameInputs->superAttack && !gameInputs->pauseResume || (getMenuSettings()->gameMode == GAME_MODE::PVP && gameInputs->opNormalAttack && gameInputs->opSuperAttack && !gameInputs->opPauseResume)) loadMusic();
            // Pause
            while (1) {
                loadMusic();
                if (pvp) syncDevices(); // For pvp, need to sync game ticks for both devices
                gameInputs = readInputs();
                if (gameInputs->normalAttack && gameInputs->superAttack && !gameInputs->pauseResume || (getMenuSettings()->gameMode == GAME_MODE::PVP && gameInputs->opNormalAttack && gameInputs->opSuperAttack && !gameInputs->opPauseResume)) {
                    printf("Game Resumed\n");
                    erasePaused();
                    drawResumed();
                    getGameLoop()->gameStatus = GAMESTATUS::RUNNING;
                    while ((gameInputs = readInputs()) && gameInputs->normalAttack && gameInputs->superAttack && !gameInputs->pauseResume || (getMenuSettings()->gameMode == GAME_MODE::PVP && gameInputs->opNormalAttack && gameInputs->opSuperAttack && !gameInputs->opPauseResume)) loadMusic();
                    eraseResumed();
                    break;
                }
            }
        }
        // Check for game win or loss
        if (getPlayer()->playerStatus == CHARACTER_STATUS::DEAD && getPlayer()->destroyedTick == 20) {
            gameLoop->gameStatus = GAMESTATUS::LOST;
            break;
        } else if (getEnemyDLL()->size == 0 && getBoss() == NULL && getOpponent() == NULL) {
            if (!scoreCap && !infinite) {
                gameLoop->gameStatus = GAMESTATUS::WON;
            }
            break;
        } else if (pvp && getPlayer()->score >= 10 || getOpponent()->score >= 10) {
            if (getPlayer()->score >= 10 && getOpponent()->score < 10) {
                gameLoop->gameStatus = GAMESTATUS::WON;
                break;
            } else if (getPlayer()->score < 10 && getOpponent()->score >= 10) {
                gameLoop->gameStatus = GAMESTATUS::LOST;
                break;
            } else if (getPlayer()->score >= 10 && getOpponent()->score >= 10) {
                if (getPlayer()->score > getOpponent()->score) {
                    gameLoop->gameStatus = GAMESTATUS::WON;
                    break;
                } else if (getPlayer()->score < getOpponent()->score) {
                    gameLoop->gameStatus = GAMESTATUS::LOST;
                    break;
                }
            }
        } else if (scoreCap && getPlayer()->score >= getMenuSettings()->scoreCap) {
            gameLoop->gameStatus = GAMESTATUS::WON;
            break;
        } else if (infinite && getPlayer()->score >= 10000) {
            gameLoop->gameStatus = GAMESTATUS::WON;
            break;
        }
        int waitTime = 50000 - t.elapsed_time().count();
        Timer waitForGameTick;
        waitForGameTick.start();
        while (waitForGameTick.elapsed_time().count() < waitTime) loadMusic();
        getPlayer()->sessionPlayTime += t.elapsed_time().count();
        t.reset();
    }
}

void resetGameLoop(void) {
    free(gameLoop);
    gameLoop = NULL;
}