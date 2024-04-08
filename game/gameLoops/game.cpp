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
    gameLoop = (GAMELOOP*)malloc(sizeof(GAMELOOP));
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
    uLCD.cls();
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
    uLCD.cls();
    getPlayer()->superActive = false;
    deallocateEnemies();
}

void levelTwo(void) {
    uLCD.cls();
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
    uLCD.cls();
    getPlayer()->superActive = false;
    deallocateEnemies();
}

void levelThree(void) {
    uLCD.cls();
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
    uLCD.cls();
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteBoss();
}

void infiniteDuration(void) {
    uLCD.cls();
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
    uLCD.cls();
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteBoss();
}

void scoreCap(void) {
    uLCD.cls();
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
    uLCD.cls();
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteBoss();
}

void pvp(void) {
    uLCD.cls();
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
    uLCD.cls();
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteOpponent();
}

void testLevel(void) {
    uLCD.cls();
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
    uLCD.cls();
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
        if (!readSyncState()) {
            syncSignal(true);
            while (readSyncState());
            wait_us(100);
        } else {
            syncSignal(false);
            wait_us(100);
        }
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
            int msgLength = 0;
            while (message[msgLength]) {
                msgLength++;
            }
            int j = 0;
            while (message[j]) {
                loadMusic();
                uLCD.text_width(2);
                uLCD.text_height(2);
                uLCD.text_bold(ON);
                uLCD.locate(5 - (int)round((double)msgLength / 2) + j, 3);
                uLCD.color(getHexColor(countDownColors[i], false));
                uLCD.textbackground_color(BLACK);
                uLCD.printf("%c", message[j]);
                j++;
            }
            // Reset text size
            uLCD.text_width(1);
            uLCD.text_height(1);
            while (t.elapsed_time().count() < 1000000) loadMusic();
            // Sync countdown with opponent
            if (!readSyncState()) {
                syncSignal(true);
                while (readSyncState());
                wait_us(100);
            } else {
                syncSignal(false);
                wait_us(100);
            }
            t.reset();
        }
        drawBox(0, 40, 127, 70, '0');
    }
    int run = 0;
    while (1) {
        loadMusic();
        t.start();
        if (pvp) {
            // For pvp, need to sync game ticks for both devices
            if (!readSyncState()) {
                syncSignal(true);
                while (readSyncState());
                wait_us(100);
            } else {
                syncSignal(false);
                wait_us(100);
            }
        }
        GAME_INPUTS* gameInputs = readMyInputs();
        if (pvp) {
            // For pvp, need to sync game ticks for both devices
            if (!readSyncState()) {
                syncSignal(true);
                while (readSyncState());
                wait_us(100);
            } else {
                syncSignal(false);
                wait_us(100);
            }
            gameInputs = readOpponentInputs();
        }
        printf("\033[F%d\n", run++);
        //printf("\033[FMe: %d, %d, %d, %d Op: %d, %d, %d, %d | %d", gameInputs->up, gameInputs->down, gameInputs->left, gameInputs->right, gameInputs->opUp, gameInputs->opDown, gameInputs->opLeft, gameInputs->opRight, run++);
        //wait_us(10000000);
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
        if (getMenuSettings()->gameMode != GAME_MODE::PVP) {
            uLCD.color(WHITE);
            uLCD.locate(0, 0);
            uLCD.textbackground_color(BLACK);
            if (getPlayer()->score >= 1000) {
                uLCD.printf("SCORE: %dK\r", getPlayer()->score / 1000);
            } else if (getPlayer()->score >= 100) {
                uLCD.printf("SCORE:%d\r", getPlayer()->score);
            } else {
                uLCD.printf("SCORE: %d\r", getPlayer()->score);
            }
            uLCD.locate(10, 0);
            if (getMenuSettings()->gameMode == GAME_MODE::INFINITE) {
                uLCD.printf("INFINITE\r");
            } else if (getMenuSettings()->gameMode == GAME_MODE::LEVELS) {
                uLCD.printf("LEVEL: %d\r", gameLoop->level);
            } else if (getMenuSettings()->gameMode == GAME_MODE::SCORECAP) {
                if (getMenuSettings()->scoreCap == 100) {
                    uLCD.printf("GOAL:100\r");
                } else {
                    uLCD.printf("GOAL: %d\r", getMenuSettings()->scoreCap);
                }
            }
        } else { // If gamemode is pvp then display score ratio
            if (getPlayer()->score > getOpponent()->score) {
                uLCD.color(GREEN);
            } else if (getPlayer()->score < getOpponent()->score){
                uLCD.color(RED);
            } else {
                uLCD.color(WHITE);
            }
            uLCD.textbackground_color(BLACK);
            if (getPlayer()->score >= 1000) {
                uLCD.locate(6, 0);
                uLCD.printf("%dK\r", getPlayer()->score / 1000);
            } else if (getPlayer()->score >= 100) {
                uLCD.locate(5, 0);
                uLCD.printf("%d\r", getPlayer()->score);
            } else if (getPlayer()->score >= 10) {
                uLCD.locate(6, 0);
                uLCD.printf("%d\r", getPlayer()->score);
            } else {
                uLCD.locate(7, 0);
                uLCD.printf("%d\r", getPlayer()->score);
            }
            uLCD.locate(10, 0);
            if (getOpponent()->score >= 1000) {
                uLCD.printf("%dK\r", getOpponent()->score / 1000);
            } else {
                uLCD.printf("%d\r", getOpponent()->score);
            }
        }
        if (pvp) {
            // For pvp, need to sync game ticks for both devices
            if (!readSyncState()) {
                syncSignal(true);
                while (readSyncState());
                wait_us(100);
            } else {
                syncSignal(false);
                wait_us(100);
            }
        }
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
                notifyPvp(true);
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
                if (pvp) {
                    // For pvp, need to sync game ticks for both devices
                    if (!readSyncState()) {
                        syncSignal(true);
                        while (readSyncState());
                        wait_us(100);
                    } else {
                        syncSignal(false);
                        wait_us(100);
                    }
                }
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
            notifyPvp(true);
            break;
        } else if (getEnemyDLL()->size == 0 && getBoss() == NULL && getOpponent() == NULL) {
            if (!scoreCap && !infinite) {
                gameLoop->gameStatus = GAMESTATUS::WON;
                notifyPvp(true);
            }
            break;
        } else if (pvp && getPlayer()->score >= 10 || getOpponent()->score >= 10) {
            if (getPlayer()->score >= 10 && getOpponent()->score < 10) {
                gameLoop->gameStatus = GAMESTATUS::WON;
                notifyPvp(true);
                break;
            } else if (getPlayer()->score < 10 && getOpponent()->score >= 10) {
                gameLoop->gameStatus = GAMESTATUS::LOST;
                break;
            } else if (getPlayer()->score >= 10 && getOpponent()->score >= 10) {
                if (getPlayer()->score > getOpponent()->score) {
                    gameLoop->gameStatus = GAMESTATUS::WON;
                    notifyPvp(true);
                    break;
                } else if (getPlayer()->score < getOpponent()->score) {
                    gameLoop->gameStatus = GAMESTATUS::LOST;
                    notifyPvp(true);
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