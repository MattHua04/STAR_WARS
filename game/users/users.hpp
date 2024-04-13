#ifndef USERS_H
#define USERS_H

#include "mbed.h"
#include "globals.hpp"
#include "graphics.hpp"
#include "login.hpp"
#include "menu.hpp"

/** Get a list of usernames
 */
char (*getUsernameList())[20];

/** Convert user credentials to a file name
 */
void generateFileName(USER* user);

/** Get currently saved player default skin
 */
PLAYER_SKIN getSavedSkin(USER* user);

/** Get currently saved user high score
 */
int getSavedHighScore(USER* user);

/** Get currently saved user enemies killed
 */
int getSavedEnemiesKilled(USER* user);

/** Get currently saved player deaths
 */
int getSavedPlayerDeaths(USER* user);

/** Get currently saved user total points
 */
int getSavedTotalPoints(USER* user);

/** Get currently saved user play time
 */
int getSavedPlayTime(USER* user);

/** Get how many times user file has been updated
 */
int getUpdateCount(USER* user);

/** Check if a username exists
 */
int userExists(char username[NAME_LENGTH], int userNum);

/** Check if password is correct for entered username
 */
int checkPassword(USER* user);

/** Update a user.
 * User session data should be updated beforehand, this function will combine session data with saved data.
 */
void updateUser(USER* user);

/** Marks a user file as deleted
 * Actual file removal is handled by the RPi syncing local and external data
 */
void deleteUser(USER* user);

#endif