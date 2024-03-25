#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "graphics.h"
#include "users.h"

typedef enum {
    USERNAME,
    PASSWORD,
    DEFAULT_SKIN,
} REGISTRATION_PAGE;

/** Get information about new user
 */
USER* getNewUser(void);

/** Create a registration screen
 */
void registrationInit(void);

/** Update the registration screen
 */
int registrationUpdate(void);

/** Delete the registration screen and data
 */
void deleteRegistration(void);

/** Register a user
 */
void registerNewUser(USER* userInfo);

#endif