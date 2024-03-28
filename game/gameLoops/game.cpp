#include "globals.h"
#include "doublyLinkedList.h"
#include "graphics.h"
#include "healthBar.h"
#include "menu.h"
#include "player.h"
#include "enemy.h"
#include "finalBoss.h"
#include "hardware.h"
#include "game.h"
#include "projectile.h"

Timer randNumSeed;

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
    start(false, false);
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        wait_us(3000000);
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        wait_us(3000000);
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
    start(false, false);
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        wait_us(3000000);
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        wait_us(3000000);
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
    start(false, false);
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        wait_us(3000000);
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        wait_us(3000000);
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
        start(true, false);
        if (gameLoop->gameStatus == GAMESTATUS::WON || gameLoop->gameStatus == GAMESTATUS::LOST) {
            break;
        }
    }
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        wait_us(3000000);
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        wait_us(3000000);
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
        start(false, true);
        if (gameLoop->gameStatus == GAMESTATUS::WON || gameLoop->gameStatus == GAMESTATUS::LOST) {
            break;
        }
    }
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        wait_us(3000000);
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        wait_us(3000000);
    }
    uLCD.cls();
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteBoss();
}

void testLevel(void) {
    uLCD.cls();
    enemyInit();
    bossInit();
    projectileInit();
    healthBarInit();
    //enemyGenerator(CHARACTER_TYPE::LONG_RANGE_ENEMY);
    start(false, false);
    if (gameLoop->gameStatus == GAMESTATUS::WON) {
        drawGameWon();
        winSound();
        wait_us(3000000);
    } else if (gameLoop->gameStatus == GAMESTATUS::LOST) {
        drawGameLost();
        loseSound();
        wait_us(3000000);
    }
    uLCD.cls();
    getPlayer()->superActive = false;
    deallocateEnemies();
    deleteBoss();
}

void start(bool infinite, bool scoreCap) {
    Timer t;
    // Stop loop when all enemies are destroyed or player is destroyed
    while (1) {
        loadMusic();
        t.start();
        // Update projectiles, player, enemies, boss, and healthBars
        //printf("Started updating stuff\n");
        //printf("ri\n");
        updatePlayerProjectiles();
        //printf("pp\n");
        updateBossProjectiles();
        //printf("bp\n");
        updateEnemyProjectiles();
        //printf("ep\n");
        playerUpdate();
        //printf("pu\n");
        bossUpdate();
        //printf("bu\n");
        enemiesUpdate();
        //printf("eu\n");
        updateBars();
        //printf("Finished updating stuff\n");
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
        // Check for pause
        GAME_INPUTS* gameInputs = readInputs();
        if (!gameInputs->quitGame) {
            Timer quit;
            bool quitting = true;
            quit.start();
            while (quitting && quit.elapsed_time().count() < 3000000) {
                loadMusic();
                if (readInputs()->quitGame) {
                    quitting = false;
                }
            }
            if (quitting) {
                gameLoop->gameStatus = GAMESTATUS::LOST;
                break;
            }
        }
        if (gameInputs->normalAttack && gameInputs->superAttack && !gameInputs->pauseResume) {
            printf("Game Paused\n");
            drawPaused();
            getGameLoop()->gameStatus = GAMESTATUS::PAUSED;
            while ((gameInputs = readInputs()) && gameInputs->normalAttack && gameInputs->superAttack && !gameInputs->pauseResume) loadMusic();
            // Pause
            while ((gameInputs = readInputs())) {
                loadMusic();
                if (gameInputs->normalAttack && gameInputs->superAttack && !gameInputs->pauseResume) {
                    printf("Game Resumed\n");
                    erasePaused();
                    drawResumed();
                    getGameLoop()->gameStatus = GAMESTATUS::RUNNING;
                    while ((gameInputs = readInputs()) && gameInputs->normalAttack && gameInputs->superAttack && !gameInputs->pauseResume) loadMusic();
                    eraseResumed();
                    break;
                }
            }
        }
        // Check for game win or loss
        if (getPlayer()->playerStatus == CHARACTER_STATUS::DEAD && getPlayer()->destroyedTick == 20) {
            gameLoop->gameStatus = GAMESTATUS::LOST;
            break;
        } else if (getEnemyDLL()->size == 0 && getBoss() == NULL) {
            if (!scoreCap && !infinite) {
                gameLoop->gameStatus = GAMESTATUS::WON;
            }
            break;
        } else if (scoreCap && getPlayer()->score >= getMenuSettings()->scoreCap) {
            gameLoop->gameStatus = GAMESTATUS::WON;
            break;
        } else if (infinite && getPlayer()->score >= 10000) {
            gameLoop->gameStatus = GAMESTATUS::WON;
            break;
        }
        int waitTime = 50000 - t.elapsed_time().count();
        if (waitTime > 0) {
            wait_us(waitTime);
        }
        getPlayer()->sessionPlayTime += t.elapsed_time().count();
        t.reset();
    }
}