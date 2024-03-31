#ifndef OPPONENT_H
#define OPPONENT_H

#include "doublyLinkedList.h"
#include "mbed.h"
#include "healthBar.h"
#include "globals.h"
#include "graphics.h"
#include "player.h"

/** Get the opponent object from other files
 */
PLAYER* getOpponent(void);

/** Create a opponent and healthBar
*/
void opponentInit(void);

/** Move up if possible
*/
void opponentMoveUp(void);

/** Move down if possible
*/
void opponentMoveDown(void);

/** Move left if possible
*/
void opponentMoveLeft(void);

/** Move right if possible
*/
void opponentMoveRight(void);

/** Fire a normal opponent projectile
 */
void opponentFireProjectile(void);

/** Activate opponent laser attack
 */
void opponentSuperAttack(void);

/** Update opponent position and render opponent using drawOpponent
 */
void opponentUpdate(void);

/** Deallocate opponent memory
 */
void deleteOpponent(void);

#endif