#ifndef OPPONENT_H
#define OPPONENT_H

#include "doublyLinkedList.hpp"
#include "mbed.h"
#include "healthBar.hpp"
#include "globals.hpp"
#include "graphics.hpp"
#include "player.hpp"

/** Get the opponent object from other files
 */
PLAYER* getOpponent(void);

/** Create an opponent and healthBar
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