#ifndef _UNITTEST_HELPERS_H
#define _UNITTEST_HELPERS_H

#define CONTINUE_ON_FAIL 1

typedef enum { false, true } bool;

void assert(bool val, char *description);
void resetGameState(struct gameState *G, int numPlayers, int kingdomCards[10]);
bool sameHand(struct gameState *state, struct gameState *prevState, int player);
bool sameDeck(struct gameState *state, struct gameState *prevState, int player);
bool sameDiscard(struct gameState *state, struct gameState *prevState, int player);
#endif