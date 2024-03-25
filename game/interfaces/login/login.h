#ifndef LOGIN_H
#define LOGIN_H

#include "globals.h"

/** Get information about currently logged in user
 */
USER* getUserInfo();

/** Create a login screen
 */
void loginInit(void);

/** Update the login screen
 */
int loginUpdate(void);

/** Delete the login screen
 */
void deleteLogin(void);

#endif