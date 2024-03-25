#ifndef MENUS_H
#define MENUS_H

#include "mbed.h"
#include "globals.h"
#include "graphics.h"

/** Get menu settings to determine game loop settings
 */
MENU_SETTINGS* getMenuSettings();

/** Create a menu page
 */
void menuInit(void);

/** Update the menu
 */
int menuUpdate(void);

/** Delete the menu
 */
void deleteMenu(void);

#endif