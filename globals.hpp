#ifndef GLOBAL_H
#define GLOBAL_H
#include "mbed.h"
#include "gameMusic.hpp"
#include "uLCD_4DGL.hpp"
#include <math.h>
#include "SDBlockDevice.h"
#include "FATFileSystem.h"

#define NAME_LENGTH 20
#define PASSWORD_LENGTH 4 + 1 // For null character
#define YELLOW 0xFFFF00
#define GREEN 0x00FF00
#define DGREEN 0x008f26
#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define BROWN  0xD2691E
#define VIOLET 0xa600ff
#define PINK 0xff00fb
#define RED    0xFF0000
#define MRED 0x750000
#define DRED 0x420000
#define BLUE   0x0000FF
#define ORANGE 0xFFA500
#define MGREY 0x8f8f8f

// Character animation states
typedef enum {
    DEFAULT=0,
    LEFT=1,
    RIGHT=2,
    VERTICAL=3,
    DEFAULTDAMAGED=4,
    LEFTDAMAGED=5,
    RIGHTDAMAGED=6,
    VERTICALDAMAGED=7,
    DESTROYED=8,
} CHARACTER_DISPLAY;

// Types of characters
typedef enum {
    PLAYER_CHARACTER,
    OPPONENT_CHARACTER,
    SHORT_RANGE_ENEMY,
    LONG_RANGE_ENEMY,
    MISSILE_ENEMY,
    BOSS_CHARACTER,
} CHARACTER_TYPE;

// Character existence
typedef enum {
    ACTIVE,
    DEAD,
} CHARACTER_STATUS;

// Point cords
typedef struct {
    int x;
    int y;
} point;

// Display element bounds
typedef struct {
    point topLeft;
    point bottomRight;
} BOUNDINGBOX;

// Button status
typedef enum {
    SELECTED,
    NOT_SELECTED,
} BUTTON_STATUS;

// Button struct
typedef struct {
    BUTTON_STATUS buttonStatus;
    BOUNDINGBOX* boundingBox;
} BUTTON;

// Player skin types
typedef enum {
    FIRE_RED = 'R',
    BOLD_ORANGE = 'O',
    STAR_GOLD = 'Y',
    EMERALD_GREEN = 'G',
    ICE_BLUE = 'B',
    PLASMA_VIOLET = 'V',
    HOT_PINK = 'P',
    NONE = NULL,
} PLAYER_SKIN;

// User info
typedef struct {
    char username[NAME_LENGTH];
    int userNum;
    char password[PASSWORD_LENGTH + 1];
    PLAYER_SKIN defaultSkin;
    int totalPlayTime;
    int totalEnemiesKilled;
    int totalPlayerDeaths;
    int totalPoints;
    int highScore;
} USER;

// Game mode types
typedef enum {
    LEVELS = 1,
    SCORECAP = 2,
    INFINITE = 3,
    PVP = 4,
} GAME_MODE;

// Sliding scale struct
typedef struct {
    int value;
    int maxVal;
    BOUNDINGBOX* boundingBox;
    BUTTON_STATUS sliderStatus;
} SLIDING_SCALE;

// Menu settings
typedef struct {
    PLAYER_SKIN playerSkin;
    GAME_MODE gameMode;
    int difficulty;
    int scoreCap;
    double volume;
} MENU_SETTINGS;

// Menu pages
typedef enum {
    MENU_HOME,
    USER_STATS,
} MENU_PAGE;

// === [global objects] ===
extern uLCD_4DGL uLCD;
extern SDBlockDevice sd_block;
extern FATFileSystem fs;

#endif //GLOBAL_H
