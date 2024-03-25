#include <stdlib.h>

#define timer_read_f(x)     chrono::duration<float>((x).elapsed_time()).count()
#define timer_read_s(x)     chrono::duration_cast<chrono::seconds>((x).elapsed_time()).count();
#define timer_read_ms(x)    chrono::duration_cast<chrono::milliseconds>((x).elapsed_time()).count()
#define timer_read_us(x)    (x).elapsed_time().count()

#include "doublyLinkedList.h"
#include "globals.h"
#include "hardware.h"
#include "enemy.h"
#include "finalBoss.h"
#include "player.h"
#include "game.h"

int main()
{
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    printf("Program Starting\n");
    while (1) {
        menuInit();
        int menuOutput;
        while ( (menuOutput = menuUpdate()) == 0);
        if (menuOutput == 999) break;
        uLCD.cls();
        playerInit();
        
        if (getMenuSettings()->gameMode == GAME_MODE::LEVELS) {
            generateGameLoop(1);
        } else if (getMenuSettings()->gameMode == GAME_MODE::INFINITE) {
            generateGameLoop(4);
        }

        getPlayer()->score = 0;
    }
    deletePlayer();
    deallocateHardware();
    deleteMenu();
    printf("Stopping Program\n");
    return 0;
}